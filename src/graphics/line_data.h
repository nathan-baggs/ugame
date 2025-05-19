#pragma once

#include <format>
#include <string>

#include "maths/colour.h"
#include "maths/vector3.h"
#include "utils/formatter.h"

namespace game
{

/**
 * Struct representing one end of a line in 3D space, with a colour.
 */
struct LineData
{
    /** Position of the line end in 3D space. */
    Vector3 position;

    /** Colour of the line. */
    Colour colour;
};

/**
 * Format a LineData object as a string.
 *
 * @param obj
 *   The LineData object to format.
 *
 * @returns
 *   The formatted string.
 */
inline auto to_string(const game::LineData &obj) -> std::string
{
    return std::format("p=[{}] c=[{}]", obj.position, obj.colour);
}

}
