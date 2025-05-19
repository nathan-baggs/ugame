#include "graphics/shader.h"

#include <format>
#include <string>
#include <string_view>

#include "graphics/opengl.h"
#include "utils/auto_release.h"
#include "utils/error.h"
#include "utils/exception.h"

namespace
{

auto to_native(game::ShaderType type) -> ::GLenum
{
    switch (type)
    {
        using enum game::ShaderType;
        case VERTEX: return GL_VERTEX_SHADER;
        case FRAGMENT: return GL_FRAGMENT_SHADER;
    }

    throw game::Exception("unknown shader type: {}", type);
}
}

namespace game
{
Shader::Shader(std::string_view source, ShaderType type)
    : handle_{}
    , type_(type)
{
    handle_ = game::AutoRelease<::GLuint>{::glCreateShader(to_native(type)), ::glDeleteShader};

    const ::GLchar *strings[] = {source.data()};
    const ::GLint lengths[] = {static_cast<::GLint>(source.length())};

    ::glShaderSource(handle_, 1, strings, lengths);
    ::glCompileShader(handle_);

    ::GLint result{};
    ::glGetShaderiv(handle_, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE)
    {
        char log[512];
        ::glGetShaderInfoLog(handle_, sizeof(log), nullptr, log);

        game::ensure(result, "failed to compile shader {}\n{}", type_, log);
    }
}

auto Shader::type() const -> ShaderType
{
    return type_;
}

auto Shader::native_handle() const -> ::GLuint
{
    return handle_;
}

auto to_string(ShaderType obj) -> std::string
{
    switch (obj)
    {
        using enum game::ShaderType;
        case VERTEX: return "VERTEX";
        case FRAGMENT: return "FRAGMENT";
    }

    throw game::Exception("unknown shader type: {}", obj);
}
}
