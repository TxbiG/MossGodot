#pragma once

#include "../physics/moss_physics_world_core.h"
#include "../resources/moss_shape_resources.h"

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace mossgodot {

class MossPhysicsWorld3D;
class MossPhysicsWorld2D;

enum class MossBodyMode : int { Static = 0, Kinematic = 1, Dynamic = 2 };

class MossRigidBody3D : public godot::Node3D {
    GDCLASS(MossRigidBody3D, godot::Node3D)
public:
    void set_shape(const godot::Ref<MossShape3D> &value);
    godot::Ref<MossShape3D> get_shape() const { return m_shape; }
    void set_body_mode(int value);
    int get_body_mode() const { return static_cast<int>(m_mode); }
    void set_mass(float value);
    float get_mass() const { return m_mass; }
    void set_sensor(bool value);
    bool is_sensor() const { return m_sensor; }
    void set_collision_layer(int value);
    int get_collision_layer() const { return m_collision_layer; }
    void set_friction(float value);
    float get_friction() const { return m_friction; }
    void set_restitution(float value);
    float get_restitution() const { return m_restitution; }
    void apply_impulse(const godot::Vector3 &impulse);
    void set_linear_velocity(const godot::Vector3 &velocity);
    void move_kinematic(const godot::Transform3D &target, float delta);
    bool is_moss_active() const { return m_registered; }

    void _notification(int what);
protected:
    static void _bind_methods();
private:
    friend class MossPhysicsWorld3D;
    void try_register_with_world();
    void unregister_from_world();
    void sync_from_physics();
    void notify_contact(bool entered, godot::Object *other);
    void rebuild_registered_body();
    void _moss_shape_changed();

    godot::Ref<MossShape3D> m_shape;
    MossPhysicsWorld3D *m_world = nullptr;
    ::BodyID m_body_id;
    MossBodyMode m_mode = MossBodyMode::Dynamic;
    float m_mass = 1.0f;
    float m_friction = 0.5f;
    float m_restitution = 0.0f;
    int m_collision_layer = 0;
    bool m_sensor = false;
    bool m_registered = false;
};

class MossRigidBody2D : public godot::Node2D {
    GDCLASS(MossRigidBody2D, godot::Node2D)
public:
    void set_shape(const godot::Ref<MossShape2D> &value);
    godot::Ref<MossShape2D> get_shape() const { return m_shape; }
    void set_body_mode(int value);
    int get_body_mode() const { return static_cast<int>(m_mode); }
    void set_mass(float value);
    float get_mass() const { return m_mass; }
    void set_sensor(bool value);
    bool is_sensor() const { return m_sensor; }
    void set_collision_layer(int value);
    int get_collision_layer() const { return m_collision_layer; }
    void set_friction(float value);
    float get_friction() const { return m_friction; }
    void set_restitution(float value);
    float get_restitution() const { return m_restitution; }
    void apply_impulse(const godot::Vector2 &impulse);
    void set_linear_velocity(const godot::Vector2 &velocity);
    void move_kinematic(const godot::Transform2D &target, float delta);
    bool is_moss_active() const { return m_registered; }

    void _notification(int what);
protected:
    static void _bind_methods();
private:
    friend class MossPhysicsWorld2D;
    void try_register_with_world();
    void unregister_from_world();
    void sync_from_physics();
    void notify_contact(bool entered, godot::Object *other);
    void rebuild_registered_body();
    void _moss_shape_changed();

    godot::Ref<MossShape2D> m_shape;
    MossPhysicsWorld2D *m_world = nullptr;
    ::BodyID m_body_id;
    MossBodyMode m_mode = MossBodyMode::Dynamic;
    float m_mass = 1.0f;
    float m_friction = 0.5f;
    float m_restitution = 0.0f;
    int m_collision_layer = 0;
    bool m_sensor = false;
    bool m_registered = false;
};

} // namespace mossgodot
