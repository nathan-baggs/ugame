#include "graphics/frame_buffer.h"

#include <cstdint>

#include "graphics/opengl.h"
#include "graphics/texture.h"
#include "third_party/opengl/glext.h"

namespace game
{
FrameBuffer::FrameBuffer(std::uint32_t width, std::uint32_t height)
    : handle_(0u, [](const auto buffer) { ::glDeleteFramebuffers(1u, &buffer); })
    , width_(width)
    , height_(height)
    , colour_texture_(TextureUsage::FRAMEBUFFER, width, height)
    , depth_texture_(TextureUsage::DEPTH, width, height)
{
    ::glCreateFramebuffers(1, &handle_);
    ::glNamedFramebufferTexture(handle_, GL_COLOR_ATTACHMENT0, colour_texture_.native_handle(), 0);
    ::glNamedFramebufferTexture(handle_, GL_DEPTH_ATTACHMENT, depth_texture_.native_handle(), 0);
}

auto FrameBuffer::native_handle() const -> ::GLuint
{
    return handle_;
}

auto FrameBuffer::bind() const -> void
{
    ::glBindFramebuffer(GL_FRAMEBUFFER, handle_);
}

auto FrameBuffer::unbind() const -> void
{
    ::glBindFramebuffer(GL_FRAMEBUFFER, 0u);
}

auto FrameBuffer::width() const -> std::uint32_t
{
    return width_;
}

auto FrameBuffer::height() const -> std::uint32_t
{
    return height_;
}

auto FrameBuffer::colour_texture() const -> const Texture &
{
    return colour_texture_;
}

}
