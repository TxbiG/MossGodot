@tool
extends EditorPlugin

func _enter_tree() -> void:
    print("[MossGodot] Editor plugin enabled.")

    if not ClassDB.class_exists("MossSubsystem"):
        push_warning("[MossGodot] Native GDExtension classes are not loaded.")
        return

    print("[MossGodot] Native subsystem classes detected.")
    print("[MossGodot] Physics: ", ClassDB.class_exists("MossPhysicsSubsystem"))
    print("[MossGodot] Audio: ", ClassDB.class_exists("MossAudioSubsystem"))
    print("[MossGodot] Input: ", ClassDB.class_exists("MossInputSubsystem"))
    print("[MossGodot] Navigation: ", ClassDB.class_exists("MossNavigationSubsystem"))
    print("[MossGodot] Rendering: ", ClassDB.class_exists("MossRenderingSubsystem"))
    print("[MossGodot] Networking: ", ClassDB.class_exists("MossNetworkingSubsystem"))
    print("[MossGodot] XR: ", ClassDB.class_exists("MossXRSubsystem"))

func _exit_tree() -> void:
    print("[MossGodot] Editor plugin disabled.")
