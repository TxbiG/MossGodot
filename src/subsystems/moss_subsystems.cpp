
#include "moss_subsystems.h"

using namespace godot;

// ------------------------------------------------------------
// Physics
// ------------------------------------------------------------

void MossPhysicsSubsystem::_bind_methods()
{
}

StringName MossPhysicsSubsystem::get_subsystem_name() const
{
    return StringName("Physics");
}

// ------------------------------------------------------------
// Audio
// ------------------------------------------------------------

void MossAudioSubsystem::_bind_methods()
{
}

StringName MossAudioSubsystem::get_subsystem_name() const
{
    return StringName("Audio");
}

// ------------------------------------------------------------
// Input
// ------------------------------------------------------------

void MossInputSubsystem::_bind_methods()
{
}

StringName MossInputSubsystem::get_subsystem_name() const
{
    return StringName("Input");
}

// ------------------------------------------------------------
// Navigation
// ------------------------------------------------------------

void MossNavigationSubsystem::_bind_methods()
{
}

StringName MossNavigationSubsystem::get_subsystem_name() const
{
    return StringName("Navigation");
}

// ------------------------------------------------------------
// Rendering
// ------------------------------------------------------------

void MossRenderingSubsystem::_bind_methods()
{
}

StringName MossRenderingSubsystem::get_subsystem_name() const
{
    return StringName("Rendering");
}

// ------------------------------------------------------------
// Networking
// ------------------------------------------------------------

void MossNetworkingSubsystem::_bind_methods()
{
}

StringName MossNetworkingSubsystem::get_subsystem_name() const
{
    return StringName("Networking");
}

// ------------------------------------------------------------
// XR
// ------------------------------------------------------------

void MossXRSubsystem::_bind_methods()
{
}

StringName MossXRSubsystem::get_subsystem_name() const
{
    return StringName("XR");
}
