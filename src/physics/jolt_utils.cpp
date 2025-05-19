#include "jolt_utils.h"

#include <concepts>

#include <Jolt/Jolt.h>

#include <Jolt/Core/Color.h>
#include <Jolt/Math/Real.h>

#include "maths/colour.h"
#include "maths/quaternion.h"
#include "maths/vector3.h"

namespace game
{
auto to_native(::JPH::RVec3Arg v) -> game::Vector3
{
    return {v.GetX(), v.GetY(), v.GetZ()};
}

auto to_native(::JPH::QuatArg &q) -> game::Quaternion
{
    return {q.GetX(), q.GetY(), q.GetZ(), q.GetW()};
}

auto to_native(::JPH::ColorArg c) -> Colour
{
    const auto to_float = []<std::integral T>(T b)
    { return static_cast<float>(b) / static_cast<float>(std::numeric_limits<T>::max()); };
    return {to_float(c.r), to_float(c.g), to_float(c.b)};
}

auto to_jolt(const Vector3 &v) -> ::JPH::RVec3
{
    return {v.x, v.y, v.z};
}

auto to_jolt(RigidBodyType type) -> ::JPH::ObjectLayer
{
    return static_cast<::JPH::ObjectLayer>(std::to_underlying(type));
}
}
