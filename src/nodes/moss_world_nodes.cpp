#include "moss_world_nodes.h"
#include "moss_body_nodes.h"
#include "moss_joint_nodes.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>
#include <cmath>

using namespace godot;
using namespace mossgodot;

static ::Ref<::Shape> apply_node_scale(const ::Ref<::Shape> &shape, const Vector3 &scale) {
    if (shape.GetPtr() == nullptr) return {};
    const Vector3 safe_scale(std::abs(scale.x), std::abs(scale.y), std::abs(scale.z));
    if (safe_scale.is_equal_approx(Vector3(1, 1, 1))) return shape;
    const auto result = shape->ScaleShape(::Vec3(safe_scale.x, safe_scale.y, safe_scale.z));
    if (result.HasError()) {
        UtilityFunctions::push_error("MossGodot: node scale is not valid for its selected Moss collision shape.");
        return {};
    }
    return result.Get();
}

static ::EMotionType to_moss_motion(MossBodyMode mode) {
    switch (mode) {
        case MossBodyMode::Static: return ::EMotionType::Static;
        case MossBodyMode::Kinematic: return ::EMotionType::Kinematic;
        default: return ::EMotionType::Dynamic;
    }
}

static Dictionary query_hit_to_dictionary(const QueryHit &hit, bool two_d) {
    Dictionary value;
    value["collider_id"] = static_cast<int64_t>(hit.object_id);
    value["body_token"] = static_cast<int64_t>(hit.body_token);
    value["fraction"] = hit.fraction;
    value["distance"] = hit.distance;
    value["penetration_depth"] = hit.penetration_depth;
    value["position"] = two_d ? Variant(Vector2(hit.position.GetX(), hit.position.GetY()))
                              : Variant(Vector3(hit.position.GetX(), hit.position.GetY(), hit.position.GetZ()));
    value["normal"] = two_d ? Variant(Vector2(hit.normal.GetX(), hit.normal.GetY()))
                            : Variant(Vector3(hit.normal.GetX(), hit.normal.GetY(), hit.normal.GetZ()));
    return value;
}

static Array query_hits_to_array(const std::vector<QueryHit> &hits, bool two_d) {
    Array result;
    for (const QueryHit &hit : hits) result.push_back(query_hit_to_dictionary(hit, two_d));
    return result;
}

static void invalidate_joint_nodes(Node *node, Object *body, bool two_d) {
    if (!node) return;
    for (int64_t i = 0; i < node->get_child_count(); ++i) {
        Node *child = node->get_child(i);
        if (two_d) {
            if (Object::cast_to<MossPhysicsWorld2D>(child) || Object::cast_to<MossPhysicsWorld3D>(child)) continue;
            if (auto *joint = Object::cast_to<MossJoint2D>(child)) joint->on_body_invalidated(body);
        } else {
            if (Object::cast_to<MossPhysicsWorld2D>(child) || Object::cast_to<MossPhysicsWorld3D>(child)) continue;
            if (auto *joint = Object::cast_to<MossJoint3D>(child)) joint->on_body_invalidated(body);
        }
        invalidate_joint_nodes(child, body, two_d);
    }
}

void MossPhysicsWorld3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &MossPhysicsWorld3D::set_gravity);
    ClassDB::bind_method(D_METHOD("get_gravity"), &MossPhysicsWorld3D::get_gravity);
    ClassDB::bind_method(D_METHOD("set_fixed_fps", "fps"), &MossPhysicsWorld3D::set_fixed_fps);
    ClassDB::bind_method(D_METHOD("get_fixed_fps"), &MossPhysicsWorld3D::get_fixed_fps);
    ClassDB::bind_method(D_METHOD("set_max_bodies", "count"), &MossPhysicsWorld3D::set_max_bodies);
    ClassDB::bind_method(D_METHOD("get_max_bodies"), &MossPhysicsWorld3D::get_max_bodies);
    ClassDB::bind_method(D_METHOD("set_worker_threads", "count"), &MossPhysicsWorld3D::set_worker_threads);
    ClassDB::bind_method(D_METHOD("get_worker_threads"), &MossPhysicsWorld3D::get_worker_threads);
    ClassDB::bind_method(D_METHOD("set_max_substeps", "count"), &MossPhysicsWorld3D::set_max_substeps);
    ClassDB::bind_method(D_METHOD("get_max_substeps"), &MossPhysicsWorld3D::get_max_substeps);
    ClassDB::bind_method(D_METHOD("set_layer_mask", "layer", "mask"), &MossPhysicsWorld3D::set_layer_mask);
    ClassDB::bind_method(D_METHOD("get_layer_mask", "layer"), &MossPhysicsWorld3D::get_layer_mask);
    ClassDB::bind_method(D_METHOD("is_backend_active"), &MossPhysicsWorld3D::is_backend_active);
    ClassDB::bind_method(D_METHOD("ray_cast", "from", "to", "query_mask"), &MossPhysicsWorld3D::ray_cast, DEFVAL(-1));
    ClassDB::bind_method(D_METHOD("ray_cast_all", "from", "to", "query_mask", "max_results"), &MossPhysicsWorld3D::ray_cast_all, DEFVAL(-1), DEFVAL(64));
    ClassDB::bind_method(D_METHOD("intersect_point", "point", "query_mask", "max_results"), &MossPhysicsWorld3D::intersect_point, DEFVAL(-1), DEFVAL(64));
    ClassDB::bind_method(D_METHOD("intersect_shape", "shape", "transform", "query_mask", "max_results"), &MossPhysicsWorld3D::intersect_shape, DEFVAL(-1), DEFVAL(64));
    ClassDB::bind_method(D_METHOD("cast_shape", "shape", "transform", "motion", "query_mask", "max_results"), &MossPhysicsWorld3D::cast_shape, DEFVAL(-1), DEFVAL(64));
    ClassDB::bind_method(D_METHOD("add_fixed_joint", "body_a", "body_b"), &MossPhysicsWorld3D::add_fixed_joint);
    ClassDB::bind_method(D_METHOD("add_distance_joint", "body_a", "body_b", "min_distance", "max_distance"), &MossPhysicsWorld3D::add_distance_joint);
    ClassDB::bind_method(D_METHOD("remove_joint", "joint_id"), &MossPhysicsWorld3D::remove_joint);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "gravity"), "set_gravity", "get_gravity");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "fixed_fps", PROPERTY_HINT_RANGE, "1,240,1"), "set_fixed_fps", "get_fixed_fps");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_bodies", PROPERTY_HINT_RANGE, "64,65536,64"), "set_max_bodies", "get_max_bodies");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "worker_threads", PROPERTY_HINT_RANGE, "0,32,1"), "set_worker_threads", "get_worker_threads");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_substeps", PROPERTY_HINT_RANGE, "1,16,1"), "set_max_substeps", "get_max_substeps");
    ADD_SIGNAL(MethodInfo("body_contact_started", PropertyInfo(Variant::OBJECT, "body_a"), PropertyInfo(Variant::OBJECT, "body_b")));
    ADD_SIGNAL(MethodInfo("body_contact_ended", PropertyInfo(Variant::OBJECT, "body_a"), PropertyInfo(Variant::OBJECT, "body_b")));
}

Dictionary MossPhysicsWorld3D::ray_cast(const Vector3 &from, const Vector3 &to, int query_mask) {
    QueryHit hit;
    const Vector3 motion = to - from;
    if (!m_core.ray_cast(::RVec3(from.x, from.y, from.z), ::Vec3(motion.x, motion.y, motion.z), static_cast<uint32_t>(query_mask), hit)) return {};
    return query_hit_to_dictionary(hit, false);
}
Array MossPhysicsWorld3D::ray_cast_all(const Vector3 &from, const Vector3 &to, int query_mask, int max_results) {
    const Vector3 motion = to - from;
    return query_hits_to_array(m_core.ray_cast_all(::RVec3(from.x, from.y, from.z), ::Vec3(motion.x, motion.y, motion.z), static_cast<uint32_t>(query_mask), static_cast<uint32_t>(CLAMP(max_results, 1, 4096))), false);
}
Array MossPhysicsWorld3D::intersect_point(const Vector3 &point, int query_mask, int max_results) {
    return query_hits_to_array(m_core.intersect_point(::RVec3(point.x, point.y, point.z), static_cast<uint32_t>(query_mask), static_cast<uint32_t>(CLAMP(max_results, 1, 4096))), false);
}
Array MossPhysicsWorld3D::intersect_shape(const Ref<MossShape3D> &shape, const Transform3D &transform, int query_mask, int max_results) {
    if (shape.is_null()) return {};
    const Ref<::Shape> moss_shape = apply_node_scale(shape->create_moss_shape(), transform.basis.get_scale());
    if (moss_shape.GetPtr() == nullptr) return {};
    const Quaternion q = transform.basis.get_rotation_quaternion();
    return query_hits_to_array(m_core.intersect_shape(moss_shape, ::RVec3(transform.origin.x, transform.origin.y, transform.origin.z),
        ::Quat(q.x, q.y, q.z, q.w), ::Vec3(1, 1, 1), static_cast<uint32_t>(query_mask), static_cast<uint32_t>(CLAMP(max_results, 1, 4096))), false);
}
Array MossPhysicsWorld3D::cast_shape(const Ref<MossShape3D> &shape, const Transform3D &transform, const Vector3 &motion, int query_mask, int max_results) {
    if (shape.is_null()) return {};
    const Ref<::Shape> moss_shape = apply_node_scale(shape->create_moss_shape(), transform.basis.get_scale());
    if (moss_shape.GetPtr() == nullptr) return {};
    const Quaternion q = transform.basis.get_rotation_quaternion();
    return query_hits_to_array(m_core.cast_shape(moss_shape, ::RVec3(transform.origin.x, transform.origin.y, transform.origin.z),
        ::Quat(q.x, q.y, q.z, q.w), ::Vec3(1, 1, 1), ::Vec3(motion.x, motion.y, motion.z),
        static_cast<uint32_t>(query_mask), static_cast<uint32_t>(CLAMP(max_results, 1, 4096))), false);
}
uint64_t MossPhysicsWorld3D::add_fixed_joint(MossRigidBody3D *a, MossRigidBody3D *b) {
    if (!a || !b || !a->m_registered || !b->m_registered || a->m_world != this || b->m_world != this) return 0;
    return m_core.create_fixed_constraint(a->m_body_id, b->m_body_id);
}
uint64_t MossPhysicsWorld3D::add_distance_joint(MossRigidBody3D *a, MossRigidBody3D *b, float min_distance, float max_distance) {
    if (!a || !b || !a->m_registered || !b->m_registered || a->m_world != this || b->m_world != this) return 0;
    return m_core.create_distance_constraint(a->m_body_id, b->m_body_id, min_distance, max_distance);
}
void MossPhysicsWorld3D::remove_joint(uint64_t joint_id) { m_core.remove_constraint(joint_id); }

void MossPhysicsWorld3D::set_gravity(const Vector3 &value) {
    m_gravity = value;
    m_core.set_gravity(::Vec3(value.x, value.y, value.z));
}
void MossPhysicsWorld3D::set_fixed_fps(int value) { m_fixed_fps = CLAMP(value, 1, 240); }
void MossPhysicsWorld3D::set_max_bodies(int value) { m_max_bodies = CLAMP(value, 64, 65536); }
void MossPhysicsWorld3D::set_worker_threads(int value) { m_worker_threads = CLAMP(value, 0, 32); }
void MossPhysicsWorld3D::set_max_substeps(int value) { m_max_substeps = CLAMP(value, 1, 16); }
void MossPhysicsWorld3D::set_layer_mask(int layer, int mask) { m_core.set_layer_mask(static_cast<uint32_t>(layer), static_cast<uint32_t>(mask)); }
int MossPhysicsWorld3D::get_layer_mask(int layer) const { return static_cast<int>(m_core.get_layer_mask(static_cast<uint32_t>(layer))); }
void MossPhysicsWorld3D::_ready() {
    if (!m_core.initialize(false, ::Vec3(m_gravity.x, m_gravity.y, m_gravity.z),
                           static_cast<uint32_t>(m_max_bodies), static_cast<uint32_t>(m_worker_threads))) {
        UtilityFunctions::push_error("MossGodot: failed to initialise 3D Moss physics world");
        return;
    }
    const auto register_children = [this](auto &&self, Node *node) -> void {
        for (int64_t i = 0; i < node->get_child_count(); ++i) {
            Node *child = node->get_child(i);
            if (Object::cast_to<MossPhysicsWorld3D>(child) || Object::cast_to<MossPhysicsWorld2D>(child)) continue;
            if (auto *body = Object::cast_to<MossRigidBody3D>(child)) register_body(body);
            self(self, child);
        }
    };
    register_children(register_children, this);
    set_physics_process(true);
}
void MossPhysicsWorld3D::register_body(MossRigidBody3D *body) {
    if (!body || body->m_registered || !m_core.is_initialized() || body->m_shape.is_null()) return;
    const Transform3D t = body->get_global_transform();
    const ::Ref<::Shape> raw_shape = body->m_shape->create_moss_shape();
    if (raw_shape.GetPtr() && raw_shape->MustBeStatic() && body->m_mode != MossBodyMode::Static) {
        UtilityFunctions::push_error("MossGodot: mesh and heightfield colliders require a Static MossRigidBody3D.");
        return;
    }
    const ::Ref<::Shape> shape = apply_node_scale(raw_shape, t.basis.get_scale());
    if (shape.is_null()) return;
    const Quaternion q = t.basis.get_rotation_quaternion();
    const uint64_t object_id = body->get_instance_id();
    body->m_body_id = m_core.create_body(shape, ::RVec3(t.origin.x, t.origin.y, t.origin.z),
            ::Quat(q.x, q.y, q.z, q.w), to_moss_motion(body->m_mode),
            static_cast<::ObjectLayer>(body->m_collision_layer), object_id, body->m_sensor,
            body->m_mass, body->m_friction, body->m_restitution);
    if (body->m_body_id.IsInvalid()) return;
    body->m_world = this;
    body->m_registered = true;
    m_bodies[object_id] = body;
}
void MossPhysicsWorld3D::unregister_body(MossRigidBody3D *body) {
    if (!body || !body->m_registered) return;
    invalidate_joint_nodes(this, body, false);
    m_core.destroy_body(body->m_body_id);
    m_bodies.erase(body->get_instance_id());
    body->m_registered = false;
    body->m_body_id = ::BodyID();
}
void MossPhysicsWorld3D::dispatch_contact_events() {
    for (const ContactEvent &event : m_core.take_contact_events()) {
        const auto a = m_bodies.find(event.body_a);
        const auto b = m_bodies.find(event.body_b);
        if (a == m_bodies.end() || b == m_bodies.end()) continue;
        const bool entered = event.type == ContactEvent::Type::Enter;
        a->second->notify_contact(entered, b->second);
        b->second->notify_contact(entered, a->second);
        emit_signal(entered ? "body_contact_started" : "body_contact_ended", a->second, b->second);
    }
}
void MossPhysicsWorld3D::_physics_process(double delta) {
    if (!m_core.is_initialized()) return;
    const double fixed = 1.0 / static_cast<double>(m_fixed_fps);
    m_accumulator = std::min(m_accumulator + delta, fixed * static_cast<double>(m_max_substeps + 1));
    int steps = 0;
    while (m_accumulator >= fixed && steps < m_max_substeps) {
        for (auto &entry : m_bodies) {
            MossRigidBody3D *body = entry.second;
            if (body->m_mode == MossBodyMode::Dynamic) continue;
            const Transform3D t = body->get_global_transform();
            const Quaternion q = t.basis.get_rotation_quaternion();
            if (body->m_mode == MossBodyMode::Kinematic)
                m_core.set_kinematic_target(body->m_body_id, ::RVec3(t.origin.x, t.origin.y, t.origin.z), ::Quat(q.x, q.y, q.z, q.w), static_cast<float>(fixed));
            else
                m_core.set_transform(body->m_body_id, ::RVec3(t.origin.x, t.origin.y, t.origin.z), ::Quat(q.x, q.y, q.z, q.w));
        }
        m_core.step(static_cast<float>(fixed), 1);
        for (auto &entry : m_bodies) entry.second->sync_from_physics();
        m_accumulator -= fixed;
        ++steps;
    }
    dispatch_contact_events();
}
void MossPhysicsWorld3D::_exit_tree() {
    for (auto &entry : m_bodies) {
        entry.second->m_registered = false;
        entry.second->m_world = nullptr;
        entry.second->m_body_id = ::BodyID();
    }
    m_bodies.clear();
    m_core.shutdown();
}

void MossPhysicsWorld2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_gravity", "gravity"), &MossPhysicsWorld2D::set_gravity);
    ClassDB::bind_method(D_METHOD("get_gravity"), &MossPhysicsWorld2D::get_gravity);
    ClassDB::bind_method(D_METHOD("set_fixed_fps", "fps"), &MossPhysicsWorld2D::set_fixed_fps);
    ClassDB::bind_method(D_METHOD("get_fixed_fps"), &MossPhysicsWorld2D::get_fixed_fps);
    ClassDB::bind_method(D_METHOD("set_max_bodies", "count"), &MossPhysicsWorld2D::set_max_bodies);
    ClassDB::bind_method(D_METHOD("get_max_bodies"), &MossPhysicsWorld2D::get_max_bodies);
    ClassDB::bind_method(D_METHOD("set_worker_threads", "count"), &MossPhysicsWorld2D::set_worker_threads);
    ClassDB::bind_method(D_METHOD("get_worker_threads"), &MossPhysicsWorld2D::get_worker_threads);
    ClassDB::bind_method(D_METHOD("set_max_substeps", "count"), &MossPhysicsWorld2D::set_max_substeps);
    ClassDB::bind_method(D_METHOD("get_max_substeps"), &MossPhysicsWorld2D::get_max_substeps);
    ClassDB::bind_method(D_METHOD("set_layer_mask", "layer", "mask"), &MossPhysicsWorld2D::set_layer_mask);
    ClassDB::bind_method(D_METHOD("get_layer_mask", "layer"), &MossPhysicsWorld2D::get_layer_mask);
    ClassDB::bind_method(D_METHOD("is_backend_active"), &MossPhysicsWorld2D::is_backend_active);
    ClassDB::bind_method(D_METHOD("ray_cast", "from", "to", "query_mask"), &MossPhysicsWorld2D::ray_cast, DEFVAL(-1));
    ClassDB::bind_method(D_METHOD("ray_cast_all", "from", "to", "query_mask", "max_results"), &MossPhysicsWorld2D::ray_cast_all, DEFVAL(-1), DEFVAL(64));
    ClassDB::bind_method(D_METHOD("intersect_point", "point", "query_mask", "max_results"), &MossPhysicsWorld2D::intersect_point, DEFVAL(-1), DEFVAL(64));
    ClassDB::bind_method(D_METHOD("intersect_shape", "shape", "transform", "query_mask", "max_results"), &MossPhysicsWorld2D::intersect_shape, DEFVAL(-1), DEFVAL(64));
    ClassDB::bind_method(D_METHOD("cast_shape", "shape", "transform", "motion", "query_mask", "max_results"), &MossPhysicsWorld2D::cast_shape, DEFVAL(-1), DEFVAL(64));
    ClassDB::bind_method(D_METHOD("add_fixed_joint", "body_a", "body_b"), &MossPhysicsWorld2D::add_fixed_joint);
    ClassDB::bind_method(D_METHOD("add_distance_joint", "body_a", "body_b", "min_distance", "max_distance"), &MossPhysicsWorld2D::add_distance_joint);
    ClassDB::bind_method(D_METHOD("remove_joint", "joint_id"), &MossPhysicsWorld2D::remove_joint);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "gravity"), "set_gravity", "get_gravity");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "fixed_fps", PROPERTY_HINT_RANGE, "1,240,1"), "set_fixed_fps", "get_fixed_fps");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_bodies", PROPERTY_HINT_RANGE, "64,65536,64"), "set_max_bodies", "get_max_bodies");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "worker_threads", PROPERTY_HINT_RANGE, "0,32,1"), "set_worker_threads", "get_worker_threads");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_substeps", PROPERTY_HINT_RANGE, "1,16,1"), "set_max_substeps", "get_max_substeps");
    ADD_SIGNAL(MethodInfo("body_contact_started", PropertyInfo(Variant::OBJECT, "body_a"), PropertyInfo(Variant::OBJECT, "body_b")));
    ADD_SIGNAL(MethodInfo("body_contact_ended", PropertyInfo(Variant::OBJECT, "body_a"), PropertyInfo(Variant::OBJECT, "body_b")));
}
Dictionary MossPhysicsWorld2D::ray_cast(const Vector2 &from, const Vector2 &to, int query_mask) {
    QueryHit hit;
    const Vector2 motion = to - from;
    if (!m_core.ray_cast(::RVec3(from.x, from.y, 0), ::Vec3(motion.x, motion.y, 0), static_cast<uint32_t>(query_mask), hit)) return {};
    return query_hit_to_dictionary(hit, true);
}
Array MossPhysicsWorld2D::ray_cast_all(const Vector2 &from, const Vector2 &to, int query_mask, int max_results) {
    const Vector2 motion = to - from;
    return query_hits_to_array(m_core.ray_cast_all(::RVec3(from.x, from.y, 0), ::Vec3(motion.x, motion.y, 0), static_cast<uint32_t>(query_mask), static_cast<uint32_t>(CLAMP(max_results, 1, 4096))), true);
}
Array MossPhysicsWorld2D::intersect_point(const Vector2 &point, int query_mask, int max_results) {
    return query_hits_to_array(m_core.intersect_point(::RVec3(point.x, point.y, 0), static_cast<uint32_t>(query_mask), static_cast<uint32_t>(CLAMP(max_results, 1, 4096))), true);
}
Array MossPhysicsWorld2D::intersect_shape(const Ref<MossShape2D> &shape, const Transform2D &transform, int query_mask, int max_results) {
    if (shape.is_null()) return {};
    const Vector2 scale = transform.get_scale();
    const Ref<::Shape> moss_shape = apply_node_scale(shape->create_moss_shape(), Vector3(scale.x, scale.y, 1));
    if (moss_shape.GetPtr() == nullptr) return {};
    const float half = static_cast<float>(transform.get_rotation()) * 0.5f;
    return query_hits_to_array(m_core.intersect_shape(moss_shape, ::RVec3(transform.get_origin().x, transform.get_origin().y, 0),
        ::Quat(0, 0, std::sin(half), std::cos(half)), ::Vec3(1, 1, 1), static_cast<uint32_t>(query_mask), static_cast<uint32_t>(CLAMP(max_results, 1, 4096))), true);
}
Array MossPhysicsWorld2D::cast_shape(const Ref<MossShape2D> &shape, const Transform2D &transform, const Vector2 &motion, int query_mask, int max_results) {
    if (shape.is_null()) return {};
    const Vector2 scale = transform.get_scale();
    const Ref<::Shape> moss_shape = apply_node_scale(shape->create_moss_shape(), Vector3(scale.x, scale.y, 1));
    if (moss_shape.GetPtr() == nullptr) return {};
    const float half = static_cast<float>(transform.get_rotation()) * 0.5f;
    return query_hits_to_array(m_core.cast_shape(moss_shape, ::RVec3(transform.get_origin().x, transform.get_origin().y, 0),
        ::Quat(0, 0, std::sin(half), std::cos(half)), ::Vec3(1, 1, 1), ::Vec3(motion.x, motion.y, 0),
        static_cast<uint32_t>(query_mask), static_cast<uint32_t>(CLAMP(max_results, 1, 4096))), true);
}
uint64_t MossPhysicsWorld2D::add_fixed_joint(MossRigidBody2D *a, MossRigidBody2D *b) {
    if (!a || !b || !a->m_registered || !b->m_registered || a->m_world != this || b->m_world != this) return 0;
    return m_core.create_fixed_constraint(a->m_body_id, b->m_body_id);
}
uint64_t MossPhysicsWorld2D::add_distance_joint(MossRigidBody2D *a, MossRigidBody2D *b, float min_distance, float max_distance) {
    if (!a || !b || !a->m_registered || !b->m_registered || a->m_world != this || b->m_world != this) return 0;
    return m_core.create_distance_constraint(a->m_body_id, b->m_body_id, min_distance, max_distance);
}
void MossPhysicsWorld2D::remove_joint(uint64_t joint_id) { m_core.remove_constraint(joint_id); }

void MossPhysicsWorld2D::set_gravity(const Vector2 &value) {
    m_gravity = value;
    m_core.set_gravity(::Vec3(value.x, value.y, 0));
}
void MossPhysicsWorld2D::set_fixed_fps(int value) { m_fixed_fps = CLAMP(value, 1, 240); }
void MossPhysicsWorld2D::set_max_bodies(int value) { m_max_bodies = CLAMP(value, 64, 65536); }
void MossPhysicsWorld2D::set_worker_threads(int value) { m_worker_threads = CLAMP(value, 0, 32); }
void MossPhysicsWorld2D::set_max_substeps(int value) { m_max_substeps = CLAMP(value, 1, 16); }
void MossPhysicsWorld2D::set_layer_mask(int layer, int mask) { m_core.set_layer_mask(static_cast<uint32_t>(layer), static_cast<uint32_t>(mask)); }
int MossPhysicsWorld2D::get_layer_mask(int layer) const { return static_cast<int>(m_core.get_layer_mask(static_cast<uint32_t>(layer))); }
void MossPhysicsWorld2D::_ready() {
    if (!m_core.initialize(true, ::Vec3(m_gravity.x, m_gravity.y, 0),
                           static_cast<uint32_t>(m_max_bodies), static_cast<uint32_t>(m_worker_threads))) {
        UtilityFunctions::push_error("MossGodot: failed to initialise 2D Moss physics world");
        return;
    }
    const auto register_children = [this](auto &&self, Node *node) -> void {
        for (int64_t i = 0; i < node->get_child_count(); ++i) {
            Node *child = node->get_child(i);
            if (Object::cast_to<MossPhysicsWorld2D>(child) || Object::cast_to<MossPhysicsWorld3D>(child)) continue;
            if (auto *body = Object::cast_to<MossRigidBody2D>(child)) register_body(body);
            self(self, child);
        }
    };
    register_children(register_children, this);
    set_physics_process(true);
}
void MossPhysicsWorld2D::register_body(MossRigidBody2D *body) {
    if (!body || body->m_registered || !m_core.is_initialized() || body->m_shape.is_null()) return;
    const Transform2D transform = body->get_global_transform();
    const ::Ref<::Shape> raw_shape = body->m_shape->create_moss_shape();
    if (raw_shape.GetPtr() && raw_shape->MustBeStatic() && body->m_mode != MossBodyMode::Static) {
        UtilityFunctions::push_error("MossGodot: this collider requires a Static MossRigidBody2D.");
        return;
    }
    const Vector2 scale = transform.get_scale();
    const ::Ref<::Shape> shape = apply_node_scale(raw_shape, Vector3(scale.x, scale.y, 1.0f));
    if (shape.is_null()) return;
    const Vector2 p = transform.get_origin();
    const float half = static_cast<float>(body->get_global_rotation()) * 0.5f;
    const uint64_t object_id = body->get_instance_id();
    body->m_body_id = m_core.create_body(shape, ::RVec3(p.x, p.y, 0),
            ::Quat(0, 0, std::sin(half), std::cos(half)), to_moss_motion(body->m_mode),
            static_cast<::ObjectLayer>(body->m_collision_layer), object_id, body->m_sensor,
            body->m_mass, body->m_friction, body->m_restitution);
    if (body->m_body_id.IsInvalid()) return;
    body->m_world = this;
    body->m_registered = true;
    m_bodies[object_id] = body;
}
void MossPhysicsWorld2D::unregister_body(MossRigidBody2D *body) {
    if (!body || !body->m_registered) return;
    invalidate_joint_nodes(this, body, true);
    m_core.destroy_body(body->m_body_id);
    m_bodies.erase(body->get_instance_id());
    body->m_registered = false;
    body->m_body_id = ::BodyID();
}
void MossPhysicsWorld2D::dispatch_contact_events() {
    for (const ContactEvent &event : m_core.take_contact_events()) {
        const auto a = m_bodies.find(event.body_a);
        const auto b = m_bodies.find(event.body_b);
        if (a == m_bodies.end() || b == m_bodies.end()) continue;
        const bool entered = event.type == ContactEvent::Type::Enter;
        a->second->notify_contact(entered, b->second);
        b->second->notify_contact(entered, a->second);
        emit_signal(entered ? "body_contact_started" : "body_contact_ended", a->second, b->second);
    }
}
void MossPhysicsWorld2D::_physics_process(double delta) {
    if (!m_core.is_initialized()) return;
    const double fixed = 1.0 / static_cast<double>(m_fixed_fps);
    m_accumulator = std::min(m_accumulator + delta, fixed * static_cast<double>(m_max_substeps + 1));
    int steps = 0;
    while (m_accumulator >= fixed && steps < m_max_substeps) {
        for (auto &entry : m_bodies) {
            MossRigidBody2D *body = entry.second;
            if (body->m_mode == MossBodyMode::Dynamic) continue;
            const Vector2 p = body->get_global_position();
            const float half = static_cast<float>(body->get_global_rotation()) * 0.5f;
            const ::RVec3 mp(p.x, p.y, 0);
            const ::Quat mq(0, 0, std::sin(half), std::cos(half));
            if (body->m_mode == MossBodyMode::Kinematic)
                m_core.set_kinematic_target(body->m_body_id, mp, mq, static_cast<float>(fixed));
            else
                m_core.set_transform(body->m_body_id, mp, mq);
        }
        m_core.step(static_cast<float>(fixed), 1);
        for (auto &entry : m_bodies) entry.second->sync_from_physics();
        m_accumulator -= fixed;
        ++steps;
    }
    dispatch_contact_events();
}
void MossPhysicsWorld2D::_exit_tree() {
    for (auto &entry : m_bodies) {
        entry.second->m_registered = false;
        entry.second->m_world = nullptr;
        entry.second->m_body_id = ::BodyID();
    }
    m_bodies.clear();
    m_core.shutdown();
}
