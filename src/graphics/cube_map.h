#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

#include "graphics/opengl.h"
#include "utils/auto_release.h"

namespace game
{

class TLVReader;

/**
 * Class representing a cube map texture. Used for skyboxes.
 */
class CubeMap
{
  public:
    /**
     * Construct a new CubeMap object.
     *
     * @param faces
     *   The data for each face of the cube map.
     * @param width
     *   The width of each face of the cube map.
     * @param height
     *   The height of each face of the cube map.
     */
    CubeMap(std::vector<std::span<const std::byte>> faces, std::uint32_t width, std::uint32_t height);

    /**
     * Construct a new CubeMap object.
     *
     * @param reader
     *   The TLV reader to read the cube map data from.
     * @param image_names
     *   The names of the images to load for each face of the cube map.
     */
    CubeMap(const TLVReader &reader, std::array<std::string_view, 6> image_names);

    /**
     * Get the native OpenGL texture handle.
     *
     * @returns
     *   The native OpenGL texture handle.
     */
    auto native_handle() const -> ::GLuint;

  private:
    /** OpenGL texture handle. */
    AutoRelease<::GLuint> handle_;
};

}
