#include "moss_subsystem.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void MossSubsystem::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_subsystem_name"), &MossSubsystem::get_subsystem_name);
    ClassDB::bind_method(D_METHOD("is_available"), &MossSubsystem::is_available);
    ClassDB::bind_method(D_METHOD("is_moss_native_backend_connected"), &MossSubsystem::is_moss_native_backend_connected);
    ClassDB::bind_method(D_METHOD("get_integration_status"), &MossSubsystem::get_integration_status);
}

StringName MossSubsystem::get_subsystem_name() const { return StringName("Unspecified"); }
bool MossSubsystem::is_available() const { return false; }
bool MossSubsystem::is_moss_native_backend_connected() const { return false; }
String MossSubsystem::get_integration_status() const { return "No subsystem implementation has been selected."; }
