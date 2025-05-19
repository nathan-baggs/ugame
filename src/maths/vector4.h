#pragma once

#include <cmath>
#include <format>

namespace game
{

struct Vector4
{
    constexpr Vector4()
        : Vector4(0.0f)
    {
    }

    constexpr Vector4(float xyzw)
        : Vector4(xyzw, xyzw, xyzw, xyzw)
    {
    }

    constexpr Vector4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {
    }

    static constexpr auto dot(const Vector4 &v1, const Vector4 &v2) -> float
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    constexpr auto operator==(const Vector4 &) const -> bool = default;

    auto to_string() const -> std::string;

    float x;
    float y;
    float z;
    float w;
};

constexpr auto operator+=(Vector4 &v1, const Vector4 &v2) -> Vector4 &
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;

    return v1;
}

constexpr auto operator+(const Vector4 &v1, const Vector4 &v2) -> Vector4
{
    auto tmp = v1;
    return tmp += v2;
}

constexpr auto operator-=(Vector4 &v1, const Vector4 &v2) -> Vector4 &
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;

    return v1;
}

constexpr auto operator-(const Vector4 &v1, const Vector4 &v2) -> Vector4
{
    auto tmp = v1;
    return tmp -= v2;
}

inline auto Vector4::to_string() const -> std::string
{
    return std::format("x={} y={} z={} w={}", x, y, z, w);
}
}
