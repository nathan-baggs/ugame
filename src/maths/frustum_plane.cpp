#include "maths/frustum_plane.h"

#include <cmath>
#include <format>
#include <string>

#include "maths/vector3.h"

namespace game
{

FrustumPlane::FrustumPlane(float a, float b, float c, float d)
{
    const auto inv_length = 1.0f / std::hypot(a, b, c);
    normal = Vector3(a * inv_length, b * inv_length, c * inv_length);
    distance = d * inv_length;
}

auto to_string(const FrustumPlane &obj) -> std::string
{
    return std::format("normal=[{}] distance={}", obj.normal, obj.distance);
}

}
