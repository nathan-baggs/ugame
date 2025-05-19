#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "graphics/opengl.h"
#include "utils/auto_release.h"

namespace game
{

class TLVReader;
class Sampler;

/**
 * Enumeration of possible texture usages.
 */
enum class TextureUsage
{
    FRAMEBUFFER,
    DEPTH,
    SRGB,
    DATA
};

/**
 * Enumeration of possible texture formats.
 */
enum class TextureFormat
{
    RGB,
    RGBA
};

/**
 * A description of a texture to load.
 */
struct TextureDescription
{
    /** Width of the texture. */
    std::uint32_t width;

    /** Height of the texture. */
    std::uint32_t height;

    /** Format of the texture. */
    TextureFormat format;

    /** Usage of the texture. */
    TextureUsage usage;

    /** The raw pixel data of the texture. */
    std::vector<std::byte> data;
};

/**
 * Represents a texture in OpenGL. Textures store a non-owning pointer to their sampler.
 */
class Texture
{
  public:
    /**
     * Constructs a texture from raw data. Calling with a usage that is not SRGB or DATA is undefined behaviour.
     *
     * @param usage
     *   The usage of the texture.
     * @param data
     *   The raw pixel data of the texture.
     * @param width
     *   The width of the texture.
     * @param height
     *   The height of the texture.
     * @param sampler
     *  The sampler to use for the texture.
     */
    Texture(
        TextureUsage usage,
        std::span<const std::byte> data,
        std::uint32_t width,
        std::uint32_t height,
        const Sampler *sampler);

    /**
     * Constructs a texture from a description.
     *
     * @param description
     *   The description of the texture.
     * @param sampler
     *   The sampler to use for the texture.
     */
    Texture(const TextureDescription &description, const Sampler *sampler);

    /**
     * Constructs a texture from a TLV reader.
     *
     * @param reader
     *   The TLV reader to use.
     * @param name
     *   The name of the texture in the tlv.
     * @param sampler
     *   The sampler to use for the texture.
     */
    Texture(const TLVReader &reader, std::string_view name, const Sampler *sampler);

    /**
     * Constructs a texture with the given usage, width and height. Note this texture is not initialized. Calling with a
     * usage that is not FRAMEBUFFER or DEPTH is undefined behaviour.
     *
     * @param usage
     *   The usage of the texture.
     * @param width
     *   The width of the texture.
     * @param height
     *   The height of the texture.
     */
    Texture(TextureUsage usage, std::uint32_t width, std::uint32_t height);

    /**
     * Gets the OpenGL handle of the texture.
     *
     * @returns
     *   The OpenGL handle of the texture.
     */
    auto native_handle() const -> ::GLuint;

    /**
     * Gets the sampler of the texture.
     *
     * @returns
     *   The sampler of the texture.
     */
    auto sampler() const -> const Sampler *;

  private:
    /** OpenGL handle of the texture. */
    AutoRelease<::GLuint> handle_;

    /** Sampler of the texture. */
    const Sampler *sampler_;
};

/**
 * Converts a texture usage to a string.
 *
 * @param obj
 *   The texture usage to convert.
 *
 * @returns
 *   The string representation of the texture usage.
 */
auto to_string(TextureUsage obj) -> std::string;

/**
 * Converts a texture format to a string.
 *
 * @param obj
 *   The texture format to convert.
 *
 * @returns
 *   The string representation of the texture format.
 */
auto to_string(TextureFormat obj) -> std::string;

/**
 * Converts a texture description to a string.
 *
 * @param obj
 *   The texture description to convert.
 *
 * @returns
 *   The string representation of the texture description.
 */
auto to_string(const TextureDescription &obj) -> std::string;
}
