#include "moss_shape_resources.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <Moss/Geometry/Triangle.h>
#include <cmath>
#include <algorithm>

using namespace godot;
using namespace mossgodot;

static ::Ref<::Shape> take_shape(::Shape::ShapeResult &&result) {
    if (result.HasError()) {
        UtilityFunctions::push_error("MossGodot: failed to create Moss collision shape.");
        return {};
    }
    return result.Get();
}

::Ref<::Shape> MossBoxShape3D::create_moss_shape() const {
    const Vector3 half = m_size * 0.5;
    ::BoxShapeSettings settings(::Vec3(half.x, half.y, half.z));
    return take_shape(settings.Create());
}

::Ref<::Shape> MossSphereShape3D::create_moss_shape() const {
    ::SphereShapeSettings settings(m_radius);
    return take_shape(settings.Create());
}

::Ref<::Shape> MossCapsuleShape3D::create_moss_shape() const {
    ::CapsuleShapeSettings settings((m_height - 2.0f * m_radius) * 0.5f, m_radius);
    return take_shape(settings.Create());
}


::Ref<::Shape> MossMeshShape3D::create_moss_shape() const {
    if (m_mesh.is_null()) {
        UtilityFunctions::push_error("MossGodot: MossMeshShape3D requires a Mesh resource.");
        return {};
    }
    ::TriangleList triangles;
    for (int32_t surface = 0; surface < m_mesh->get_surface_count(); ++surface) {
        if (m_mesh->surface_get_primitive_type(surface) != Mesh::PRIMITIVE_TRIANGLES) {
            UtilityFunctions::push_warning("MossGodot: skipped a non-triangle mesh surface.");
            continue;
        }
        const Array arrays = m_mesh->surface_get_arrays(surface);
        if (arrays.size() <= Mesh::ARRAY_VERTEX) continue;
        const PackedVector3Array vertices = arrays[Mesh::ARRAY_VERTEX];
        if (vertices.size() < 3) continue;
        PackedInt32Array indices;
        if (arrays.size() > Mesh::ARRAY_INDEX) indices = arrays[Mesh::ARRAY_INDEX];
        const int64_t index_count = indices.is_empty() ? vertices.size() : indices.size();
        for (int64_t i = 0; i + 2 < index_count; i += 3) {
            int64_t i0 = indices.is_empty() ? i : indices[i];
            int64_t i1 = indices.is_empty() ? i + 1 : indices[i + 1];
            int64_t i2 = indices.is_empty() ? i + 2 : indices[i + 2];
            if (i0 < 0 || i1 < 0 || i2 < 0 || i0 >= vertices.size() || i1 >= vertices.size() || i2 >= vertices.size()) {
                UtilityFunctions::push_warning("MossGodot: ignored an out-of-range mesh triangle index.");
                continue;
            }
            if (m_reverse_winding) std::swap(i1, i2);
            const Vector3 a = vertices[i0];
            const Vector3 b = vertices[i1];
            const Vector3 c = vertices[i2];
            triangles.push_back(::Triangle(::Vec3(a.x, a.y, a.z), ::Vec3(b.x, b.y, b.z), ::Vec3(c.x, c.y, c.z)));
        }
    }
    if (triangles.empty()) {
        UtilityFunctions::push_error("MossGodot: mesh contains no valid triangles.");
        return {};
    }
    ::MeshShapeSettings settings(triangles);
    return take_shape(settings.Create());
}

::Ref<::Shape> MossHeightFieldShape3D::create_moss_shape() const {
    const int64_t expected = static_cast<int64_t>(m_sample_count) * static_cast<int64_t>(m_sample_count);
    if (m_sample_count < 2 || m_heights.size() != expected) {
        UtilityFunctions::push_error("MossGodot: heightfield data must contain sample_count * sample_count floats.");
        return {};
    }
    ::HeightFieldShapeSettings settings(
        m_heights.ptr(),
        ::Vec3(0.0f, m_height_offset, 0.0f),
        ::Vec3(m_horizontal_spacing, m_height_scale, m_horizontal_spacing),
        static_cast<uint32_t>(m_sample_count));
    return take_shape(settings.Create());
}

::Ref<::Shape> MossRectangleShape2D::create_moss_shape() const {
    // Plane2D bodies are constrained to XY. A thin Z extent avoids zero-volume box geometry.
    ::BoxShapeSettings settings(::Vec3(m_size.x * 0.5f, m_size.y * 0.5f, 0.01f));
    return take_shape(settings.Create());
}

::Ref<::Shape> MossCircleShape2D::create_moss_shape() const {
    ::SphereShapeSettings settings(m_radius);
    return take_shape(settings.Create());
}

void MossBoxShape3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_size", "size"), &MossBoxShape3D::set_size);
    ClassDB::bind_method(D_METHOD("get_size"), &MossBoxShape3D::get_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "size", PROPERTY_HINT_LINK), "set_size", "get_size");
}
void MossSphereShape3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_radius", "radius"), &MossSphereShape3D::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"), &MossSphereShape3D::get_radius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius", PROPERTY_HINT_RANGE, "0.001,1000,0.01,or_greater"), "set_radius", "get_radius");
}
void MossCapsuleShape3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_radius", "radius"), &MossCapsuleShape3D::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"), &MossCapsuleShape3D::get_radius);
    ClassDB::bind_method(D_METHOD("set_height", "height"), &MossCapsuleShape3D::set_height);
    ClassDB::bind_method(D_METHOD("get_height"), &MossCapsuleShape3D::get_height);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius", PROPERTY_HINT_RANGE, "0.001,1000,0.01,or_greater"), "set_radius", "get_radius");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "height", PROPERTY_HINT_RANGE, "0.002,1000,0.01,or_greater"), "set_height", "get_height");
}

void MossMeshShape3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_mesh", "mesh"), &MossMeshShape3D::set_mesh);
    ClassDB::bind_method(D_METHOD("get_mesh"), &MossMeshShape3D::get_mesh);
    ClassDB::bind_method(D_METHOD("set_reverse_winding", "enabled"), &MossMeshShape3D::set_reverse_winding);
    ClassDB::bind_method(D_METHOD("is_reverse_winding"), &MossMeshShape3D::is_reverse_winding);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "mesh", PROPERTY_HINT_RESOURCE_TYPE, "Mesh"), "set_mesh", "get_mesh");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "reverse_winding"), "set_reverse_winding", "is_reverse_winding");
}
void MossHeightFieldShape3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_heights", "heights"), &MossHeightFieldShape3D::set_heights);
    ClassDB::bind_method(D_METHOD("get_heights"), &MossHeightFieldShape3D::get_heights);
    ClassDB::bind_method(D_METHOD("set_sample_count", "count"), &MossHeightFieldShape3D::set_sample_count);
    ClassDB::bind_method(D_METHOD("get_sample_count"), &MossHeightFieldShape3D::get_sample_count);
    ClassDB::bind_method(D_METHOD("set_horizontal_spacing", "spacing"), &MossHeightFieldShape3D::set_horizontal_spacing);
    ClassDB::bind_method(D_METHOD("get_horizontal_spacing"), &MossHeightFieldShape3D::get_horizontal_spacing);
    ClassDB::bind_method(D_METHOD("set_height_scale", "scale"), &MossHeightFieldShape3D::set_height_scale);
    ClassDB::bind_method(D_METHOD("get_height_scale"), &MossHeightFieldShape3D::get_height_scale);
    ClassDB::bind_method(D_METHOD("set_height_offset", "offset"), &MossHeightFieldShape3D::set_height_offset);
    ClassDB::bind_method(D_METHOD("get_height_offset"), &MossHeightFieldShape3D::get_height_offset);
    ADD_PROPERTY(PropertyInfo(Variant::PACKED_FLOAT32_ARRAY, "heights"), "set_heights", "get_heights");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "sample_count", PROPERTY_HINT_RANGE, "2,2048,1"), "set_sample_count", "get_sample_count");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "horizontal_spacing", PROPERTY_HINT_RANGE, "0.001,1000,0.01"), "set_horizontal_spacing", "get_horizontal_spacing");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "height_scale", PROPERTY_HINT_RANGE, "0.001,1000,0.01"), "set_height_scale", "get_height_scale");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "height_offset"), "set_height_offset", "get_height_offset");
}

void MossRectangleShape2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_size", "size"), &MossRectangleShape2D::set_size);
    ClassDB::bind_method(D_METHOD("get_size"), &MossRectangleShape2D::get_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "size", PROPERTY_HINT_LINK), "set_size", "get_size");
}
void MossCircleShape2D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_radius", "radius"), &MossCircleShape2D::set_radius);
    ClassDB::bind_method(D_METHOD("get_radius"), &MossCircleShape2D::get_radius);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius", PROPERTY_HINT_RANGE, "0.001,100000,0.1,or_greater"), "set_radius", "get_radius");
}
