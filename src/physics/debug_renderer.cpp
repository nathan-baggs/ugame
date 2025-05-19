#include "debug_renderer.h"

#include <concepts>
#include <limits>
#include <vector>

#if !defined(JPH_DEBUG_RENDERER)
#define JPH_DEBUG_RENDERER
#endif
#include <Jolt/Jolt.h>

#include <Jolt/Core/Color.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include <Jolt/Renderer/DebugRendererSimple.h>

#include "maths/colour.h"
#include "maths/vector3.h"
#include "physics/jolt_utils.h"
#include "utils/log.h"

namespace game
{

DebugRenderer::DebugRenderer(PassKey<PhysicsSystem>)
{
}

void DebugRenderer::DrawLine(::JPH::RVec3Arg from, ::JPH::RVec3Arg to, ::JPH::ColorArg colour)
{
    lines_.push_back({to_native(from), to_native(colour)});
    lines_.push_back({to_native(to), to_native(colour)});
}

void DebugRenderer::DrawTriangle(
    ::JPH::RVec3Arg v1,
    ::JPH::RVec3Arg v2,
    ::JPH::RVec3Arg v3,
    ::JPH::ColorArg colour,
    ECastShadow)
{
    DrawLine(v1, v2, colour);
    DrawLine(v2, v3, colour);
    DrawLine(v3, v1, colour);
}

void DebugRenderer::DrawText3D(::JPH::RVec3Arg, const std::string_view &str, ::JPH::ColorArg, float)
{
    log::info("debug text {}", str);
}

auto DebugRenderer::clear() -> void
{
    lines_.clear();
}

auto DebugRenderer::lines() const -> std::span<const LineData>
{
    return lines_;
}

}
