#pragma once

#include <string>

#include "maths/matrix3.h"
#include "maths/vector3.h"
#include "utils/formatter.h"

namespace game
{

/**
 * A plane in 3D space.
 */
struct FrustumPlane
{
    FrustumPlane() = default;

    /**
     * Construct a plane from the given coefficients. This normalizes the plane
     *
     * @param a
     *   The x coefficient of the plane equation.
     * @param b
     *   The y coefficient of the plane equation.
     * @param c
     *   The z coefficient of the plane equation.
     * @param d
     *   The distance from the origin to the plane.
     */
    FrustumPlane(float a, float b, float c, float d);

    /** The normal of the plane. */
    Vector3 normal;

    /** The distance from the origin to the plane. */
    float distance;
};

/**
 * Convert the given plane to a string.
 *
 * @param obj
 *   The plane to convert.
 *
 * @returns
 *   The string representation of the plane.
 */
[[nodiscard]]
auto to_string(const FrustumPlane &obj) -> std::string;

}
