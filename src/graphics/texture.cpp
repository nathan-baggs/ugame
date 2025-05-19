#include "graphics/texture.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <format>
#include <memory>
#include <print>
#include <ranges>
#include <span>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "third_party/opengl/glext.h"
#include <stb_image.h>

#include "graphics/opengl.h"
#include "graphics/sampler.h"
#include "tlv/tlv_reader.h"
#include "utils/error.h"
#include "utils/exception.h"
#include "utils/formatter.h"
#include "utils/log.h"

namespace
{

/**
 * Helper function to convert a TextureUsage and TextureFormat to an OpenGL format.
 *
 * @param usage
 *   The usage of the texture.
 * @param format
 *   The format of the texture.
 *
 *  @returns
 *  The OpenGL format.
 */
::GLenum to_opengl(game::TextureUsage usage, game::TextureFormat format)
{
    switch (usage)
    {
        using enum game::TextureUsage;
        using enum game::TextureFormat;

        case SRGB: return format == RGBA ? GL_SRGB8_ALPHA8 : GL_SRGB8;
        case DATA: return format == RGBA ? GL_RGBA8 : GL_RGB8;
        default: throw game::Exception("unknown usage");
    }
}

}
namespace game
{

Texture::Texture(
    TextureUsage usage,
    std::span<const std::byte> data,
    std::uint32_t width,
    std::uint32_t height,
    const Sampler *sampler)
    : handle_{0u, [](auto texture) { ::glDeleteTextures(1u, &texture); }}
    , sampler_(sampler)
{
    log::info("creating tex with: {}x{} usage={} data={}", width, height, to_string(usage), data.size());

    TextureUsage valid_usage[] = {TextureUsage::SRGB, TextureUsage::DATA};
    expect(std::ranges::contains(valid_usage, usage), "invalid usage");
    auto w = int{};
    auto h = int{};
    auto num_channels = int{};

    auto raw_data = std::unique_ptr<::stbi_uc, void (*)(void *)>(
        ::stbi_load_from_memory(
            reinterpret_cast<const stbi_uc *>(data.data()), static_cast<int>(data.size()), &w, &h, &num_channels, 0),
        ::stbi_image_free);

    ensure(raw_data, "could not parse texture data");
    ensure(static_cast<std::uint32_t>(w) == width, "width has changed");
    ensure(static_cast<std::uint32_t>(h) == height, "height has changed");

    ::glCreateTextures(GL_TEXTURE_2D, 1, &handle_);

    auto format = ::GLenum{};

    switch (usage)
    {
        using enum TextureUsage;
        case SRGB: format = num_channels == 4 ? GL_SRGB8_ALPHA8 : GL_SRGB8; break;
        case DATA: format = num_channels == 4 ? GL_RGBA8 : GL_RGB8; break;
        default: break;
    }

    ::glTextureStorage2D(handle_, 1, format, width, height);
    ::glTextureSubImage2D(
        handle_, 0, 0, 0, width, height, num_channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, raw_data.get());
}

Texture::Texture(const TextureDescription &description, const Sampler *sampler)
    : handle_{0u, [](auto texture) { ::glDeleteTextures(1u, &texture); }}
    , sampler_(sampler)
{
    log::info("creating tex with: {}", description);

    ::glCreateTextures(GL_TEXTURE_2D, 1, &handle_);

    ::glTextureStorage2D(
        handle_, 1, to_opengl(description.usage, description.format), description.width, description.height);
    ::glTextureSubImage2D(
        handle_,
        0,
        0,
        0,
        description.width,
        description.height,
        description.format == TextureFormat::RGBA ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE,
        description.data.data());
}

Texture::Texture(const TLVReader &reader, std::string_view name, const Sampler *sampler)
    : handle_{0u, [](auto texture) { ::glDeleteTextures(1u, &texture); }}
    , sampler_(sampler)
{
    auto desc = std::ranges::find_if(reader, [name](const auto &e) { return e.is_texture(name); });
    ensure(desc != std::ranges::end(reader), "could not find texture");

    // bit gross but we can reuse the other ctor
    auto tex = Texture{(*desc).texture_description_value(), sampler_};
    std::ranges::swap(handle_, tex.handle_);
}

Texture::Texture(TextureUsage usage, std::uint32_t width, std::uint32_t height)
    : handle_{0u, [](auto texture) { ::glDeleteTextures(1u, &texture); }}
{
    TextureUsage valid_usage[] = {TextureUsage::FRAMEBUFFER, TextureUsage::DEPTH};
    expect(std::ranges::contains(valid_usage, usage), "invalid usage");

    ::glCreateTextures(GL_TEXTURE_2D, 1, &handle_);
    switch (usage)
    {
        using enum TextureUsage;
        case FRAMEBUFFER: ::glTextureStorage2D(handle_, 1, GL_RGB16F, width, height); break;
        case DEPTH: ::glTextureStorage2D(handle_, 1, GL_DEPTH_COMPONENT24, width, height); break;
        default: break;
    }
}

auto Texture::native_handle() const -> ::GLuint
{
    return handle_;
}

auto Texture::sampler() const -> const Sampler *
{
    return sampler_;
}

auto to_string(TextureUsage obj) -> std::string
{
    switch (obj)
    {
        using enum TextureUsage;
        case FRAMEBUFFER: return "FRAMEBUFFER";
        case DEPTH: return "DEPTH";
        case SRGB: return "SRGB";
        case DATA: return "DATA";
    }
    return "UNKNOWN";
}

auto to_string(TextureFormat obj) -> std::string
{
    switch (obj)
    {
        using enum TextureFormat;
        case RGB: return "RGB";
        case RGBA: return "RGBA";
    }
    return "UNKNOWN";
}

auto to_string(const TextureDescription &obj) -> std::string
{
    return std::format(
        "width={} height={} format={} usage={} data={}", obj.width, obj.height, obj.format, obj.usage, obj.data.size());
}
}
