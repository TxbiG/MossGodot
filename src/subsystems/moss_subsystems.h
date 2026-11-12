
#pragma once

#include "moss_subsystem.h"

// ------------------------------------------------------------
// Physics
// ------------------------------------------------------------

class MossPhysicsSubsystem : public MossSubsystem
{
    GDCLASS(MossPhysicsSubsystem, MossSubsystem)

protected:
    static void _bind_methods();

public:
    MossPhysicsSubsystem() = default;

    godot::StringName get_subsystem_name() const override;
};

// ------------------------------------------------------------
// Audio
// ------------------------------------------------------------

class MossAudioSubsystem : public MossSubsystem
{
    GDCLASS(MossAudioSubsystem, MossSubsystem)

protected:
    static void _bind_methods();

public:
    MossAudioSubsystem() = default;

    godot::StringName get_subsystem_name() const override;
};

// ------------------------------------------------------------
// Input and haptics
// ------------------------------------------------------------

class MossInputSubsystem : public MossSubsystem
{
    GDCLASS(MossInputSubsystem, MossSubsystem)

protected:
    static void _bind_methods();

public:
    MossInputSubsystem() = default;

    godot::StringName get_subsystem_name() const override;
};

// ------------------------------------------------------------
// Navigation
// ------------------------------------------------------------

class MossNavigationSubsystem : public MossSubsystem
{
    GDCLASS(MossNavigationSubsystem, MossSubsystem)

protected:
    static void _bind_methods();

public:
    MossNavigationSubsystem() = default;

    godot::StringName get_subsystem_name() const override;
};

// ------------------------------------------------------------
// Rendering
// ------------------------------------------------------------

class MossRenderingSubsystem : public MossSubsystem
{
    GDCLASS(MossRenderingSubsystem, MossSubsystem)

protected:
    static void _bind_methods();

public:
    MossRenderingSubsystem() = default;

    godot::StringName get_subsystem_name() const override;
};

// ------------------------------------------------------------
// Networking and multiplayer
// ------------------------------------------------------------

class MossNetworkingSubsystem : public MossSubsystem
{
    GDCLASS(MossNetworkingSubsystem, MossSubsystem)

protected:
    static void _bind_methods();

public:
    MossNetworkingSubsystem() = default;

    godot::StringName get_subsystem_name() const override;
};

// ------------------------------------------------------------
// OpenXR
// ------------------------------------------------------------

class MossXRSubsystem : public MossSubsystem
{
    GDCLASS(MossXRSubsystem, MossSubsystem)

protected:
    static void _bind_methods();

public:
    MossXRSubsystem() = default;

    godot::StringName get_subsystem_name() const override;
};
