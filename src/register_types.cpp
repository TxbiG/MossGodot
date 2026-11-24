#include "register_types.h"

#include "subsystems/moss_subsystem.h"
#include "subsystems/moss_subsystems.h"
#include "nodes/moss_body_nodes.h"
#include "nodes/moss_world_nodes.h"
#include "nodes/moss_joint_nodes.h"
#include "resources/moss_shape_resources.h"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>


using namespace godot;
using namespace mossgodot;


// ------------------------------------------------------------
// Initialisation
// ------------------------------------------------------------

void initialize_moss_godot(godot::ModuleInitializationLevel level) {
    if (level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) { return; }

    // Base class.
    GDREGISTER_CLASS(MossSubsystem);

    // Physics.
    GDREGISTER_CLASS(MossPhysicsSubsystem);

    // Audio.
    GDREGISTER_CLASS(MossAudioSubsystem);

    // Input.
    GDREGISTER_CLASS(MossInputSubsystem);

    // Navigation.
    GDREGISTER_CLASS(MossNavigationSubsystem);

    // Rendering.
    GDREGISTER_CLASS(MossRenderingSubsystem);

    // Networking.
    GDREGISTER_CLASS(MossNetworkingSubsystem);

    // XR.
    GDREGISTER_CLASS(MossXRSubsystem);

    // MossPhysicsServer3D is deliberately not registered.
    // Implement the required Godot physics interfaces first.

    ClassDB::register_class<MossPhysicsWorld3D>();
    ClassDB::register_class<MossPhysicsWorld2D>();
    ClassDB::register_class<MossRigidBody3D>();
    ClassDB::register_class<MossRigidBody2D>();

    ClassDB::register_abstract_class<MossJoint3D>();
    ClassDB::register_class<MossFixedJoint3D>();
    ClassDB::register_class<MossDistanceJoint3D>();
    ClassDB::register_abstract_class<MossJoint2D>();
    ClassDB::register_class<MossFixedJoint2D>();
    ClassDB::register_class<MossDistanceJoint2D>();

    ClassDB::register_abstract_class<MossShape3D>();
    ClassDB::register_class<MossBoxShape3D>();
    ClassDB::register_class<MossSphereShape3D>();
    ClassDB::register_class<MossCapsuleShape3D>();
    ClassDB::register_class<MossMeshShape3D>();
    ClassDB::register_class<MossHeightFieldShape3D>();
    ClassDB::register_abstract_class<MossShape2D>();
    ClassDB::register_class<MossRectangleShape2D>();
    ClassDB::register_class<MossCircleShape2D>();
}

// ------------------------------------------------------------
// Shutdown
// ------------------------------------------------------------

void uninitialize_moss_godot(godot::ModuleInitializationLevel level) {
    if (level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
    {
        return;
    }

    // Future:
    // Shut down Moss worlds, audio devices, networking
    // resources and other native subsystems here.
}

// ------------------------------------------------------------
// GDExtension entry point
// ------------------------------------------------------------

extern "C" {
GDExtensionBool GDE_EXPORT moss_godot_library_init(GDExtensionInterfaceGetProcAddress get_proc_address, GDExtensionClassLibraryPtr library, GDExtensionInitialization* initialization) {
    godot::GDExtensionBinding::InitObject init_obj(get_proc_address, library, initialization);

    init_obj.register_initializer(initialize_moss_godot);
    init_obj.register_terminator(uninitialize_moss_godot);
    init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);
    return init_obj.init();
}
}
