#include "physics/box_shape.h"

#include <format>
#include <string>

#include <Jolt/Jolt.h>

#include <Jolt/Core/Reference.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include "maths/vector3.h"
#include "physics/jolt_utils.h"
#include "physics/shape.h"
#include "utils/error.h"
#include "utils/pass_key.h"

namespace game
{

BoxShape::BoxShape(const Vector3 &dimensions, PassKey<PhysicsSystem> pass_key)
    : Shape(pass_key)
    , dimensions_{dimensions}
    , box_shape_settings_{to_jolt(dimensions_)}
{
    box_shape_settings_.SetEmbedded();
}

auto BoxShape::dimensions() const -> Vector3
{
    return dimensions_;
}

auto BoxShape::native_handle() const -> const ::JPH::ShapeSettings *
{
    return std::addressof(box_shape_settings_);
}

auto BoxShape::to_string() const -> std::string
{
    return std::format("BoxShape : {}", dimensions());
}

}
