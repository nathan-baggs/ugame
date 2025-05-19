#pragma once

#include <format>
#include <string>

namespace game
{

/**
 * A quaternion. This is a 4D vector used to represent rotations.
 */
class Quaternion
{
  public:
    /**
     * Construct the identity quaternion.
     */
    constexpr Quaternion()
        : Quaternion(0.0f, 0.0f, 0.0f, 1.0f)
    {
    }

    /**
     * Construct a quaternion from the given components.
     *
     * @param x
     *   The x component of the quaternion.
     * @param y
     *   The y component of the quaternion.
     * @param z
     *   The z component of the quaternion.
     * @param w
     *   The w component of the quaternion.
     */
    constexpr Quaternion(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {
    }

    /**
     * Format the quaternion as a string.
     *
     * @returns
     *   The quaternion as a string.
     */
    auto to_string() const -> std::string;

    float x;
    float y;
    float z;
    float w;
};

inline auto Quaternion::to_string() const -> std::string
{
    return std::format("x={} y={} z={} w={}", x, y, z, w);
}

}
