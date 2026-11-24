#pragma once

// Moss headers are global C++ APIs rather than a namespace-scoped library.
// Keep their dependency order explicit and avoid the broad Moss/Moss.h
// umbrella, which pulls in engine/platform subsystems Godot already owns.
// CMake supplies a compatibility-staged Moss_stdinc.h with the POSIX stat
// declaration included where required.

#include <Moss/Moss_stdinc.h>
#include <Moss/Variants/Vector/Float2.h>
#include <Moss/Variants/Vector/Float3.h>
#include <Moss/Variants/Vector/Float4.h>
#include <Moss/Variants/Vector/Vec4.h>
#include <Moss/Variants/Vector/UVec4.h>
#include <Moss/Variants/Vector/Vec2.h>
#include <Moss/Variants/Vector/Vec3.h>
#include <Moss/Variants/Quat.h>
#include <Moss/Resources/Timer.h>
#include <Moss/Resources/Json.h>

#if defined(MOSS_GODOT_WITH_PHYSICS)
#include <Moss/Core/JobSystemThreadPool.h>
#include <Moss/Core/TempAllocator.h>
#include <Moss/Physics/PhysicsSystem.h>
#include <Moss/Physics/Body/Body.h>
#include <Moss/Physics/Collision/BroadPhase/BroadPhase.h>
#include <Moss/Physics/Collision/Shape/Shapes.h>
#endif
