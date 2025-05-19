#include "debug_lines.h"

#include "graphics/buffer_writer.h"
#include "graphics/line_data.h"
#include "graphics/opengl.h"
#include "utils/auto_release.h"

namespace game
{

DebugLines::DebugLines(std::span<const LineData> lines)
    : vao_{0u, [](auto vao) { ::glDeleteVertexArrays(1, &vao); }}
    , vbo_{static_cast<uint32_t>(lines.size_bytes())}
    , count_(static_cast<std::uint32_t>(lines.size()))
{
    BufferWriter writer{vbo_};
    writer.write(lines);

    ::glCreateVertexArrays(1, &vao_);
    ::glVertexArrayVertexBuffer(vao_, 0, vbo_.native_handle(), 0, sizeof(LineData));

    ::glEnableVertexArrayAttrib(vao_, 0);
    ::glEnableVertexArrayAttrib(vao_, 1);

    ::glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(LineData, position));
    ::glVertexArrayAttribFormat(vao_, 1, 3, GL_FLOAT, GL_FALSE, offsetof(LineData, colour));

    ::glVertexArrayAttribBinding(vao_, 0, 0);
    ::glVertexArrayAttribBinding(vao_, 1, 0);
}

auto DebugLines::bind() const -> void
{
    ::glBindVertexArray(vao_);
}

auto DebugLines::unbind() const -> void
{
    ::glBindVertexArray(0);
}

auto DebugLines::count() const -> std::uint32_t
{
    return count_;
}

}
