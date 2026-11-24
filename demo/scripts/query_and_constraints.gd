extends Node3D

# Attach this script to a Node3D and run it after building the GDExtension.
# The two rigid bodies and the joint are owned by the explicit Moss world.
func _ready() -> void:
    var world := MossPhysicsWorld3D.new()
    world.name = "MossWorld"
    add_child(world)

    var ground_shape := MossBoxShape3D.new()
    ground_shape.size = Vector3(20.0, 1.0, 20.0)
    var ground := MossRigidBody3D.new()
    ground.name = "Ground"
    ground.body_mode = 0 # Static
    ground.shape = ground_shape
    ground.position = Vector3(0.0, -0.5, 0.0)
    world.add_child(ground)

    var sphere_shape := MossSphereShape3D.new()
    sphere_shape.radius = 0.5
    var body_a := MossRigidBody3D.new()
    body_a.name = "BodyA"
    body_a.body_mode = 2 # Dynamic
    body_a.shape = sphere_shape
    body_a.position = Vector3(0.0, 2.0, 0.0)
    world.add_child(body_a)

    var body_b := MossRigidBody3D.new()
    body_b.name = "BodyB"
    body_b.body_mode = 2
    body_b.shape = sphere_shape
    body_b.position = Vector3(0.0, 4.0, 0.0)
    world.add_child(body_b)

    var joint := MossDistanceJoint3D.new()
    joint.name = "BodyDistance"
    joint.body_a = NodePath("../BodyA")
    joint.body_b = NodePath("../BodyB")
    joint.min_distance = 1.0
    joint.max_distance = 2.0
    world.add_child(joint)

    await get_tree().physics_frame
    var hits := world.ray_cast_all(Vector3(0.0, 8.0, 0.0), Vector3(0.0, -8.0, 0.0))
    print("Moss ray hits: ", hits)

    var overlap := world.intersect_shape(sphere_shape, Transform3D(Basis.IDENTITY, Vector3(0.0, 2.0, 0.0)))
    print("Moss shape overlaps: ", overlap)
