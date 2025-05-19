#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/Core/Color.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

#include "maths/colour.h"
#include "maths/quaternion.h"
#include "maths/vector3.h"
#include "physics/rigid_body.h"

namespace game
{

// functions to convert to/from game/jolt types

auto to_native(::JPH::RVec3Arg v) -> Vector3;

auto to_native(::JPH::QuatArg &v) -> Quaternion;

auto to_native(::JPH::ColorArg c) -> Colour;

auto to_jolt(const Vector3 &v) -> ::JPH::RVec3;

auto to_jolt(RigidBodyType type) -> ::JPH::ObjectLayer;

}
