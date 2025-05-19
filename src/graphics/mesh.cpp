#include "graphics/mesh.h"

#include <ranges>

#include "buffer_writer.h"
#include "graphics/buffer.h"
#include "graphics/opengl.h"
#include "graphics/vertex_data.h"
#include "tlv/tlv_reader.h"
#include "utils/auto_release.h"
#include "utils/error.h"

namespace game
{

Mesh::Mesh(const MeshData &data)
    : vao_{0u, [](auto vao) { ::glDeleteVertexArrays(1, &vao); }}
    , vbo_{static_cast<uint32_t>(data.vertices.size_bytes() + data.indices.size_bytes())}
    , index_count_(static_cast<std::uint32_t>(data.indices.size()))
    , index_offset_(data.vertices.size_bytes())
{
    // write the vertex data and then the index data to the buffer
    BufferWriter writer{vbo_};
    writer.write(data.vertices);
    writer.write(data.indices);

    ::glCreateVertexArrays(1, &vao_);
    ::glVertexArrayVertexBuffer(vao_, 0, vbo_.native_handle(), 0, sizeof(VertexData));
    ::glVertexArrayElementBuffer(vao_, vbo_.native_handle());

    // hard coded vertex layout

    ::glEnableVertexArrayAttrib(vao_, 0);
    ::glEnableVertexArrayAttrib(vao_, 1);
    ::glEnableVertexArrayAttrib(vao_, 2);
    ::glEnableVertexArrayAttrib(vao_, 3);

    ::glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, position));
    ::glVertexArrayAttribFormat(vao_, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, normal));
    ::glVertexArrayAttribFormat(vao_, 2, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, tangent));
    ::glVertexArrayAttribFormat(vao_, 3, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, uv));

    ::glVertexArrayAttribBinding(vao_, 0, 0);
    ::glVertexArrayAttribBinding(vao_, 1, 0);
    ::glVertexArrayAttribBinding(vao_, 2, 0);
    ::glVertexArrayAttribBinding(vao_, 3, 0);
}

Mesh::Mesh(const TLVReader &reader, std::string_view name)
    : vao_{0u, [](auto vao) { ::glDeleteVertexArrays(1, &vao); }}
    , vbo_{1u}
    , index_count_{}
    , index_offset_{}
{
    auto mesh_data = std::ranges::find_if(reader, [name](const auto &e) { return e.is_mesh(name); });
    ensure(mesh_data != std::ranges::end(reader), "could not find mesh");

    // bit of a hack but we can use the other constructor to do all the opengl setup and the just "steal" its members
    auto mesh = Mesh{(*mesh_data).mesh_value()};

    std::ranges::swap(vao_, mesh.vao_);
    std::ranges::swap(vbo_, mesh.vbo_);
    std::ranges::swap(index_count_, mesh.index_count_);
    std::ranges::swap(index_offset_, mesh.index_offset_);
}

auto Mesh::bind() const -> void
{
    ::glBindVertexArray(vao_);
}

auto Mesh::unbind() const -> void
{
    ::glBindVertexArray(0);
}

auto Mesh::index_count() const -> std::uint32_t
{
    return index_count_;
}

auto Mesh::index_offset() const -> std::uintptr_t
{
    return index_offset_;
}

}
