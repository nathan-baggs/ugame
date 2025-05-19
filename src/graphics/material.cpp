#include "graphics/material.h"

#include <format>
#include <ranges>
#include <string>

#include "entity.h"
#include "graphics/opengl.h"
#include "graphics/shader.h"
#include "utils/auto_release.h"
#include "utils/error.h"

namespace game
{
Material::Material(const Shader &vertex_shader, const Shader &fragment_shader)
    : handle_{}
    , uniforms_{}
    , uniform_callback_{}
{
    expect(vertex_shader.type() == ShaderType::VERTEX, "shader is not a vertex shader");
    expect(fragment_shader.type() == ShaderType::FRAGMENT, "shader is not a fragment shader");

    handle_ = game::AutoRelease<::GLuint>{::glCreateProgram(), ::glDeleteProgram};
    ensure(handle_, "failed to create opengl program");

    ::glAttachShader(handle_, vertex_shader.native_handle());
    ::glAttachShader(handle_, fragment_shader.native_handle());
    ::glLinkProgram(handle_);

    // check program linked

    auto result = ::GLint{};
    ::glGetProgramiv(handle_, GL_LINK_STATUS, &result);

    if (result != GL_TRUE)
    {
        char log[512];
        ::glGetProgramInfoLog(handle_, sizeof(log), nullptr, log);

        game::ensure(result, "failed to link program\n{}", log);
    }

    // get uniforms

    auto uniform_count = ::GLint{};
    ::glGetProgramiv(handle_, GL_ACTIVE_UNIFORMS, &uniform_count);

    if (uniform_count != 0)
    {
        auto max_name_length = ::GLint{};
        ::glGetProgramiv(handle_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length);

        auto length = ::GLsizei{};
        auto count = ::GLsizei{};
        auto type = ::GLenum{};

        for (auto i = 0; i < uniform_count; ++i)
        {
            auto name = std::string(max_name_length, '\0');
            ::glGetActiveUniform(handle_, i, max_name_length, &length, &count, &type, name.data());
            name.resize(length);

            const auto location = ::glGetUniformLocation(handle_, name.c_str());
            uniforms_[name] = location;
        }
    }

    log::info("new material ({} uniforms)", uniform_count);
}

auto Material::use() const -> void
{
    ::glUseProgram(handle_);
}

auto Material::set_uniform(std::string_view name, int obj) const -> void
{
    const auto uniform = uniforms_.find(name);
    expect(uniform != std::ranges::cend(uniforms_), "missing uniform {}", name);

    ::glUniform1i(uniform->second, obj);
}

auto Material::set_uniform(std::string_view name, float obj) const -> void
{
    const auto uniform = uniforms_.find(name);
    expect(uniform != std::ranges::cend(uniforms_), "missing uniform {}", name);

    ::glUniform1f(uniform->second, obj);
}

auto Material::set_uniform(std::string_view name, const Matrix4 &obj) const -> void
{
    const auto uniform = uniforms_.find(name);
    expect(uniform != std::ranges::cend(uniforms_), "missing uniform {}", name);

    ::glUniformMatrix4fv(uniform->second, 1, GL_FALSE, obj.data().data());
}

auto Material::set_uniform(std::string_view name, const Colour &obj) const -> void
{
    const auto uniform = uniforms_.find(name);
    expect(uniform != std::ranges::cend(uniforms_), "missing uniform {}", name);

    ::glUniform3fv(uniform->second, 1, reinterpret_cast<const ::GLfloat *>(std::addressof(obj)));
}

auto Material::bind_cube_map(const CubeMap *cube_map, const Sampler *sampler) const -> void
{
    ::glBindTextureUnit(0, cube_map->native_handle());
    ::glBindSampler(0, sampler->native_handle());

    const auto uniform_name = std::format("tex{}", 0);

    set_uniform(uniform_name, 0);
}

auto Material::bind_texture(std::uint32_t index, const Texture *texture, const Sampler *sampler) const -> void
{
    ::glBindTextureUnit(index, texture->native_handle());
    ::glBindSampler(index, sampler->native_handle());

    const auto uniform_name = std::format("tex{}", index);

    set_uniform(uniform_name, static_cast<int>(index));
}

auto Material::bind_texture(std::uint32_t index, const Texture *texture) const -> void
{
    bind_texture(index, texture, texture->sampler());
}

auto Material::bind_textures(std::span<const Texture *const> textures) const -> void
{
    for (const auto &[index, texture] : textures | std::views::enumerate)
    {
        bind_texture(static_cast<std::uint32_t>(index), texture);
    }
}

auto Material::set_uniform_callback(UniformCallback uniform_callback) -> void
{
    uniform_callback_ = std::move(uniform_callback);
}

auto Material::invoke_uniform_callback(const Entity *entity) const -> void
{
    if (uniform_callback_)
    {
        uniform_callback_(this, entity);
    }
}

auto Material::native_handle() const -> ::GLuint
{
    return handle_;
}
}
