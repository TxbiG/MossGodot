#pragma once

#include "../physics/moss_physics_world_core.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <unordered_map>

namespace mossgodot {
class MossRigidBody3D;
class MossRigidBody2D;
class MossJoint3D;
class MossJoint2D;
class MossShape3D;
class MossShape2D;

class MossPhysicsWorld3D : public godot::Node3D {
    GDCLASS(MossPhysicsWorld3D, godot::Node3D)
public:
    void set_gravity(const godot::Vector3 &value);
    godot::Vector3 get_gravity() const { return m_gravity; }
    void set_fixed_fps(int value);
    int get_fixed_fps() const { return m_fixed_fps; }
    void set_max_bodies(int value);
    int get_max_bodies() const { return m_max_bodies; }
    void set_worker_threads(int value);
    int get_worker_threads() const { return m_worker_threads; }
    void set_max_substeps(int value);
    int get_max_substeps() const { return m_max_substeps; }
    void set_layer_mask(int layer, int mask);
    int get_layer_mask(int layer) const;
    bool is_backend_active() const { return m_core.is_initialized(); }
    godot::Dictionary ray_cast(const godot::Vector3 &from, const godot::Vector3 &to, int query_mask = -1);
    godot::Array ray_cast_all(const godot::Vector3 &from, const godot::Vector3 &to, int query_mask = -1, int max_results = 64);
    godot::Array intersect_point(const godot::Vector3 &point, int query_mask = -1, int max_results = 64);
    godot::Array intersect_shape(const godot::Ref<MossShape3D> &shape, const godot::Transform3D &transform, int query_mask = -1, int max_results = 64);
    godot::Array cast_shape(const godot::Ref<MossShape3D> &shape, const godot::Transform3D &transform, const godot::Vector3 &motion, int query_mask = -1, int max_results = 64);
    uint64_t add_fixed_joint(MossRigidBody3D *body_a, MossRigidBody3D *body_b);
    uint64_t add_distance_joint(MossRigidBody3D *body_a, MossRigidBody3D *body_b, float min_distance, float max_distance);
    void remove_joint(uint64_t joint_id);
    void _ready() override;
    void _physics_process(double delta) override;
    void _exit_tree() override;
protected:
    static void _bind_methods();
private:
    friend class MossRigidBody3D;
    friend class MossJoint3D;
    void register_body(MossRigidBody3D *body);
    void unregister_body(MossRigidBody3D *body);
    void dispatch_contact_events();
    MossPhysicsWorldCore m_core;
    godot::Vector3 m_gravity = godot::Vector3(0, -9.81, 0);
    std::unordered_map<uint64_t, MossRigidBody3D *> m_bodies;
    double m_accumulator = 0.0;
    int m_fixed_fps = 60;
    int m_max_bodies = 4096;
    int m_worker_threads = 0;
    int m_max_substeps = 4;
};

class MossPhysicsWorld2D : public godot::Node2D {
    GDCLASS(MossPhysicsWorld2D, godot::Node2D)
public:
    void set_gravity(const godot::Vector2 &value);
    godot::Vector2 get_gravity() const { return m_gravity; }
    void set_fixed_fps(int value);
    int get_fixed_fps() const { return m_fixed_fps; }
    void set_max_bodies(int value);
    int get_max_bodies() const { return m_max_bodies; }
    void set_worker_threads(int value);
    int get_worker_threads() const { return m_worker_threads; }
    void set_max_substeps(int value);
    int get_max_substeps() const { return m_max_substeps; }
    void set_layer_mask(int layer, int mask);
    int get_layer_mask(int layer) const;
    bool is_backend_active() const { return m_core.is_initialized(); }
    godot::Dictionary ray_cast(const godot::Vector2 &from, const godot::Vector2 &to, int query_mask = -1);
    godot::Array ray_cast_all(const godot::Vector2 &from, const godot::Vector2 &to, int query_mask = -1, int max_results = 64);
    godot::Array intersect_point(const godot::Vector2 &point, int query_mask = -1, int max_results = 64);
    godot::Array intersect_shape(const godot::Ref<MossShape2D> &shape, const godot::Transform2D &transform, int query_mask = -1, int max_results = 64);
    godot::Array cast_shape(const godot::Ref<MossShape2D> &shape, const godot::Transform2D &transform, const godot::Vector2 &motion, int query_mask = -1, int max_results = 64);
    uint64_t add_fixed_joint(MossRigidBody2D *body_a, MossRigidBody2D *body_b);
    uint64_t add_distance_joint(MossRigidBody2D *body_a, MossRigidBody2D *body_b, float min_distance, float max_distance);
    void remove_joint(uint64_t joint_id);
    void _ready() override;
    void _physics_process(double delta) override;
    void _exit_tree() override;
protected:
    static void _bind_methods();
private:
    friend class MossRigidBody2D;
    friend class MossJoint2D;
    void register_body(MossRigidBody2D *body);
    void unregister_body(MossRigidBody2D *body);
    void dispatch_contact_events();
    MossPhysicsWorldCore m_core;
    godot::Vector2 m_gravity = godot::Vector2(0, 980);
    std::unordered_map<uint64_t, MossRigidBody2D *> m_bodies;
    double m_accumulator = 0.0;
    int m_fixed_fps = 60;
    int m_max_bodies = 4096;
    int m_worker_threads = 0;
    int m_max_substeps = 4;
};

} // namespace mossgodot
