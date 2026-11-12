
#pragma once

#include <godot_cpp/classes/physics_server_3d_extension.hpp>

/**
 * MossGodot 3D physics server foundation.
 *
 * This class is not yet a functional physics backend.
 *
 * Required Godot PhysicsServer3DExtension operations must be
 * implemented and connected to the verified Moss physics API
 * before this class can be instantiated or registered as the
 * active Godot physics server.
 */
class MossPhysicsServer3D : public godot::PhysicsServer3DExtension {
    GDCLASS(MossPhysicsServer3D, godot::PhysicsServer3DExtension)

protected:
    static void _bind_methods();

public:
    MossPhysicsServer3D() = default;
    ~MossPhysicsServer3D() override = default;

    godot::String get_integration_status() const;
};
