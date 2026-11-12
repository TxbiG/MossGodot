
#include "moss_subsystem.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void MossSubsystem::_bind_methods()
{
    ClassDB::bind_method(
        D_METHOD("get_subsystem_name"),
        &MossSubsystem::get_subsystem_name
    );

    ClassDB::bind_method(
        D_METHOD("get_integration_status"),
        &MossSubsystem::get_integration_status
    );

    ClassDB::bind_method(
        D_METHOD("is_moss_backend_connected"),
        &MossSubsystem::is_moss_backend_connected
    );
}

StringName MossSubsystem::get_subsystem_name() const
{
    return StringName("Unspecified");
}

String MossSubsystem::get_integration_status() const
{
    return String(
        "Scaffold only. The Moss native backend is not connected yet."
    );
}

bool MossSubsystem::is_moss_backend_connected() const
{
    return false;
}
