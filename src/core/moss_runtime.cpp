#include "moss_runtime.h"
#include "../subsystems/moss_subsystems.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void MossRuntime::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_version"), &MossRuntime::get_version);
    ClassDB::bind_method(D_METHOD("get_native_backend"), &MossRuntime::get_native_backend);
    ClassDB::bind_method(D_METHOD("create_math"), &MossRuntime::create_math);
    ClassDB::bind_method(D_METHOD("create_timer", "duration_seconds", "repeating"), &MossRuntime::create_timer, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("create_json"), &MossRuntime::create_json);
    ClassDB::bind_method(D_METHOD("get_subsystem_status"), &MossRuntime::get_subsystem_status);
    ClassDB::bind_method(D_METHOD("get_diagnostics"), &MossRuntime::get_diagnostics);
}

String MossRuntime::get_version() const { return "0.2.0"; }
String MossRuntime::get_native_backend() const { return "Moss C++17"; }

Ref<MossMath> MossRuntime::create_math() const {
    Ref<MossMath> result;
    result.instantiate();
    return result;
}

Ref<MossTimer> MossRuntime::create_timer(double duration_seconds, bool repeating) const {
    Ref<MossTimer> result;
    result.instantiate();
    result->configure(duration_seconds, repeating);
    return result;
}

Ref<MossJson> MossRuntime::create_json() const {
    Ref<MossJson> result;
    result.instantiate();
    return result;
}

Dictionary MossRuntime::get_subsystem_status() const {
    Dictionary result;
    Ref<MossCoreSubsystem> core; core.instantiate();
    Ref<MossPhysicsSubsystem> physics; physics.instantiate();
    Ref<MossAudioSubsystem> audio; audio.instantiate();
    Ref<MossInputSubsystem> input; input.instantiate();
    Ref<MossNavigationSubsystem> navigation; navigation.instantiate();
    Ref<MossRenderingSubsystem> rendering; rendering.instantiate();
    Ref<MossNetworkingSubsystem> networking; networking.instantiate();
    Ref<MossXRSubsystem> xr; xr.instantiate();

    MossSubsystem *subsystems[] = { core.ptr(), physics.ptr(), audio.ptr(), input.ptr(), navigation.ptr(), rendering.ptr(), networking.ptr(), xr.ptr() };
    for (MossSubsystem *subsystem : subsystems) {
        Dictionary entry;
        entry["available"] = subsystem->is_available();
        entry["native_connected"] = subsystem->is_moss_native_backend_connected();
        entry["status"] = subsystem->get_integration_status();
        result[subsystem->get_subsystem_name()] = entry;
    }
    return result;
}

Dictionary MossRuntime::get_diagnostics() const {
    Dictionary result;
    result["integration"] = "MossGodot";
    result["integration_version"] = get_version();
    result["framework"] = get_native_backend();
    result["language_standard"] = "C++17";
    result["subsystems"] = get_subsystem_status();
    return result;
}
