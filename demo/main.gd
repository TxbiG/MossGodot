extends Node

const SUBSYSTEMS: Array[Dictionary] = [
    {
        "name": "Physics",
        "class": "MossPhysicsSubsystem"
    },
    {
        "name": "Audio",
        "class": "MossAudioSubsystem"
    },
    {
        "name": "Input",
        "class": "MossInputSubsystem"
    },
    {
        "name": "Navigation",
        "class": "MossNavigationSubsystem"
    },
    {
        "name": "Rendering",
        "class": "MossRenderingSubsystem"
    },
    {
        "name": "Networking",
        "class": "MossNetworkingSubsystem"
    },
    {
        "name": "XR",
        "class": "MossXRSubsystem"
    }
]


func _ready() -> void:
    print("")
    print("======================================")
    print("       MossGodot Foundation")
    print("======================================")

    print("GDExtension loaded: ", ClassDB.class_exists(
        "MossSubsystem"
    ))

    print("")

    for entry in SUBSYSTEMS:
        inspect_subsystem(entry)

    print("")
    print("======================================")
    print("MossGodot initialisation complete.")
    print("======================================")


func inspect_subsystem(entry: Dictionary) -> void:
    var subsystem_class: StringName = StringName(entry["class"])

    if not ClassDB.class_exists(subsystem_class):
        push_warning("Moss subsystem class not registered: " + str(subsystem_class))
        return

    var instance: Object = ClassDB.instantiate(subsystem_class)

    if instance == null:
        push_error("Could not instantiate: " + str(subsystem_class))
        return

    var name: String = str(instance.call("get_subsystem_name"))
    var status: String = str(instance.call("get_integration_status"))
    var connected: bool = bool(instance.call("is_moss_backend_connected"))

    print("Subsystem: ", name)
    print("Backend connected: ", connected)
    print("Status: ", status)
    print("--------------------------------------")
