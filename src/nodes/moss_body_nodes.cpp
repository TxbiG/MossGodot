#include "moss_body_nodes.h"
#include "moss_world_nodes.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <cmath>

using namespace godot;
using namespace mossgodot;

static ::EMotionType to_moss_motion(MossBodyMode mode) {
    switch (mode) {
        case MossBodyMode::Static: return ::EMotionType::Static;
        case MossBodyMode::Kinematic: return ::EMotionType::Kinematic;
        default: return ::EMotionType::Dynamic;
    }
}

void MossRigidBody3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_shape", "shape"), &MossRigidBody3D::set_shape);
    ClassDB::bind_method(D_METHOD("get_shape"), &MossRigidBody3D::get_shape);
    ClassDB::bind_method(D_METHOD("set_body_mode", "mode"), &MossRigidBody3D::set_body_mode);
    ClassDB::bind_method(D_METHOD("get_body_mode"), &MossRigidBody3D::get_body_mode);
    ClassDB::bind_method(D_METHOD("set_mass", "mass"), &MossRigidBody3D::set_mass);
    ClassDB::bind_method(D_METHOD("get_mass"), &MossRigidBody3D::get_mass);
    ClassDB::bind_method(D_METHOD("set_sensor", "sensor"), &MossRigidBody3D::set_sensor);
    ClassDB::bind_method(D_METHOD("is_sensor"), &MossRigidBody3D::is_sensor);
    ClassDB::bind_method(D_METHOD("set_collision_layer", "layer"), &MossRigidBody3D::set_collision_layer);
    ClassDB::bind_method(D_METHOD("get_collision_layer"), &MossRigidBody3D::get_collision_layer);
    ClassDB::bind_method(D_METHOD("set_friction", "friction"), &MossRigidBody3D::set_friction);
    ClassDB::bind_method(D_METHOD("get_friction"), &MossRigidBody3D::get_friction);
    ClassDB::bind_method(D_METHOD("set_restitution", "restitution"), &MossRigidBody3D::set_restitution);
    ClassDB::bind_method(D_METHOD("get_restitution"), &MossRigidBody3D::get_restitution);
    ClassDB::bind_method(D_METHOD("apply_impulse", "impulse"), &MossRigidBody3D::apply_impulse);
    ClassDB::bind_method(D_METHOD("set_linear_velocity", "velocity"), &MossRigidBody3D::set_linear_velocity);
    ClassDB::bind_method(D_METHOD("move_kinematic", "target", "delta"), &MossRigidBody3D::move_kinematic);
    ClassDB::bind_method(D_METHOD("is_moss_active"), &MossRigidBody3D::is_moss_active);
    ClassDB::bind_method(D_METHOD("_moss_deferred_register"), &MossRigidBody3D::try_register_with_world);
    ClassDB::bind_method(D_METHOD("_moss_shape_changed"), &MossRigidBody3D::_moss_shape_changed);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "MossShape3D"), "set_shape", "get_shape");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "body_mode", PROPERTY_HINT_ENUM, "Static,Kinematic,Dynamic"), "set_body_mode", "get_body_mode");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mass", PROPERTY_HINT_RANGE, "0.001,100000,0.01,or_greater"), "set_mass", "get_mass");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sensor"), "set_sensor", "is_sensor");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_layer", PROPERTY_HINT_RANGE, "0,31,1"), "set_collision_layer", "get_collision_layer");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0,2,0.01"), "set_friction", "get_friction");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "restitution", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_restitution", "get_restitution");
    ADD_SIGNAL(MethodInfo("body_entered", PropertyInfo(Variant::OBJECT, "body")));
    ADD_SIGNAL(MethodInfo("body_exited", PropertyInfo(Variant::OBJECT, "body")));
}

void MossRigidBody3D::_notification(int what) {
    if (what == NOTIFICATION_READY) call_deferred("_moss_deferred_register");
    if (what == NOTIFICATION_EXIT_TREE) unregister_from_world();
}

void MossRigidBody3D::set_shape(const Ref<MossShape3D> &value) {
    const Callable callback(this, "_moss_shape_changed");
    if (m_shape.is_valid() && m_shape->is_connected("changed", callback))
        m_shape->disconnect("changed", callback);
    m_shape = value;
    if (m_shape.is_valid() && !m_shape->is_connected("changed", callback))
        m_shape->connect("changed", callback);
    rebuild_registered_body();
}
void MossRigidBody3D::set_body_mode(int value) { m_mode = static_cast<MossBodyMode>(CLAMP(value, 0, 2)); rebuild_registered_body(); }
void MossRigidBody3D::set_mass(float value) { m_mass = Math::max(0.001f, value); rebuild_registered_body(); }
void MossRigidBody3D::set_sensor(bool value) { m_sensor = value; rebuild_registered_body(); }
void MossRigidBody3D::set_collision_layer(int value) { m_collision_layer = CLAMP(value, 0, 31); rebuild_registered_body(); }
void MossRigidBody3D::set_friction(float value) { m_friction = Math::max(0.0f, value); rebuild_registered_body(); }
void MossRigidBody3D::set_restitution(float value) { m_restitution = CLAMP(value, 0.0f, 1.0f); rebuild_registered_body(); }
void MossRigidBody3D::rebuild_registered_body() {
    if (!is_inside_tree()) return;
    if (!m_world) { try_register_with_world(); return; }
    MossPhysicsWorld3D *world = m_world;
    if (m_registered) world->unregister_body(this);
    world->register_body(this);
}
void MossRigidBody3D::_moss_shape_changed() { rebuild_registered_body(); }

void MossRigidBody3D::try_register_with_world() {
    if (m_registered || !is_inside_tree()) return;
    Node *ancestor = get_parent();
    while (ancestor && !Object::cast_to<MossPhysicsWorld3D>(ancestor)) ancestor = ancestor->get_parent();
    m_world = Object::cast_to<MossPhysicsWorld3D>(ancestor);
    if (m_world) m_world->register_body(this);
}

void MossRigidBody3D::unregister_from_world() {
    if (m_world && m_registered) m_world->unregister_body(this);
    m_world = nullptr;
    m_registered = false;
}

void MossRigidBody3D::sync_from_physics() {
    if (!m_registered || m_mode != MossBodyMode::Dynamic || !m_world) return;
    ::RVec3 p; ::Quat q;
    if (m_world->m_core.get_transform(m_body_id, p, q)) {
        const Vector3 scale = get_global_transform().basis.get_scale();
        Transform3D t;
        t.origin = Vector3(static_cast<real_t>(p.GetX()), static_cast<real_t>(p.GetY()), static_cast<real_t>(p.GetZ()));
        t.basis = Basis(Quaternion(q.GetX(), q.GetY(), q.GetZ(), q.GetW())).scaled(scale);
        set_global_transform(t);
    }
}

void MossRigidBody3D::notify_contact(bool entered, Object *other) {
    emit_signal(entered ? "body_entered" : "body_exited", other);
}
void MossRigidBody3D::apply_impulse(const Vector3 &impulse) {
    if (m_registered && m_world) m_world->m_core.add_impulse(m_body_id, ::Vec3(impulse.x, impulse.y, impulse.z));
}
void MossRigidBody3D::set_linear_velocity(const Vector3 &velocity) {
    if (m_registered && m_world) m_world->m_core.set_linear_velocity(m_body_id, ::Vec3(velocity.x, velocity.y, velocity.z));
}
void MossRigidBody3D::move_kinematic(const Transform3D &target, float delta) {
    if (!m_registered || !m_world || m_mode != MossBodyMode::Kinematic) return;
    const Quaternion q = target.basis.get_rotation_quaternion();
    set_global_transform(target);
    m_world->m_core.set_kinematic_target(m_body_id, ::RVec3(target.origin.x, target.origin.y, target.origin.z), ::Quat(q.x, q.y, q.z, q.w), delta);
}

void MossRigidBody2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_shape", "shape"), &MossRigidBody2D::set_shape);
    ClassDB::bind_method(D_METHOD("get_shape"), &MossRigidBody2D::get_shape);
    ClassDB::bind_method(D_METHOD("set_body_mode", "mode"), &MossRigidBody2D::set_body_mode);
    ClassDB::bind_method(D_METHOD("get_body_mode"), &MossRigidBody2D::get_body_mode);
    ClassDB::bind_method(D_METHOD("set_mass", "mass"), &MossRigidBody2D::set_mass);
    ClassDB::bind_method(D_METHOD("get_mass"), &MossRigidBody2D::get_mass);
    ClassDB::bind_method(D_METHOD("set_sensor", "sensor"), &MossRigidBody2D::set_sensor);
    ClassDB::bind_method(D_METHOD("is_sensor"), &MossRigidBody2D::is_sensor);
    ClassDB::bind_method(D_METHOD("set_collision_layer", "layer"), &MossRigidBody2D::set_collision_layer);
    ClassDB::bind_method(D_METHOD("get_collision_layer"), &MossRigidBody2D::get_collision_layer);
    ClassDB::bind_method(D_METHOD("set_friction", "friction"), &MossRigidBody2D::set_friction);
    ClassDB::bind_method(D_METHOD("get_friction"), &MossRigidBody2D::get_friction);
    ClassDB::bind_method(D_METHOD("set_restitution", "restitution"), &MossRigidBody2D::set_restitution);
    ClassDB::bind_method(D_METHOD("get_restitution"), &MossRigidBody2D::get_restitution);
    ClassDB::bind_method(D_METHOD("apply_impulse", "impulse"), &MossRigidBody2D::apply_impulse);
    ClassDB::bind_method(D_METHOD("set_linear_velocity", "velocity"), &MossRigidBody2D::set_linear_velocity);
    ClassDB::bind_method(D_METHOD("move_kinematic", "target", "delta"), &MossRigidBody2D::move_kinematic);
    ClassDB::bind_method(D_METHOD("is_moss_active"), &MossRigidBody2D::is_moss_active);
    ClassDB::bind_method(D_METHOD("_moss_deferred_register"), &MossRigidBody2D::try_register_with_world);
    ClassDB::bind_method(D_METHOD("_moss_shape_changed"), &MossRigidBody2D::_moss_shape_changed);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "MossShape2D"), "set_shape", "get_shape");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "body_mode", PROPERTY_HINT_ENUM, "Static,Kinematic,Dynamic"), "set_body_mode", "get_body_mode");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mass", PROPERTY_HINT_RANGE, "0.001,100000,0.01,or_greater"), "set_mass", "get_mass");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sensor"), "set_sensor", "is_sensor");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_layer", PROPERTY_HINT_RANGE, "0,31,1"), "set_collision_layer", "get_collision_layer");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0,2,0.01"), "set_friction", "get_friction");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "restitution", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_restitution", "get_restitution");
    ADD_SIGNAL(MethodInfo("body_entered", PropertyInfo(Variant::OBJECT, "body")));
    ADD_SIGNAL(MethodInfo("body_exited", PropertyInfo(Variant::OBJECT, "body")));
}

void MossRigidBody2D::_notification(int what) {
    if (what == NOTIFICATION_READY) call_deferred("_moss_deferred_register");
    if (what == NOTIFICATION_EXIT_TREE) unregister_from_world();
}
void MossRigidBody2D::set_shape(const Ref<MossShape2D> &value) {
    const Callable callback(this, "_moss_shape_changed");
    if (m_shape.is_valid() && m_shape->is_connected("changed", callback))
        m_shape->disconnect("changed", callback);
    m_shape = value;
    if (m_shape.is_valid() && !m_shape->is_connected("changed", callback))
        m_shape->connect("changed", callback);
    rebuild_registered_body();
}
void MossRigidBody2D::set_body_mode(int value) { m_mode = static_cast<MossBodyMode>(CLAMP(value, 0, 2)); rebuild_registered_body(); }
void MossRigidBody2D::set_mass(float value) { m_mass = Math::max(0.001f, value); rebuild_registered_body(); }
void MossRigidBody2D::set_sensor(bool value) { m_sensor = value; rebuild_registered_body(); }
void MossRigidBody2D::set_collision_layer(int value) { m_collision_layer = CLAMP(value, 0, 31); rebuild_registered_body(); }
void MossRigidBody2D::set_friction(float value) { m_friction = Math::max(0.0f, value); rebuild_registered_body(); }
void MossRigidBody2D::set_restitution(float value) { m_restitution = CLAMP(value, 0.0f, 1.0f); rebuild_registered_body(); }
void MossRigidBody2D::rebuild_registered_body() {
    if (!is_inside_tree()) return;
    if (!m_world) { try_register_with_world(); return; }
    MossPhysicsWorld2D *world = m_world;
    if (m_registered) world->unregister_body(this);
    world->register_body(this);
}
void MossRigidBody2D::_moss_shape_changed() { rebuild_registered_body(); }
void MossRigidBody2D::try_register_with_world() {
    if (m_registered || !is_inside_tree()) return;
    Node *ancestor = get_parent();
    while (ancestor && !Object::cast_to<MossPhysicsWorld2D>(ancestor)) ancestor = ancestor->get_parent();
    m_world = Object::cast_to<MossPhysicsWorld2D>(ancestor);
    if (m_world) m_world->register_body(this);
}
void MossRigidBody2D::unregister_from_world() {
    if (m_world && m_registered) m_world->unregister_body(this);
    m_world = nullptr;
    m_registered = false;
}
void MossRigidBody2D::sync_from_physics() {
    if (!m_registered || m_mode != MossBodyMode::Dynamic || !m_world) return;
    ::RVec3 p; ::Quat q;
    if (m_world->m_core.get_transform(m_body_id, p, q)) {
        set_global_position(Vector2(static_cast<real_t>(p.GetX()), static_cast<real_t>(p.GetY())));
        set_global_rotation(static_cast<real_t>(2.0 * std::atan2(q.GetZ(), q.GetW())));
    }
}
void MossRigidBody2D::notify_contact(bool entered, Object *other) {
    emit_signal(entered ? "body_entered" : "body_exited", other);
}
void MossRigidBody2D::apply_impulse(const Vector2 &impulse) {
    if (m_registered && m_world) m_world->m_core.add_impulse(m_body_id, ::Vec3(impulse.x, impulse.y, 0));
}
void MossRigidBody2D::set_linear_velocity(const Vector2 &velocity) {
    if (m_registered && m_world) m_world->m_core.set_linear_velocity(m_body_id, ::Vec3(velocity.x, velocity.y, 0));
}
void MossRigidBody2D::move_kinematic(const Transform2D &target, float delta) {
    if (!m_registered || !m_world || m_mode != MossBodyMode::Kinematic) return;
    const float half = static_cast<float>(target.get_rotation()) * 0.5f;
    set_global_transform(target);
    m_world->m_core.set_kinematic_target(m_body_id, ::RVec3(target.get_origin().x, target.get_origin().y, 0), ::Quat(0, 0, std::sin(half), std::cos(half)), delta);
}
