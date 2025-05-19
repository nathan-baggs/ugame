#include "graphics/cube_map.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <span>
#include <string_view>
#include <tuple>
#include <vector>

#include <stb_image.h>

#include "graphics/opengl.h"
#include "third_party/opengl/glext.h"
#include "tlv/tlv_reader.h"
#include "utils/auto_release.h"
#include "utils/error.h"

namespace game
{
CubeMap::CubeMap(std::vector<std::span<const std::byte>> faces, std::uint32_t width, std::uint32_t height)
    : handle_{0u, [](auto texture) { ::glDeleteTextures(1u, &texture); }}
{
    ::glCreateTextures(GL_TEXTURE_CUBE_MAP, 1u, &handle_);
    ::glTextureStorage2D(handle_, 1, GL_SRGB8, width, height);

    // load each face
    for (const auto &[index, face] : std::views::enumerate(faces))
    {
        auto w = int{};
        auto h = int{};
        auto num_channels = int{};

        auto raw_data = std::unique_ptr<::stbi_uc, void (*)(void *)>(
            ::stbi_load_from_memory(
                reinterpret_cast<const stbi_uc *>(face.data()),
                static_cast<int>(face.size()),
                &w,
                &h,
                &num_channels,
                0),
            ::stbi_image_free);

        ensure(raw_data, "could not parse texture data");
        ensure(static_cast<std::uint32_t>(w) == width, "width has changed");
        ensure(static_cast<std::uint32_t>(h) == height, "height has changed");

        ::glTextureSubImage3D(
            handle_, 0, 0, 0, static_cast<::GLint>(index), width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, raw_data.get());
    }
}

CubeMap::CubeMap(const TLVReader &reader, std::array<std::string_view, 6> image_names)
    : handle_{0u, [](auto texture) { ::glDeleteTextures(1u, &texture); }}
{

    // load each face from the tlv
    const auto descs =
        image_names | std::views::transform(
                          [&reader](const auto &e)
                          {
                              const auto desc =
                                  std::ranges::find_if(reader, [e](const auto &e2) { return e2.is_texture(e); });
                              ensure(desc != std::ranges::end(reader), "cannot find image");

                              return (*desc).texture_description_value();
                          });

    const auto width = descs.front().width;
    const auto height = descs.front().height;

    // sanity check all faces have the same dimensions
    ensure(
        std::ranges::all_of(
            descs | std::views::transform([](const auto &e) { return std::make_tuple(e.width, e.height); }) |
                std::views::drop(1),
            [width, height](const auto &e) { return e == std::make_tuple(width, height); }),
        "all width and heights need to be the same");

    ::glCreateTextures(GL_TEXTURE_CUBE_MAP, 1u, &handle_);
    ::glTextureStorage2D(handle_, 1, GL_SRGB8, width, height);

    // load each face
    for (const auto &[index, desc] : std::views::enumerate(descs))
    {
        ::glTextureSubImage3D(
            handle_,
            0,
            0,
            0,
            static_cast<::GLint>(index),
            desc.width,
            desc.height,
            1,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            desc.data.data());
    }
}

auto CubeMap::native_handle() const -> ::GLuint
{
    return handle_;
}
}
