#pragma once

#include <span>
#include <string_view>
#include <vector>

#if !defined(JPH_DEBUG_RENDERER)
#define JPH_DEBUG_RENDERER
#endif
#include <Jolt/Jolt.h>

#include <Jolt/Core/Color.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include <Jolt/Renderer/DebugRendererSimple.h>

#include "graphics/line_data.h"
#include "utils/pass_key.h"

namespace game
{

class PhysicsSystem;

/**
 * A debug renderer that uses Jolt's debug renderer. It is used to render debug information in the game.
 */
class DebugRenderer : public ::JPH::DebugRendererSimple
{
  public:
    /**
     * Construct a new debug renderer.
     *
     * @param pass_key
     *   A pass key to allow construction only from the PhysicsSystem.
     */
    DebugRenderer(PassKey<PhysicsSystem>);

    // overloads for Jolt's debug renderer functions

    virtual void DrawLine(::JPH::RVec3Arg from, ::JPH::RVec3Arg to, ::JPH::ColorArg colour) override;

    virtual void DrawTriangle(
        ::JPH::RVec3Arg v1,
        ::JPH::RVec3Arg v2,
        ::JPH::RVec3Arg v3,
        ::JPH::ColorArg colour,
        ECastShadow) override;

    virtual void DrawText3D(::JPH::RVec3Arg, const std::string_view &, ::JPH::ColorArg, float) override;

    /**
     * Clear the debug renderer.
     */
    auto clear() -> void;

    /**
     * Get the lines drawn by the debug renderer.
     *
     * @returns
     *   A span of the lines drawn by the debug renderer.
     */
    auto lines() const -> std::span<const LineData>;

  private:
    /** Collection of lines drawn by the debug renderer. */
    std::vector<LineData> lines_;
};
}
