#include <array>
#include <cmath>
#include <print>

#include <gtest/gtest.h>

#include "graphics/shape_wireframe_renderer.h"
#include "maths/frustum_plane.h"
#include "maths/vector3.h"

// TEST(shape_wireframe_renderer, simple)
// {
//     const auto d = std::hypot(1.0f, 0.5f);
//
//     const auto planes = std::array<game::FrustumPlane, 6u>{{
//         {.normal = {0.0f, 0.0f, -1.0f}, .distance = 1.0f},
//         {.normal = {0.0f, 0.0f, 1.0f}, .distance = 2.0f},
//         {.normal = {1.0f, 0.0f, 0.0f}, .distance = d},
//         {.normal = {-1.0f, 0.0f, 0.0f}, .distance = d},
//         {.normal = {0.0f, 1.0f, 0.0f}, .distance = d},
//         {.normal = {0.0f, -1.0f, 0.0f}, .distance = d},
//     }};
//
//     auto renderer = game::ShapeWireframeRenderer{};
//
//     renderer.draw(planes);
//
//     const auto lines = renderer.yield();
//
//     for (const auto &line : lines)
//     {
//         std::println("{}", line);
//     }
// }
