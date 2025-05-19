#pragma once

#include <format>

namespace game
{

/**
 * RGB Colour structure.
 */
struct Colour
{
    /** Red. */
    float r;

    /** Green. */
    float g;

    /** Blue. */
    float b;
};

/**
 * Format a Colour to a string.
 *
 * @param obj
 *   The Colour to format.
 *
 * @returns
 *   The formatted string.
 */
inline auto to_string(const Colour &obj) -> std::string
{
    return std::format("r={} g={} b={}", obj.r, obj.g, obj.b);
}

}
