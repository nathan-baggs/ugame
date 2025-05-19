#include "graphics/renderer.h"

#include <cstdint>
#include <ranges>

#include "buffer_writer.h"
#include "entity.h"
#include "graphics/camera.h"
#include "graphics/cube_map.h"
#include "graphics/frame_buffer.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/mesh_factory.h"
#include "graphics/opengl.h"
#include "graphics/sampler.h"
#include "graphics/scene.h"
#include "graphics/texture.h"
#include "maths/colour.h"
#include "maths/matrix4.h"
#include "maths/vector3.h"
#include "resources/resource_loader.h"
#include "third_party/opengl/glext.h"

namespace
{

// structs that are padded to the same alignment as the OpenGL shader

#pragma warning(push)
#pragma warning(disable : 4324)
struct PointLightBuffer
{
    alignas(16) game::Vector3 position;
    alignas(16) game::Colour colour;
    alignas(16) game::Vector3 attenuation;
};
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4324)
struct LightBuffer
{
    alignas(16) game::Colour ambient;
    alignas(16) game::Vector3 direction;
    alignas(16) game::Colour colour;
    int num_points;
};
#pragma warning(pop)

/**
 * Helper function to create a skybox material.
 *
 * @param resource_loader
 *   The resource loader to use for loading internal resources.
 *
 * @returns
 *   The created skybox material.
 */
auto create_skybox_material(game::ResourceLoader &resource_loader) -> game::Material
{
    const auto vert_file = resource_loader.load("cube.vert");
    const auto frag_file = resource_loader.load("cube.frag");

    const auto vertex_shader = game::Shader{vert_file.as_string(), game::ShaderType::VERTEX};
    const auto fragment_shader = game::Shader{frag_file.as_string(), game::ShaderType::FRAGMENT};
    return game::Material{vertex_shader, fragment_shader};
}

/**
 * Helper function to create a debug line material.
 *
 * @param resource_loader
 *   The resource loader to use for loading internal resources.
 *
 * @returns
 *   The created debug line material.
 */
auto create_debug_line_material(game::ResourceLoader &resource_loader) -> game::Material
{
    const auto vert_file = resource_loader.load("line.vert");
    const auto frag_file = resource_loader.load("line.frag");

    const auto vertex_shader = game::Shader{vert_file.as_string(), game::ShaderType::VERTEX};
    const auto fragment_shader = game::Shader{frag_file.as_string(), game::ShaderType::FRAGMENT};
    return game::Material{vertex_shader, fragment_shader};
}

/**
 * Helper function to create a post-process material.
 *
 * @param resource_loader
 *   The resource loader to use for loading internal resources.
 *
 * @returns
 *   The created post-process material.
 */
auto create_post_process_material(game::ResourceLoader &resource_loader) -> game::Material
{
    const auto vert_file = resource_loader.load("post_process.vert");
    const auto frag_file = resource_loader.load("post_process.frag");

    const auto vertex_shader = game::Shader{vert_file.as_string(), game::ShaderType::VERTEX};
    const auto fragment_shader = game::Shader{frag_file.as_string(), game::ShaderType::FRAGMENT};
    return game::Material{vertex_shader, fragment_shader};
}

}

namespace game
{

Renderer::Renderer(
    ResourceLoader &resource_loader,
    MeshFactory &mesh_factory,
    std::uint32_t width,
    std::uint32_t height)
    : camera_buffer_(sizeof(Matrix4) * 2u + sizeof(Vector3))
    , light_buffer_(10240u)
    , skybox_cube_(mesh_factory.cube())
    , skybox_material_(create_skybox_material(resource_loader))
    , debug_line_material_(create_debug_line_material(resource_loader))
    , fb_(width, height)
    , post_process_sprite_(mesh_factory.sprite())
    , post_process_material_(create_post_process_material(resource_loader))
{
}

auto Renderer::render(const Camera &camera, const Scene &scene, float gamma) const -> void
{
    // first render everything to our internal framebuffer
    fb_.bind();

    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // write the camera data to the GPU buffer
    {
        BufferWriter writer{camera_buffer_};
        writer.write(camera.view());
        writer.write(camera.projection());
        writer.write(camera.position());
    }

    ::glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer_.native_handle());

    // write the light data to the GPU buffer
    // this isn't the most efficient as we're coping the scenese light data to another struct and then copying that to
    // the GPU but it means we can keep the padding misery confined to the renderer
    {
        LightBuffer light_buffer{
            .ambient = scene.ambient,
            .direction = scene.directional.direction,
            .colour = scene.directional.colour,
            .num_points = static_cast<int>(scene.points.size())};

        BufferWriter writer{light_buffer_};
        writer.write(light_buffer);

        for (const auto &point : scene.points)
        {
            auto point_light_buffer = PointLightBuffer{
                .position = point.position,
                .colour = point.colour,
                .attenuation = {point.const_attenuation, point.linear_attenuation, point.quad_attenuation}};
            writer.write(point_light_buffer);
        };
    }

    ::glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_buffer_.native_handle());

    // render skybox

    ::glDepthMask(GL_FALSE);

    skybox_material_.use();
    skybox_cube_.bind();

    skybox_material_.bind_cube_map(scene.skybox, scene.skybox_sampler);
    ::glDrawElements(
        GL_TRIANGLES,
        skybox_cube_.index_count(),
        GL_UNSIGNED_INT,
        reinterpret_cast<void *>(skybox_cube_.index_offset()));

    skybox_cube_.unbind();

    ::glDepthMask(GL_TRUE);

    // render the entities

    for (const auto *entity : scene.entities)
    {
        const auto *mesh = entity->mesh();
        const auto *material = entity->material();

        material->use();
        const auto model = Matrix4{entity->transform()};
        material->set_uniform("model", model);
        // set any material specific uniforms
        material->invoke_uniform_callback(entity);
        material->bind_textures(entity->textures());

        mesh->bind();
        ::glDrawElements(
            GL_TRIANGLES, mesh->index_count(), GL_UNSIGNED_INT, reinterpret_cast<void *>(mesh->index_offset()));
        mesh->unbind();
    }

    // draw any debug lines
    if (const auto &dbl = scene.debug_lines; dbl)
    {
        debug_line_material_.use();
        dbl->bind();
        ::glDrawArrays(GL_LINES, 0u, dbl->count());
        dbl->unbind();
    }

    fb_.unbind();

    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // apply post processing (HDR and gamma correction) and render to the default framebuffer
    post_process_material_.use();
    post_process_sprite_.bind();
    post_process_material_.bind_texture(0, &fb_.colour_texture(), scene.skybox_sampler);
    post_process_material_.set_uniform("gamma", gamma);
    ::glDrawElements(
        GL_TRIANGLES,
        post_process_sprite_.index_count(),
        GL_UNSIGNED_INT,
        reinterpret_cast<void *>(post_process_sprite_.index_offset()));
    post_process_sprite_.unbind();
}
}
