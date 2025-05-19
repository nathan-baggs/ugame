#pragma once

#include <cstddef>
#include <span>

#include "graphics/buffer.h"

namespace game
{

/**
 * Helper class to write data to a buffer. Use as a non-owning writable view over a buffer.
 *
 * The writer starts at the beginning of the buffer and writes data sequentially.
 */
class BufferWriter
{
  public:
    /**
     * Construct a new BufferWriter object.
     *
     * @param buffer
     *  The buffer to write to.
     */
    BufferWriter(const Buffer &buffer)
        : buffer_(buffer)
        , offset_{}
    {
    }

    /**
     * Write an object to the buffer. Writing over the end of the buffer is undefined.
     *
     * @param obj
     *   The object to write to the buffer.
     */
    template <class T>
    auto write(const T &obj) -> void
        requires(std::is_trivially_copyable_v<T>)
    {
        auto spn = std::span<const T>{&obj, 1};
        write(spn);
    }

    /**
     * Write an array of objects to the buffer. Writing over the end of the buffer is undefined.
     *
     * @param data
     *   The array of objects to write to the buffer.
     */
    template <class T, std::size_t N>
    auto write(const T (&data)[N]) -> void
    {
        buffer_.write({reinterpret_cast<const std::byte *>(data), sizeof(T) * N}, offset_);
        offset_ += sizeof(T) * N;
    }

    /**
     * Write a span of objects to the buffer. Writing over the end of the buffer is undefined.
     *
     * @param data
     *  The span of objects to write to the buffer.
     */
    template <class T>
    auto write(std::span<const T> data) -> void
    {
        buffer_.write(std::as_bytes(data), offset_);
        offset_ += data.size_bytes();
    }

  private:
    /** Buffer to write to. */
    const Buffer &buffer_;

    /** Offset in the buffer to write to. */
    std::size_t offset_;
};

}
