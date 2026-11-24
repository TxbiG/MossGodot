extends Node2D

func _ready() -> void:
    var world := MossPhysicsWorld2D.new()
    world.name = "MossWorld2D"
    world.gravity = Vector2(0, 980)
    add_child(world)

    var floor_shape := MossRectangleShape2D.new()
    floor_shape.size = Vector2(800, 40)
    var floor := MossRigidBody2D.new()
    floor.name = "Floor"
    floor.body_mode = 0
    floor.shape = floor_shape
    floor.position = Vector2(640, 650)
    world.add_child(floor)

    for index in range(8):
        var shape := MossCircleShape2D.new()
        shape.radius = 18
        var ball := MossRigidBody2D.new()
        ball.name = "MossBall2D_%02d" % index
        ball.body_mode = 2
        ball.shape = shape
        ball.position = Vector2(500 + index * 35, 100 + index * 12)
        world.add_child(ball)
