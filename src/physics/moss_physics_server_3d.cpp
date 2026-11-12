#include "moss_physics_server_3d.h"

using namespace godot;

void MossPhysicsServer3D::_bind_methods()
{
    // The server is deliberately not exposed to Godot yet.
    //
    // Implement and validate the required server callbacks,
    // direct-body-state interface and direct-space-state
    // interface before registration.
}

String MossPhysicsServer3D::get_integration_status() const
{
    return String(
        "Not operational. Moss physics server callbacks "
        "and native simulation integration are incomplete."
    );
}
