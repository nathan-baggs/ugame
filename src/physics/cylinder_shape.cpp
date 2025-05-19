#include "physics/cylinder_shape.h"

#include <format>
#include <string>

#include <Jolt/Jolt.h>

#include <Jolt/Core/Reference.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>

#include "maths/vector3.h"
#include "physics/jolt_utils.h"
#include "physics/shape.h"
#include "utils/error.h"
#include "utils/pass_key.h"

namespace game
{

CylinderShape::CylinderShape(float half_height, float radius, PassKey<PhysicsSystem> pass_key)
    : Shape(pass_key)
    , half_height_{half_height}
    , radius_{radius}
    , cylinder_shape_settings_{half_height_, radius_}
{
    cylinder_shape_settings_.SetEmbedded();
}

auto CylinderShape::half_height() const -> float
{
    return half_height_;
}

auto CylinderShape::radius() const -> float
{
    return radius_;
}

auto CylinderShape::native_handle() const -> const ::JPH::ShapeSettings *
{
    return std::addressof(cylinder_shape_settings_);
}

auto CylinderShape::to_string() const -> std::string
{
    return std::format("CylinderShape : {} {}", half_height(), radius());
}
}
