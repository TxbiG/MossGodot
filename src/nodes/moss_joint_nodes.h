#pragma once

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <cstdint>

namespace mossgodot {
class MossPhysicsWorld3D;
class MossPhysicsWorld2D;

class MossJoint3D : public godot::Node3D {
    GDCLASS(MossJoint3D, godot::Node3D)
public:
    void set_body_a(const godot::NodePath &value);
    godot::NodePath get_body_a() const { return m_body_a; }
    void set_body_b(const godot::NodePath &value);
    godot::NodePath get_body_b() const { return m_body_b; }
    void set_min_distance(float value);
    float get_min_distance() const { return m_min_distance; }
    void set_max_distance(float value);
    float get_max_distance() const { return m_max_distance; }
    bool is_moss_active() const { return m_joint_id != 0; }
    void _notification(int what);
    void on_body_invalidated(godot::Object *body);
protected:
    static void _bind_methods();
    virtual bool is_distance_joint() const = 0;
private:
    void _moss_deferred_register();
    void unregister_joint();
    godot::NodePath m_body_a;
    godot::NodePath m_body_b;
    MossPhysicsWorld3D *m_world = nullptr;
    uint64_t m_joint_id = 0;
    float m_min_distance = 0.0f;
    float m_max_distance = 1.0f;
};

class MossFixedJoint3D final : public MossJoint3D {
    GDCLASS(MossFixedJoint3D, MossJoint3D)
protected:
    static void _bind_methods() {}
    bool is_distance_joint() const override { return false; }
};

class MossDistanceJoint3D final : public MossJoint3D {
    GDCLASS(MossDistanceJoint3D, MossJoint3D)
protected:
    static void _bind_methods() {}
    bool is_distance_joint() const override { return true; }
};

class MossJoint2D : public godot::Node2D {
    GDCLASS(MossJoint2D, godot::Node2D)
public:
    void set_body_a(const godot::NodePath &value);
    godot::NodePath get_body_a() const { return m_body_a; }
    void set_body_b(const godot::NodePath &value);
    godot::NodePath get_body_b() const { return m_body_b; }
    void set_min_distance(float value);
    float get_min_distance() const { return m_min_distance; }
    void set_max_distance(float value);
    float get_max_distance() const { return m_max_distance; }
    bool is_moss_active() const { return m_joint_id != 0; }
    void _notification(int what);
    void on_body_invalidated(godot::Object *body);
protected:
    static void _bind_methods();
    virtual bool is_distance_joint() const = 0;
private:
    void _moss_deferred_register();
    void unregister_joint();
    godot::NodePath m_body_a;
    godot::NodePath m_body_b;
    MossPhysicsWorld2D *m_world = nullptr;
    uint64_t m_joint_id = 0;
    float m_min_distance = 0.0f;
    float m_max_distance = 1.0f;
};

class MossFixedJoint2D final : public MossJoint2D {
    GDCLASS(MossFixedJoint2D, MossJoint2D)
protected:
    static void _bind_methods() {}
    bool is_distance_joint() const override { return false; }
};

class MossDistanceJoint2D final : public MossJoint2D {
    GDCLASS(MossDistanceJoint2D, MossJoint2D)
protected:
    static void _bind_methods() {}
    bool is_distance_joint() const override { return true; }
};

} // namespace mossgodot
