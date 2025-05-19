#pragma once

#include "maths/vector3.h"

namespace game
{

/**
 * Axis aligned bounding box.
 */
struct AABB
{
    /** Minimum corner of the box in world space. */
    Vector3 min;

    /** Maximum corner of the box in world space. */
    Vector3 max;
};

}
