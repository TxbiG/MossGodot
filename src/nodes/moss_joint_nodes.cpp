#include "moss_joint_nodes.h"
#include "moss_body_nodes.h"
#include "moss_world_nodes.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;
using namespace mossgodot;

void MossJoint3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_body_a", "path"), &MossJoint3D::set_body_a);
    ClassDB::bind_method(D_METHOD("get_body_a"), &MossJoint3D::get_body_a);
    ClassDB::bind_method(D_METHOD("set_body_b", "path"), &MossJoint3D::set_body_b);
    ClassDB::bind_method(D_METHOD("get_body_b"), &MossJoint3D::get_body_b);
    ClassDB::bind_method(D_METHOD("set_min_distance", "distance"), &MossJoint3D::set_min_distance);
    ClassDB::bind_method(D_METHOD("get_min_distance"), &MossJoint3D::get_min_distance);
    ClassDB::bind_method(D_METHOD("set_max_distance", "distance"), &MossJoint3D::set_max_distance);
    ClassDB::bind_method(D_METHOD("get_max_distance"), &MossJoint3D::get_max_distance);
    ClassDB::bind_method(D_METHOD("is_moss_active"), &MossJoint3D::is_moss_active);
    ClassDB::bind_method(D_METHOD("_moss_deferred_register"), &MossJoint3D::_moss_deferred_register);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "body_a"), "set_body_a", "get_body_a");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "body_b"), "set_body_b", "get_body_b");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_distance", PROPERTY_HINT_RANGE, "0,10000,0.01"), "set_min_distance", "get_min_distance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance", PROPERTY_HINT_RANGE, "0,10000,0.01"), "set_max_distance", "get_max_distance");
}

void MossJoint3D::set_body_a(const NodePath &value) { m_body_a = value; if (is_inside_tree()) { unregister_joint(); call_deferred("_moss_deferred_register"); } }
void MossJoint3D::set_body_b(const NodePath &value) { m_body_b = value; if (is_inside_tree()) { unregister_joint(); call_deferred("_moss_deferred_register"); } }
void MossJoint3D::set_min_distance(float value) { m_min_distance = Math::max(0.0f, value); if (is_inside_tree()) { unregister_joint(); call_deferred("_moss_deferred_register"); } }
void MossJoint3D::set_max_distance(float value) { m_max_distance = Math::max(m_min_distance, value); if (is_inside_tree()) { unregister_joint(); call_deferred("_moss_deferred_register"); } }
void MossJoint3D::_notification(int what) {
    if (what == NOTIFICATION_READY) call_deferred("_moss_deferred_register");
    if (what == NOTIFICATION_EXIT_TREE) unregister_joint();
}
void MossJoint3D::_moss_deferred_register() {
    if (!is_inside_tree() || m_joint_id != 0) return;
    MossRigidBody3D *a = Object::cast_to<MossRigidBody3D>(get_node_or_null(m_body_a));
    MossRigidBody3D *b = Object::cast_to<MossRigidBody3D>(get_node_or_null(m_body_b));
    if (!a || !b) {
        UtilityFunctions::push_warning("MossGodot: joint requires valid body_a and body_b MossRigidBody3D node paths.");
        return;
    }
    Node *ancestor = get_parent();
    while (ancestor && !Object::cast_to<MossPhysicsWorld3D>(ancestor)) ancestor = ancestor->get_parent();
    m_world = Object::cast_to<MossPhysicsWorld3D>(ancestor);
    if (!m_world) return;
    m_joint_id = is_distance_joint()
        ? m_world->add_distance_joint(a, b, m_min_distance, m_max_distance)
        : m_world->add_fixed_joint(a, b);
    if (m_joint_id == 0) UtilityFunctions::push_warning("MossGodot: joint could not be created; check that both bodies are active in the same Moss world.");
}
void MossJoint3D::unregister_joint() {
    if (m_world && m_joint_id != 0) m_world->remove_joint(m_joint_id);
    m_joint_id = 0;
    m_world = nullptr;
}
void MossJoint3D::on_body_invalidated(Object *body) {
    if (!body || !is_inside_tree()) return;
    Node *a = get_node_or_null(m_body_a);
    Node *b = get_node_or_null(m_body_b);
    if (a == body || b == body) {
        unregister_joint();
        call_deferred("_moss_deferred_register");
    }
}

void MossJoint2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_body_a", "path"), &MossJoint2D::set_body_a);
    ClassDB::bind_method(D_METHOD("get_body_a"), &MossJoint2D::get_body_a);
    ClassDB::bind_method(D_METHOD("set_body_b", "path"), &MossJoint2D::set_body_b);
    ClassDB::bind_method(D_METHOD("get_body_b"), &MossJoint2D::get_body_b);
    ClassDB::bind_method(D_METHOD("set_min_distance", "distance"), &MossJoint2D::set_min_distance);
    ClassDB::bind_method(D_METHOD("get_min_distance"), &MossJoint2D::get_min_distance);
    ClassDB::bind_method(D_METHOD("set_max_distance", "distance"), &MossJoint2D::set_max_distance);
    ClassDB::bind_method(D_METHOD("get_max_distance"), &MossJoint2D::get_max_distance);
    ClassDB::bind_method(D_METHOD("is_moss_active"), &MossJoint2D::is_moss_active);
    ClassDB::bind_method(D_METHOD("_moss_deferred_register"), &MossJoint2D::_moss_deferred_register);
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "body_a"), "set_body_a", "get_body_a");
    ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "body_b"), "set_body_b", "get_body_b");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_distance", PROPERTY_HINT_RANGE, "0,10000,0.01"), "set_min_distance", "get_min_distance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance", PROPERTY_HINT_RANGE, "0,10000,0.01"), "set_max_distance", "get_max_distance");
}
void MossJoint2D::set_body_a(const NodePath &value) { m_body_a = value; if (is_inside_tree()) { unregister_joint(); call_deferred("_moss_deferred_register"); } }
void MossJoint2D::set_body_b(const NodePath &value) { m_body_b = value; if (is_inside_tree()) { unregister_joint(); call_deferred("_moss_deferred_register"); } }
void MossJoint2D::set_min_distance(float value) { m_min_distance = Math::max(0.0f, value); if (is_inside_tree()) { unregister_joint(); call_deferred("_moss_deferred_register"); } }
void MossJoint2D::set_max_distance(float value) { m_max_distance = Math::max(m_min_distance, value); if (is_inside_tree()) { unregister_joint(); call_deferred("_moss_deferred_register"); } }
void MossJoint2D::_notification(int what) {
    if (what == NOTIFICATION_READY) call_deferred("_moss_deferred_register");
    if (what == NOTIFICATION_EXIT_TREE) unregister_joint();
}
void MossJoint2D::_moss_deferred_register() {
    if (!is_inside_tree() || m_joint_id != 0) return;
    MossRigidBody2D *a = Object::cast_to<MossRigidBody2D>(get_node_or_null(m_body_a));
    MossRigidBody2D *b = Object::cast_to<MossRigidBody2D>(get_node_or_null(m_body_b));
    if (!a || !b) {
        UtilityFunctions::push_warning("MossGodot: joint requires valid body_a and body_b MossRigidBody2D node paths.");
        return;
    }
    Node *ancestor = get_parent();
    while (ancestor && !Object::cast_to<MossPhysicsWorld2D>(ancestor)) ancestor = ancestor->get_parent();
    m_world = Object::cast_to<MossPhysicsWorld2D>(ancestor);
    if (!m_world) return;
    m_joint_id = is_distance_joint()
        ? m_world->add_distance_joint(a, b, m_min_distance, m_max_distance)
        : m_world->add_fixed_joint(a, b);
    if (m_joint_id == 0) UtilityFunctions::push_warning("MossGodot: joint could not be created; check that both bodies are active in the same Moss world.");
}
void MossJoint2D::unregister_joint() {
    if (m_world && m_joint_id != 0) m_world->remove_joint(m_joint_id);
    m_joint_id = 0;
    m_world = nullptr;
}
void MossJoint2D::on_body_invalidated(Object *body) {
    if (!body || !is_inside_tree()) return;
    Node *a = get_node_or_null(m_body_a);
    Node *b = get_node_or_null(m_body_b);
    if (a == body || b == body) {
        unregister_joint();
        call_deferred("_moss_deferred_register");
    }
}
