#include "physics/sphere_shape.h"

#include <format>
#include <string>

#include <Jolt/Jolt.h>

#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include "physics/shape.h"
#include "utils/pass_key.h"

namespace game
{

SphereShape::SphereShape(float radius, PassKey<PhysicsSystem> pass_key)
    : Shape(pass_key)
    , radius_{radius}
    , sphere_shape_settings_{radius_}
{
    sphere_shape_settings_.SetEmbedded();
}

auto SphereShape::radius() const -> float
{
    return radius_;
}

auto SphereShape::native_handle() const -> const ::JPH::ShapeSettings *
{
    return std::addressof(sphere_shape_settings_);
}

auto SphereShape::to_string() const -> std::string
{
    return std::format("SphereShape : {}", radius());
}

}
