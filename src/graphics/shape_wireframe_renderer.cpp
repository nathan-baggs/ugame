#include "shape_wireframe_renderer.h"

#include <print>
#include <ranges>
#include <vector>

#include "graphics/camera.h"
#include "graphics/line_data.h"
#include "maths/aabb.h"
#include "maths/vector3.h"

namespace game
{

auto ShapeWireframeRenderer::draw(const Vector3 &start, const Vector3 &end, const Colour &colour) -> void
{
    // note that a line requries two entries, the start and end
    lines_.push_back({start, colour});
    lines_.push_back({end, colour});
}

auto ShapeWireframeRenderer::draw(const Camera &camera) -> void
{
    const auto corners = camera.frustum_corners();

    const auto near_colour = Colour{1.0f, 0.0f, 1.0f};
    const auto far_colour = Colour{1.0f, 0.0f, 0.0f};
    const auto connect_colour = Colour{1.0f, 1.0f, 0.0f};

    draw(corners[0], corners[1], near_colour);
    draw(corners[1], corners[2], near_colour);
    draw(corners[2], corners[3], near_colour);
    draw(corners[3], corners[0], near_colour);

    draw(corners[4], corners[5], far_colour);
    draw(corners[5], corners[6], far_colour);
    draw(corners[6], corners[7], far_colour);
    draw(corners[7], corners[4], far_colour);

    draw(corners[0], corners[4], connect_colour);
    draw(corners[1], corners[5], connect_colour);
    draw(corners[2], corners[6], connect_colour);
    draw(corners[3], corners[7], connect_colour);
}

auto ShapeWireframeRenderer::draw(const AABB &aabb) -> void
{
    draw({aabb.max.x, aabb.max.y, aabb.max.z}, {aabb.min.x, aabb.max.y, aabb.max.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.min.x, aabb.max.y, aabb.max.z}, {aabb.min.x, aabb.max.y, aabb.min.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.min.x, aabb.max.y, aabb.min.z}, {aabb.max.x, aabb.max.y, aabb.min.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.max.x, aabb.max.y, aabb.min.z}, {aabb.max.x, aabb.max.y, aabb.max.z}, {0.0f, 1.0f, 0.0f});

    draw({aabb.max.x, aabb.max.y, aabb.max.z}, {aabb.max.x, aabb.min.y, aabb.max.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.min.x, aabb.max.y, aabb.max.z}, {aabb.min.x, aabb.min.y, aabb.max.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.min.x, aabb.max.y, aabb.min.z}, {aabb.min.x, aabb.min.y, aabb.min.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.max.x, aabb.max.y, aabb.min.z}, {aabb.max.x, aabb.min.y, aabb.min.z}, {0.0f, 1.0f, 0.0f});

    draw({aabb.max.x, aabb.min.y, aabb.max.z}, {aabb.min.x, aabb.min.y, aabb.max.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.min.x, aabb.min.y, aabb.max.z}, {aabb.min.x, aabb.min.y, aabb.min.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.min.x, aabb.min.y, aabb.min.z}, {aabb.max.x, aabb.min.y, aabb.min.z}, {0.0f, 1.0f, 0.0f});
    draw({aabb.max.x, aabb.min.y, aabb.min.z}, {aabb.max.x, aabb.min.y, aabb.max.z}, {0.0f, 1.0f, 0.0f});
}

auto ShapeWireframeRenderer::yield() -> std::vector<LineData>
{
    auto tmp = std::vector<LineData>{};
    std::ranges::swap(tmp, lines_);
    return tmp;
}

}
