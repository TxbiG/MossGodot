#include "moss_math.h"
#include "moss_native.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace {
::Vec2 to_moss(const Vector2 &v) { return ::Vec2(static_cast<float>(v.x), static_cast<float>(v.y)); }
::Vec3 to_moss(const Vector3 &v) { return ::Vec3(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)); }
Vector2 to_godot(const ::Vec2 &v) { return Vector2(v.GetX(), v.GetY()); }
Vector3 to_godot(const ::Vec3 &v) { return Vector3(v.GetX(), v.GetY(), v.GetZ()); }
::Quat to_moss(const Quaternion &q) { return ::Quat(static_cast<float>(q.x), static_cast<float>(q.y), static_cast<float>(q.z), static_cast<float>(q.w)); }
Quaternion to_godot(const ::Quat &q) { return Quaternion(q.GetX(), q.GetY(), q.GetZ(), q.GetW()); }
}

void MossMath::_bind_methods() {
    ClassDB::bind_method(D_METHOD("vector2_add", "a", "b"), &MossMath::vector2_add);
    ClassDB::bind_method(D_METHOD("vector2_normalized", "value"), &MossMath::vector2_normalized);
    ClassDB::bind_method(D_METHOD("vector2_dot", "a", "b"), &MossMath::vector2_dot);
    ClassDB::bind_method(D_METHOD("vector3_add", "a", "b"), &MossMath::vector3_add);
    ClassDB::bind_method(D_METHOD("vector3_subtract", "a", "b"), &MossMath::vector3_subtract);
    ClassDB::bind_method(D_METHOD("vector3_scale", "value", "scale"), &MossMath::vector3_scale);
    ClassDB::bind_method(D_METHOD("vector3_normalized", "value"), &MossMath::vector3_normalized);
    ClassDB::bind_method(D_METHOD("vector3_cross", "a", "b"), &MossMath::vector3_cross);
    ClassDB::bind_method(D_METHOD("vector3_dot", "a", "b"), &MossMath::vector3_dot);
    ClassDB::bind_method(D_METHOD("vector3_length", "value"), &MossMath::vector3_length);
    ClassDB::bind_method(D_METHOD("vector3_lerp", "a", "b", "weight"), &MossMath::vector3_lerp);
    ClassDB::bind_method(D_METHOD("vector3_reflect", "value", "normal"), &MossMath::vector3_reflect);
    ClassDB::bind_method(D_METHOD("quaternion_normalized", "value"), &MossMath::quaternion_normalized);
    ClassDB::bind_method(D_METHOD("quaternion_from_euler", "radians"), &MossMath::quaternion_from_euler);
    ClassDB::bind_method(D_METHOD("quaternion_to_euler", "value"), &MossMath::quaternion_to_euler);
}

Vector2 MossMath::vector2_add(const Vector2 &a, const Vector2 &b) const { return to_godot(to_moss(a) + to_moss(b)); }
Vector2 MossMath::vector2_normalized(const Vector2 &value) const {
    const ::Vec2 v = to_moss(value);
    return v.LengthSq() > 1.0e-12f ? to_godot(v.Normalized()) : Vector2();
}
double MossMath::vector2_dot(const Vector2 &a, const Vector2 &b) const { return to_moss(a).Dot(to_moss(b)); }
Vector3 MossMath::vector3_add(const Vector3 &a, const Vector3 &b) const { return to_godot(to_moss(a) + to_moss(b)); }
Vector3 MossMath::vector3_subtract(const Vector3 &a, const Vector3 &b) const { return to_godot(to_moss(a) - to_moss(b)); }
Vector3 MossMath::vector3_scale(const Vector3 &value, double scale) const { return to_godot(to_moss(value) * static_cast<float>(scale)); }
Vector3 MossMath::vector3_normalized(const Vector3 &value) const {
    const ::Vec3 v = to_moss(value);
    return v.LengthSq() > 1.0e-12f ? to_godot(v.Normalized()) : Vector3();
}
Vector3 MossMath::vector3_cross(const Vector3 &a, const Vector3 &b) const { return to_godot(to_moss(a).Cross(to_moss(b))); }
double MossMath::vector3_dot(const Vector3 &a, const Vector3 &b) const { return to_moss(a).Dot(to_moss(b)); }
double MossMath::vector3_length(const Vector3 &value) const { return to_moss(value).Length(); }
Vector3 MossMath::vector3_lerp(const Vector3 &a, const Vector3 &b, double weight) const {
    const float t = static_cast<float>(weight);
    const ::Vec3 av = to_moss(a);
    return to_godot(av + (to_moss(b) - av) * t);
}
Vector3 MossMath::vector3_reflect(const Vector3 &value, const Vector3 &normal) const {
    const ::Vec3 v = to_moss(value);
    const ::Vec3 n = to_moss(normal).Normalized();
    return to_godot(v - n * (2.0f * v.Dot(n)));
}
Quaternion MossMath::quaternion_normalized(const Quaternion &value) const { return to_godot(to_moss(value).Normalized()); }
Quaternion MossMath::quaternion_from_euler(const Vector3 &radians) const { return to_godot(::Quat::EulerAngles(to_moss(radians))); }
Vector3 MossMath::quaternion_to_euler(const Quaternion &value) const { return to_godot(to_moss(value).GetEulerAngles()); }
