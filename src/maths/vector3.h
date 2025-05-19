#pragma once

#include <cmath>
#include <format>

#include "maths/vector4.h"
#include "utils/error.h"

namespace game
{

/**
 * A 3D vector class, with x, y, and z components.
 */
struct Vector3
{
    constexpr Vector3() = default;

    /**
     * Constructs a vector with all components set to the same value.
     *
     * @param xyz
     *   The value to set all components to.
     */
    constexpr Vector3(float xyz)
        : Vector3(xyz, xyz, xyz)
    {
    }

    /**
     * Constructs a vector with the specified x, y, and z components.
     *
     * @param x
     *   The x component of the vector.
     * @param y
     *   The y component of the vector.
     * @param z
     *  The z component of the vector.
     */
    constexpr Vector3(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
    }

    /**
     * Constructs a vector from a Vector4 by ignoring the w component.
     *
     * @param v
     *  The Vector4 to convert to a Vector3.
     */
    constexpr Vector3(const Vector4 &v)
        : Vector3(v.x, v.y, v.z)
    {
    }

    /**
     * Get the length of the vector.
     *
     * @returns
     *   The length of the vector.
     */
    auto length() const -> float;

    /**
     * Normalises the vector.
     *
     * Note normalising a zero vector is undefined behaviour.
     *
     * @returns
     *   The normalised vector.
     */
    static auto normalise(const Vector3 &v) -> Vector3;

    /**
     * Cross product of two vectors.
     *
     * @param v1
     *   The first vector.
     * @param v2
     *   The second vector.
     *
     * @returns
     *   The cross product of the two vectors.
     */
    static constexpr auto cross(const Vector3 &v1, const Vector3 &v2) -> Vector3;

    /**
     * Dot product of two vectors.
     *
     * @param v1
     *   The first vector.
     * @param v2
     *   The second vector.
     *
     * @returns
     *   The dot product of the two vectors.
     */
    static constexpr auto dot(const Vector3 &v1, const Vector3 &v2) -> float;

    /**
     * Distance between two vectors.
     *
     * @param v1
     *   The first vector.
     * @param v2
     *   The second vector.
     *
     * @returns
     *   The distance between the two vectors.
     */
    static auto distance(const Vector3 &v1, const Vector3 &v2) -> float;

    /**
     * Converts the vector to a string.
     *
     * @returns
     *   The string representation of the vector.
     */
    auto to_string() const -> std::string;

    constexpr auto operator==(const Vector3 &) const -> bool = default;

    /** x component. */
    float x;

    /** y component. */
    float y;

    /** z component. */
    float z;
};

// maths overloads

constexpr auto operator-=(Vector3 &v1, const Vector3 &v2) -> Vector3 &
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;

    return v1;
}

constexpr auto operator-(const Vector3 &v1, const Vector3 &v2) -> Vector3
{
    auto tmp = v1;
    return tmp -= v2;
}

constexpr auto operator+=(Vector3 &v1, const Vector3 &v2) -> Vector3 &
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;

    return v1;
}

constexpr auto operator+(const Vector3 &v1, const Vector3 &v2) -> Vector3
{
    auto tmp = v1;
    return tmp += v2;
}

constexpr auto operator*=(Vector3 &v1, const Vector3 &v2) -> Vector3 &
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;

    return v1;
}

constexpr auto operator*(const Vector3 &v1, const Vector3 &v2) -> Vector3
{
    auto tmp = v1;
    return tmp *= v2;
}

constexpr auto operator-(const Vector3 &v) -> Vector3
{
    return {-v.x, -v.y, -v.z};
}

// implementations

inline auto Vector3::length() const -> float
{
    return std::hypot(x, y, z);
}

inline auto Vector3::normalise(const Vector3 &v) -> Vector3
{
    const auto l = v.length();
    expect(l != 0.0f, "cannot normalise a zero vector");

    return {v.x / l, v.y / l, v.z / l};
}

constexpr auto Vector3::cross(const Vector3 &v1, const Vector3 &v2) -> Vector3
{
    const auto i = (v1.y * v2.z) - (v1.z * v2.y);
    const auto j = (v1.x * v2.z) - (v1.z * v2.x);
    const auto k = (v1.x * v2.y) - (v1.y * v2.x);

    return {i, -j, k};
}

constexpr auto Vector3::dot(const Vector3 &v1, const Vector3 &v2) -> float
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline auto Vector3::distance(const Vector3 &v1, const Vector3 &v2) -> float
{
    return (v2 - v1).length();
}

inline auto Vector3::to_string() const -> std::string
{
    return std::format("x={} y={} z={}", x, y, z);
}

}
