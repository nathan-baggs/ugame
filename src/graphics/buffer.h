#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include "graphics/opengl.h"
#include "utils/auto_release.h"

namespace game
{

/**
 * Class representing a fixed size GPU buffer.
 */
class Buffer
{
  public:
    /**
     * Construct a new Buffer object.
     *
     * @param size
     *  The size of the buffer in bytes.
     */
    Buffer(std::uint32_t size);

    /**
     * Write data to the buffer. Writing over the end of the buffer is undefined.
     *
     * @param data
     *   The data to write to the buffer.
     * @param offset
     *   The offset in bytes to write the data to.
     */
    auto write(std::span<const std::byte> data, std::size_t offset) const -> void;

    /**
     * Get the native OpenGL buffer handle.
     *
     * @returns
     *   The native OpenGL buffer handle.
     */
    auto native_handle() const -> ::GLuint;

  private:
    /** OpenGL buffer handle. */
    AutoRelease<::GLuint> buffer_;

    /** Size of the buffer in bytes. */
    std::uint32_t size_;
};

}
