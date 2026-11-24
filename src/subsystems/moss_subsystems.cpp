#include "moss_subsystems.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/xr_server.hpp>

using namespace godot;

void MossCoreSubsystem::_bind_methods() {}
StringName MossCoreSubsystem::get_subsystem_name() const { return "Core"; }
bool MossCoreSubsystem::is_available() const { return true; }
bool MossCoreSubsystem::is_moss_native_backend_connected() const { return true; }
String MossCoreSubsystem::get_integration_status() const { return "Connected. Core utilities use Moss C++17 headers for Timer, SIMDJson, Vec2, Vec3 and Quat."; }

void MossPhysicsSubsystem::_bind_methods() {}
StringName MossPhysicsSubsystem::get_subsystem_name() const { return "Physics"; }
bool MossPhysicsSubsystem::is_available() const {
#if defined(MOSS_GODOT_WITH_PHYSICS)
    return true;
#else
    return false;
#endif
}
bool MossPhysicsSubsystem::is_moss_native_backend_connected() const { return is_available(); }
String MossPhysicsSubsystem::get_integration_status() const {
#if defined(MOSS_GODOT_WITH_PHYSICS)
    return "Connected. Use MossPhysicsWorld3D and MossRigidBody3D for Moss PhysicsSystem simulation. Godot's default PhysicsServer3D remains unchanged.";
#else
    return "Disabled at build time. Reconfigure with MOSS_GODOT_BUILD_PHYSICS=ON to include the Moss PhysicsSystem adapter.";
#endif
}

void MossAudioSubsystem::_bind_methods() {}
StringName MossAudioSubsystem::get_subsystem_name() const { return "Audio"; }
bool MossAudioSubsystem::is_available() const { return AudioServer::get_singleton() != nullptr; }
String MossAudioSubsystem::get_integration_status() const { return "Not enabled. Moss audio has platform-specific backends and must be selected explicitly; Godot AudioServer remains the host audio system."; }

void MossInputSubsystem::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_action_pressed", "action"), &MossInputSubsystem::is_action_pressed);
    ClassDB::bind_method(D_METHOD("get_action_strength", "action"), &MossInputSubsystem::get_action_strength);
    ClassDB::bind_method(D_METHOD("get_axis", "negative_action", "positive_action"), &MossInputSubsystem::get_axis);
}
StringName MossInputSubsystem::get_subsystem_name() const { return "Input"; }
bool MossInputSubsystem::is_available() const { return Input::get_singleton() != nullptr; }
String MossInputSubsystem::get_integration_status() const { return "Godot input adapter available. Godot owns window/event polling; Moss_Platform is not initialized as a second platform layer."; }
bool MossInputSubsystem::is_action_pressed(const StringName &action) const { return Input::get_singleton() && Input::get_singleton()->is_action_pressed(action); }
double MossInputSubsystem::get_action_strength(const StringName &action) const { return Input::get_singleton() ? Input::get_singleton()->get_action_strength(action) : 0.0; }
double MossInputSubsystem::get_axis(const StringName &negative_action, const StringName &positive_action) const { return Input::get_singleton() ? Input::get_singleton()->get_axis(negative_action, positive_action) : 0.0; }

void MossNavigationSubsystem::_bind_methods() {}
StringName MossNavigationSubsystem::get_subsystem_name() const { return "Navigation"; }
bool MossNavigationSubsystem::is_available() const { return NavigationServer3D::get_singleton() != nullptr; }
String MossNavigationSubsystem::get_integration_status() const { return "Not enabled. The current Moss CMake source list excludes its Navigation/Recast sources. Use Godot NavigationServer until that module is built and validated."; }

void MossRenderingSubsystem::_bind_methods() {}
StringName MossRenderingSubsystem::get_subsystem_name() const { return "Rendering"; }
bool MossRenderingSubsystem::is_available() const { return RenderingServer::get_singleton() != nullptr; }
String MossRenderingSubsystem::get_integration_status() const { return "Godot owns rendering, GPU device and window presentation. Moss renderer interop is intentionally not initialized to avoid competing graphics contexts."; }

void MossNetworkingSubsystem::_bind_methods() {}
StringName MossNetworkingSubsystem::get_subsystem_name() const { return "Networking"; }
String MossNetworkingSubsystem::get_integration_status() const { return "Not enabled. Moss_Network.h declares ENet APIs, but the uploaded Moss CMake source list does not currently add src/Network sources to the Moss target."; }

void MossXRSubsystem::_bind_methods() {}
StringName MossXRSubsystem::get_subsystem_name() const { return "XR"; }
bool MossXRSubsystem::is_available() const { return XRServer::get_singleton() != nullptr; }
String MossXRSubsystem::get_integration_status() const { return "Not enabled. Godot XRServer/OpenXR owns the active XR session; a second Moss OpenXR session must not be started alongside it."; }
