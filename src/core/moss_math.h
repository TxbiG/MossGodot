#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/quaternion.hpp>

class MossMath : public godot::RefCounted {
    GDCLASS(MossMath, godot::RefCounted)

protected:
    static void _bind_methods();

public:
    godot::Vector2 vector2_add(const godot::Vector2 &a, const godot::Vector2 &b) const;
    godot::Vector2 vector2_normalized(const godot::Vector2 &value) const;
    double vector2_dot(const godot::Vector2 &a, const godot::Vector2 &b) const;

    godot::Vector3 vector3_add(const godot::Vector3 &a, const godot::Vector3 &b) const;
    godot::Vector3 vector3_subtract(const godot::Vector3 &a, const godot::Vector3 &b) const;
    godot::Vector3 vector3_scale(const godot::Vector3 &value, double scale) const;
    godot::Vector3 vector3_normalized(const godot::Vector3 &value) const;
    godot::Vector3 vector3_cross(const godot::Vector3 &a, const godot::Vector3 &b) const;
    double vector3_dot(const godot::Vector3 &a, const godot::Vector3 &b) const;
    double vector3_length(const godot::Vector3 &value) const;
    godot::Vector3 vector3_lerp(const godot::Vector3 &a, const godot::Vector3 &b, double weight) const;
    godot::Vector3 vector3_reflect(const godot::Vector3 &value, const godot::Vector3 &normal) const;

    godot::Quaternion quaternion_normalized(const godot::Quaternion &value) const;
    godot::Quaternion quaternion_from_euler(const godot::Vector3 &radians) const;
    godot::Vector3 quaternion_to_euler(const godot::Quaternion &value) const;
};
