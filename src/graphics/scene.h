#pragma once

#include <optional>

#include "graphics/buffer.h"
#include "graphics/cube_map.h"
#include "graphics/debug_lines.h"
#include "graphics/line_data.h"
#include "graphics/sampler.h"
#include "maths/colour.h"
#include "maths/vector3.h"

#include <vector>

namespace game
{

class Entity;

/**
 * A light that shines in a single direction.
 *
 * This is typically used for directional lights like the sun.
 */
struct DirectionalLight
{
    /** The direction of the light. */
    Vector3 direction;

    /** The colour of the light. */
    Colour colour;
};

/**
 * A light that shines in all directions from a single point.
 *
 * This is typically used for point lights like light bulbs.
 */
struct PointLight
{
    /** The position of the light. */
    Vector3 position;

    /** The colour of the light. */
    Colour colour;

    /** The attenuation of the light. */
    float const_attenuation;

    /** The linear attenuation of the light. */
    float linear_attenuation;

    /** The quadratic attenuation of the light. */
    float quad_attenuation;
};

/**
 * Collection of data that represents a renderable "scene".
 *
 * Note that the pointer members are non-owning. This means that the lifetime of the objects must outlive the scene.
 */
struct Scene
{
    /** Collection of entities to render. */
    std::vector<const Entity *> entities;

    /** The ambient light in the scene. */
    Colour ambient;

    /** The directional light in the scene. */
    DirectionalLight directional;

    /** The point lights in the scene. */
    std::vector<PointLight> points;

    /** Debug lines to draw (optional). */
    std::optional<DebugLines> debug_lines;

    /** The skybox to render. */
    const CubeMap *skybox;

    /** The skybox sampler to use. */
    const Sampler *skybox_sampler;
};

}
