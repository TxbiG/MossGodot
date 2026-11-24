#pragma once

#include "moss_subsystem.h"
#include <godot_cpp/variant/string_name.hpp>

class MossCoreSubsystem : public MossSubsystem {
    GDCLASS(MossCoreSubsystem, MossSubsystem)
protected: static void _bind_methods();
public: godot::StringName get_subsystem_name() const override; bool is_available() const override; bool is_moss_native_backend_connected() const override; godot::String get_integration_status() const override;
};

class MossPhysicsSubsystem : public MossSubsystem {
    GDCLASS(MossPhysicsSubsystem, MossSubsystem)
protected: static void _bind_methods();
public: godot::StringName get_subsystem_name() const override; bool is_available() const override; bool is_moss_native_backend_connected() const override; godot::String get_integration_status() const override;
};

class MossAudioSubsystem : public MossSubsystem {
    GDCLASS(MossAudioSubsystem, MossSubsystem)
protected: static void _bind_methods();
public: godot::StringName get_subsystem_name() const override; bool is_available() const override; godot::String get_integration_status() const override;
};

class MossInputSubsystem : public MossSubsystem {
    GDCLASS(MossInputSubsystem, MossSubsystem)
protected: static void _bind_methods();
public:
    godot::StringName get_subsystem_name() const override; bool is_available() const override; godot::String get_integration_status() const override;
    bool is_action_pressed(const godot::StringName &action) const;
    double get_action_strength(const godot::StringName &action) const;
    double get_axis(const godot::StringName &negative_action, const godot::StringName &positive_action) const;
};

class MossNavigationSubsystem : public MossSubsystem {
    GDCLASS(MossNavigationSubsystem, MossSubsystem)
protected: static void _bind_methods();
public: godot::StringName get_subsystem_name() const override; bool is_available() const override; godot::String get_integration_status() const override;
};

class MossRenderingSubsystem : public MossSubsystem {
    GDCLASS(MossRenderingSubsystem, MossSubsystem)
protected: static void _bind_methods();
public: godot::StringName get_subsystem_name() const override; bool is_available() const override; godot::String get_integration_status() const override;
};

class MossNetworkingSubsystem : public MossSubsystem {
    GDCLASS(MossNetworkingSubsystem, MossSubsystem)
protected: static void _bind_methods();
public: godot::StringName get_subsystem_name() const override; bool is_available() const override; godot::String get_integration_status() const override;
};

class MossXRSubsystem : public MossSubsystem {
    GDCLASS(MossXRSubsystem, MossSubsystem)
protected: static void _bind_methods();
public: godot::StringName get_subsystem_name() const override; bool is_available() const override; godot::String get_integration_status() const override;
};
