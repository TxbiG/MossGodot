#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>

class MossSubsystem : public godot::RefCounted {
    GDCLASS(MossSubsystem, godot::RefCounted)

protected:
    static void _bind_methods();

public:
    virtual godot::StringName get_subsystem_name() const;
    virtual bool is_available() const;
    virtual bool is_moss_native_backend_connected() const;
    virtual godot::String get_integration_status() const;
};
