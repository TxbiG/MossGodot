#pragma once

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/variant/packed_float32_array.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <Moss/Physics/Collision/Shape/Shapes.h>

namespace mossgodot {

class MossShape3D : public godot::Resource {
    GDCLASS(MossShape3D, godot::Resource)
public:
    virtual ::Ref<::Shape> create_moss_shape() const = 0;
protected:
    static void _bind_methods() {}
};

class MossBoxShape3D final : public MossShape3D {
    GDCLASS(MossBoxShape3D, MossShape3D)
public:
    void set_size(const godot::Vector3 &value) { m_size = value.max(godot::Vector3(0.001, 0.001, 0.001)); emit_changed(); }
    godot::Vector3 get_size() const { return m_size; }
    ::Ref<::Shape> create_moss_shape() const override;
protected:
    static void _bind_methods();
private:
    godot::Vector3 m_size = godot::Vector3(1, 1, 1);
};

class MossSphereShape3D final : public MossShape3D {
    GDCLASS(MossSphereShape3D, MossShape3D)
public:
    void set_radius(float value) { m_radius = godot::Math::max(0.001f, value); emit_changed(); }
    float get_radius() const { return m_radius; }
    ::Ref<::Shape> create_moss_shape() const override;
protected:
    static void _bind_methods();
private:
    float m_radius = 0.5f;
};

class MossCapsuleShape3D final : public MossShape3D {
    GDCLASS(MossCapsuleShape3D, MossShape3D)
public:
    void set_radius(float value) {
        m_radius = godot::Math::max(0.001f, value);
        m_height = godot::Math::max(m_height, m_radius * 2.0f);
        emit_changed();
    }
    float get_radius() const { return m_radius; }
    void set_height(float value) { m_height = godot::Math::max(m_radius * 2.0f, value); emit_changed(); }
    float get_height() const { return m_height; }
    ::Ref<::Shape> create_moss_shape() const override;
protected:
    static void _bind_methods();
private:
    float m_radius = 0.5f;
    float m_height = 2.0f;
};


class MossMeshShape3D final : public MossShape3D {
    GDCLASS(MossMeshShape3D, MossShape3D)
public:
    void set_mesh(const godot::Ref<godot::Mesh> &value) { m_mesh = value; emit_changed(); }
    godot::Ref<godot::Mesh> get_mesh() const { return m_mesh; }
    void set_reverse_winding(bool value) { m_reverse_winding = value; emit_changed(); }
    bool is_reverse_winding() const { return m_reverse_winding; }
    ::Ref<::Shape> create_moss_shape() const override;
protected:
    static void _bind_methods();
private:
    godot::Ref<godot::Mesh> m_mesh;
    bool m_reverse_winding = false;
};

class MossHeightFieldShape3D final : public MossShape3D {
    GDCLASS(MossHeightFieldShape3D, MossShape3D)
public:
    void set_heights(const godot::PackedFloat32Array &value) { m_heights = value; emit_changed(); }
    godot::PackedFloat32Array get_heights() const { return m_heights; }
    void set_sample_count(int value) { m_sample_count = godot::Math::max(2, value); emit_changed(); }
    int get_sample_count() const { return m_sample_count; }
    void set_horizontal_spacing(float value) { m_horizontal_spacing = godot::Math::max(0.001f, value); emit_changed(); }
    float get_horizontal_spacing() const { return m_horizontal_spacing; }
    void set_height_scale(float value) { m_height_scale = godot::Math::max(0.001f, value); emit_changed(); }
    float get_height_scale() const { return m_height_scale; }
    void set_height_offset(float value) { m_height_offset = value; emit_changed(); }
    float get_height_offset() const { return m_height_offset; }
    ::Ref<::Shape> create_moss_shape() const override;
protected:
    static void _bind_methods();
private:
    godot::PackedFloat32Array m_heights;
    int m_sample_count = 2;
    float m_horizontal_spacing = 1.0f;
    float m_height_scale = 1.0f;
    float m_height_offset = 0.0f;
};

class MossShape2D : public godot::Resource {
    GDCLASS(MossShape2D, godot::Resource)
public:
    virtual ::Ref<::Shape> create_moss_shape() const = 0;
protected:
    static void _bind_methods() {}
};

class MossRectangleShape2D final : public MossShape2D {
    GDCLASS(MossRectangleShape2D, MossShape2D)
public:
    void set_size(const godot::Vector2 &value) { m_size = value.max(godot::Vector2(0.001, 0.001)); emit_changed(); }
    godot::Vector2 get_size() const { return m_size; }
    ::Ref<::Shape> create_moss_shape() const override;
protected:
    static void _bind_methods();
private:
    godot::Vector2 m_size = godot::Vector2(32, 32);
};

class MossCircleShape2D final : public MossShape2D {
    GDCLASS(MossCircleShape2D, MossShape2D)
public:
    void set_radius(float value) { m_radius = godot::Math::max(0.001f, value); emit_changed(); }
    float get_radius() const { return m_radius; }
    ::Ref<::Shape> create_moss_shape() const override;
protected:
    static void _bind_methods();
private:
    float m_radius = 16.0f;
};

} // namespace mossgodot
