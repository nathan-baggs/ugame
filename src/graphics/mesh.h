#pragma once

#include <cstdint>
#include <string_view>

#include "graphics/buffer.h"
#include "graphics/mesh_data.h"
#include "graphics/opengl.h"
#include "utils/auto_release.h"

namespace game
{

class TLVReader;

/**
 * This class encapsulates the GPU buffer for a renderable mesh.
 *
 * Internally the data is stored on the GPU as a collection of unique vertices and the indices that reference them. The
 * index data is stored in the same GPU buffer as the vertex data.
 */
class Mesh
{
  public:
    /**
     * Construct a new Mesh object.
     *
     * @param data
     *   The mesh data to use.
     */
    Mesh(const MeshData &data);

    /**
     * Construct a new Mesh object from a TLVReader.
     *
     * @param reader
     *   The TLVReader to use.
     * @param name
     *   The name of the mesh in the tlv.
     */
    Mesh(const TLVReader &reader, std::string_view name);

    /**
     * Bind the mesh for rendering.
     */
    auto bind() const -> void;

    /**
     * Unbind the mesh.
     */
    auto unbind() const -> void;

    /**
     * Get the number of indices in the mesh.
     *
     * @returns
     *  The number of indices in the mesh.
     */
    auto index_count() const -> std::uint32_t;

    /**
     * Get the offset of the index data into the underlying GPU buffer.
     *
     * @returns
     *  The offset of the index buffer.
     */
    auto index_offset() const -> std::uintptr_t;

  private:
    /** OpenGL vertex array object handle. */
    AutoRelease<::GLuint> vao_;

    /** OpenGL vertex buffer object handle. */
    Buffer vbo_;

    /** Number of indices in the mesh. */
    std::uint32_t index_count_;

    /** Offset of the index data into the underlying GPU buffer. */
    std::uintptr_t index_offset_;
};

}
