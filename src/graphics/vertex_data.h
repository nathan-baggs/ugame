#pragma once

#include "maths/vector3.h"

namespace game
{

/**
 * A 2D texture coordinate.
 */
struct UV
{
    float x;
    float y;
};

/**
 * Data needed to render a vertex. All shaders should assume this.
 */
struct VertexData
{
    /** The position of the vertex in model space. */
    Vector3 position;

    /** The normal of the vertex in model space. */
    Vector3 normal;

    /** The tangent of the vertex in model space. */
    Vector3 tangent;

    /** The UV coordinates of the vertex. */
    UV uv;
};

}
