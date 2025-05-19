#pragma once

#include <cstdint>
#include <functional>
#include <span>
#include <string_view>
#include <tuple>

#include "graphics/cube_map.h"
#include "graphics/opengl.h"
#include "graphics/sampler.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "maths/colour.h"
#include "maths/matrix4.h"
#include "utils/auto_release.h"
#include "utils/string_map.h"

namespace game
{

class Entity;

/**
 * Class representing a material. A material is a combination of a vertex and fragment shader. It also allows for
 * setting uniforms.
 *
 * The renderer will use this class to set various required uniforms (e.g. model matrix). If you want to set uniforms
 * yourself, you will need to use the callback. This is just a byproduct of the current design of the game. But the
 * renderer will invoke the callback for you with currently rendered entity after the material has been bound.
 */
class Material
{
  public:
    /**
     * Type of the uniform callback. This is invoked after the material has been bound and before the entity is
     * rendered.
     *
     * @param material
     *   The material that is being rendered.
     * @param entity
     *   The entity that is being rendered.
     */
    using UniformCallback = std::function<void(const Material *, const Entity *)>;

    /**
     * Construct a new Material object. It is undefined behaviour to pass any shader other than a vertex and fragment
     * (in that order).
     *
     * @param vertex_shader
     *  The vertex shader to use.
     *
     *  @param fragment_shader
     *  The fragment shader to use.
     */
    Material(const Shader &vertex_shader, const Shader &fragment_shader);

    /**
     * Bind the material for rendering. All subsequent draw calls will use this material.
     */
    auto use() const -> void;

    /**
     * Set a Matrix4 uniform in the shader. The uniform must be set after the material has been bound. It is undefined
     * if the name doesn't exist in the shader.
     *
     * @param name
     *   The name of the uniform to set.
     * @param obj
     *   The object to set the uniform to.
     */
    auto set_uniform(std::string_view name, const Matrix4 &obj) const -> void;

    /**
     * Set a Colour uniform in the shader. The uniform must be set after the material has been bound. It is undefined if
     * the name doesn't exist in the shader.
     *
     * @param name
     *   The name of the uniform to set.
     * @param obj
     *   The object to set the uniform to.
     */
    auto set_uniform(std::string_view name, const Colour &obj) const -> void;

    /**
     * Set an int uniform in the shader. The uniform must be set after the material has been bound. It is undefined if
     * the name doesn't exist in the shader.
     *
     * @param name
     *   The name of the uniform to set.
     * @param obj
     *   The object to set the uniform to.
     */
    auto set_uniform(std::string_view name, int obj) const -> void;

    /**
     * Set a float uniform in the shader. The uniform must be set after the material has been bound. It is undefined if
     * the name doesn't exist in the shader.
     *
     * @param name
     *   The name of the uniform to set.
     * @param obj
     *   The object to set the uniform to.
     */
    auto set_uniform(std::string_view name, float obj) const -> void;

    /**
     * Bind a CubeMap to tex0 in the shader
     *
     * @param cube_map
     *   The CubeMap to bind.
     * @param sampler
     *   The sampler to use.
     */
    auto bind_cube_map(const CubeMap *cube_map, const Sampler *sampler) const -> void;

    /**
     * Bind a Texture in the shader, it will be bound to texN where N is the index.
     *
     * @param index
     *   The index of the texture to bind.
     * @param texture
     *   The Texture to bind.
     * @param sampler
     *   The sampler to use.
     */
    auto bind_texture(std::uint32_t index, const Texture *texture, const Sampler *sampler) const -> void;

    /**
     * Bind a Texture in the shader, it will be bound to texN where N is the index.
     *
     * @param index
     *   The index of the texture to bind.
     * @param texture
     *   The Texture to bind.
     */
    auto bind_texture(std::uint32_t index, const Texture *texture) const -> void;

    /**
     * Bind a collection of textures in the shader, they will be bound to texN where N is the index.
     *
     * @param textures
     *   The textures to bind.
     */
    auto bind_textures(std::span<const Texture *const> textures) const -> void;

    /**
     * Set a callback that will be invoked after the material has been bound and before the entity is rendered.
     *
     * @param uniform_callback
     *   The callback to set.
     */
    auto set_uniform_callback(UniformCallback uniform_callback) -> void;

    /**
     * Invoke the uniform callback for an entity. This is used to set uniforms that are specific to the entity.
     *
     * @param entity
     *   The entity to invoke the callback for.
     */
    auto invoke_uniform_callback(const Entity *entity) const -> void;

    /**
     * Get the native OpenGL handle for the material.
     *
     * @returns
     *   The native OpenGL handle for the material.
     */
    auto native_handle() const -> ::GLuint;

  private:
    /** OpenGL program handle. */
    AutoRelease<::GLuint> handle_;

    /** Collection of uniforms in the shader. */
    StringMap<::GLuint> uniforms_;

    /** Optional uniform callback. */
    UniformCallback uniform_callback_;
};

}
