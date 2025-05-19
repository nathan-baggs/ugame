#pragma once

#include <cstdint>
#include <span>

#include "graphics/buffer.h"
#include "graphics/line_data.h"
#include "graphics/opengl.h"
#include "utils/auto_release.h"

namespace game
{

/**
 * Class representing a set of debug lines. Used for rendering debug lines in the world.
 */
class DebugLines
{
  public:
    /**
     * Construct a new DebugLines object.
     *
     * @param lines
     *   The lines to render.
     */
    DebugLines(std::span<const LineData> lines);

    /**
     * Bind the buffer for rendering.
     */
    auto bind() const -> void;

    /**
     * Unbind the buffer.
     */
    auto unbind() const -> void;

    /**
     * Get the number of lines in the buffer.
     */
    auto count() const -> std::uint32_t;

  private:
    /** OpenGL vertex array object handle. */
    AutoRelease<::GLuint> vao_;

    /** OpenGL vertex buffer object handle. */
    Buffer vbo_;

    /** Number of lines in the buffer. */
    std::uint32_t count_;
};

}
