extends Node3D

func _ready() -> void:
    var world := MossPhysicsWorld3D.new()
    world.name = "MossWorld3D"
    world.gravity = Vector3(0, -9.81, 0)
    world.fixed_fps = 60
    add_child(world)

    var ground_shape := MossBoxShape3D.new()
    ground_shape.size = Vector3(20, 1, 20)
    var ground := MossRigidBody3D.new()
    ground.name = "Ground"
    ground.body_mode = 0 # Static
    ground.shape = ground_shape
    ground.position = Vector3(0, -0.5, 0)
    world.add_child(ground)

    var ball_shape := MossSphereShape3D.new()
    ball_shape.radius = 0.5
    for index in range(5):
        var ball := MossRigidBody3D.new()
        ball.name = "MossBall_%02d" % index
        ball.body_mode = 2 # Dynamic
        ball.shape = ball_shape
        ball.mass = 1.0
        ball.position = Vector3(0, 1.5 + index * 1.2, 0)
        world.add_child(ball)
        ball.body_entered.connect(_on_body_entered.bind(ball))

func _on_body_entered(other: Node, self_body: Node) -> void:
    print("Moss contact: %s with %s" % [self_body.name, other.name])
