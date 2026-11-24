@tool
extends EditorPlugin

var dock: VBoxContainer
var status_label: Label
var refresh_button: Button

func _enter_tree() -> void:
    dock = VBoxContainer.new()
    dock.name = "Moss Physics"
    var heading := Label.new()
    heading.text = "Moss Physics"
    heading.add_theme_font_size_override("font_size", 16)
    dock.add_child(heading)

    status_label = Label.new()
    status_label.text = "Open a scene containing MossPhysicsWorld2D or MossPhysicsWorld3D."
    status_label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
    dock.add_child(status_label)

    refresh_button = Button.new()
    refresh_button.text = "Refresh diagnostics"
    refresh_button.pressed.connect(_refresh_status)
    dock.add_child(refresh_button)

    add_control_to_dock(DOCK_SLOT_RIGHT_UL, dock)
    _refresh_status()

func _exit_tree() -> void:
    if is_instance_valid(dock):
        remove_control_from_docks(dock)
        dock.queue_free()

func _refresh_status() -> void:
    if not is_instance_valid(status_label):
        return
    var root := EditorInterface.get_edited_scene_root()
    if root == null:
        status_label.text = "No edited scene is open."
        return

    var worlds: Array[Node] = []
    _collect_worlds(root, worlds)
    if worlds.is_empty():
        status_label.text = "No Moss physics world found in the edited scene."
        return

    var lines: PackedStringArray = []
    for world in worlds:
        var active := false
        if world.has_method("is_backend_active"):
            active = world.is_backend_active()
        lines.append("%s: %s" % [world.get_path(), "active" if active else "not running (enter Play mode)"])
    status_label.text = "\n".join(lines)

func _collect_worlds(node: Node, output: Array[Node]) -> void:
    if node is MossPhysicsWorld3D or node is MossPhysicsWorld2D:
        output.append(node)
    for child in node.get_children():
        _collect_worlds(child, output)
