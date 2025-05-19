#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

#include "graphics/texture.h"
#include "graphics/vertex_data.h"

namespace game
{
/**
 * Class for writing primitives to a buffer in TLV format.
 *
 * Once data has been written to the buffer, it can be yielded to get a copy of the data. The buffer is cleared after.
 */
class TLVWriter
{
  public:
    /**
     * Yield the buffer. This will return a copy of the buffer and clear the internal buffer.
     *
     * @returns
     *   The buffer containing the TLV data.
     */
    auto yield() -> std::vector<std::byte>;

    /**
     * Write a uint32_t to the buffer.
     *
     * @param value
     *   The value to write.
     */
    auto write(std::uint32_t value) -> void;

    /**
     * Write an array of uint32_t to the buffer.
     *
     * @param value
     *   The value to write.
     */
    auto write(std::span<const std::uint32_t> value) -> void;

    /**
     * Write a string to the buffer.
     *
     * @param value
     *   The value to write.
     */
    auto write(std::string_view value) -> void;

    /**
     * Write a byte array to the buffer.
     *
     * @param value
     *   The value to write.
     */
    auto write(std::span<const std::byte> value) -> void;

    /**
     * Write a texture format to the buffer.
     *
     * @param value
     *   The value to write.
     */
    auto write(TextureFormat value) -> void;

    /**
     * Write a texture usage to the buffer.
     *
     * @param value
     *   The value to write.
     */
    auto write(TextureUsage value) -> void;

    /**
     * Write a vertex data to the buffer.
     *
     * @param value
     *   The value to write.
     */
    auto write(const VertexData &value) -> void;

    /**
     * Write an array of vertex data to the buffer.
     *
     * @param value
     *   The value to write.
     */
    auto write(std::span<const VertexData> value) -> void;

    /**
     * Write a texture description to the buffer.
     *
     * @param name
     *   The name of the texture.
     * @param width
     *   The width of the texture.
     * @param height
     *   The height of the texture.
     * @param format
     *   The format of the texture.
     * @param usage
     *   The usage of the texture.
     * @param data
     *   The data for the texture.
     */
    auto write(
        std::string_view name,
        std::uint32_t width,
        std::uint32_t height,
        TextureFormat format,
        TextureUsage usage,
        std::span<const std::byte> data) -> void;

    /**
     * Write a mesh data to the buffer.
     *
     * @param name
     *   The name of the mesh.
     * @param vertices
     *   The vertices of the mesh.
     * @param indices
     *   The indices of the mesh.
     */
    auto write(std::string_view name, std::span<const VertexData> vertices, std::span<const std::uint32_t> indices)
        -> void;

  private:
    /** The buffer to write to. */
    std::vector<std::byte> buffer_;
};
}
