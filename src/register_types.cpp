#include "register_types.h"

#include "subsystems/moss_subsystem.h"
#include "subsystems/moss_subsystems.h"

#include <godot_cpp/godot.hpp>

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
