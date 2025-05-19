#pragma once

#include <cstdint>

#include "graphics/opengl.h"
#include "graphics/texture.h"
#include "utils/auto_release.h"

namespace game
{

/**
 * Class representing a frame buffer object. Used as a rendering target.
 *
 * Has both a colour and depth texture, the colour texture can be exposed as a Texture object.
 */
class FrameBuffer
{
  public:
    /**
     * * Construct a new FrameBuffer object.
     *
     * @param width
     *   The width of the frame buffer.
     * @param height
     *   The height of the frame buffer.
     */
    FrameBuffer(std::uint32_t width, std::uint32_t height);

    /**
     * Get the native OpenGL frame buffer handle.
     *
     * @returns
     *   The native OpenGL frame buffer handle.
     */
    auto native_handle() const -> ::GLuint;

    /**
     * Bind the frame buffer for rendering.
     */
    auto bind() const -> void;

    /**
     * Unbind the frame buffer.
     */
    auto unbind() const -> void;

    /**
     * Get the width of the frame buffer.
     *
     * @returns
     *   The width of the frame buffer.
     */
    auto width() const -> std::uint32_t;

    /**
     * Get the height of the frame buffer.
     *
     * @returns
     *   The height of the frame buffer.
     */
    auto height() const -> std::uint32_t;

    /**
     * Get the colour texture of the frame buffer.
     *
     * @returns
     *   The colour texture of the frame buffer.
     */
    auto colour_texture() const -> const Texture &;

  private:
    /** OpenGL frame buffer handle. */
    AutoRelease<::GLuint> handle_;

    /** Width of the frame buffer. */
    std::uint32_t width_;

    /** Height of the frame buffer. */
    std::uint32_t height_;

    /** Colour texture of the frame buffer. */
    Texture colour_texture_;

    /** Depth texture of the frame buffer. */
    Texture depth_texture_;
};

}
