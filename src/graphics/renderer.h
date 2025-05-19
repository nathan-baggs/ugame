#pragma once

#include <cstdint>

#include "graphics/buffer.h"
#include "graphics/camera.h"
#include "graphics/frame_buffer.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/mesh_factory.h"
#include "graphics/scene.h"
#include "resources/resource_loader.h"

namespace game
{

/**
 * A simple opinionated forward renderer.
 *
 * This will render a scene from a camera, add basic skybox and HDR.
 */
class Renderer
{
  public:
    /**
     * Construct a Renderer.
     *
     * @param resource_loader
     *   The resource loader to use for loading internal resources.
     * @param mesh_factory
     *   The mesh factory to use for creating internal meshes.
     * @param width
     *   The width of the output framebuffer.
     * @param height
     *   The height of the output framebuffer.
     */

    Renderer(ResourceLoader &resource_loader, MeshFactory &mesh_factory, std::uint32_t width, std::uint32_t height);

    /**
     * Render a single frame to the default framebuffer. Does not swap buffers.
     *
     * @param camera
     *   The camera to use for rendering.
     * @param scene
     *   The scene to render.
     * @param gamma
     *   The gamma value to use for rendering.
     */
    auto render(const Camera &camera, const Scene &scene, float gamma) const -> void;

  private:
    /** OpenGL buffer for camera data. */
    Buffer camera_buffer_;

    /** OpenGL buffer for light data. */
    Buffer light_buffer_;

    /** The SkyBox mesh. */
    Mesh skybox_cube_;

    /** The SkyBox material. */
    Material skybox_material_;

    /** Material for rendering debug lines. */
    Material debug_line_material_;

    /** The framebuffer used for rendering (before post-processing). */
    FrameBuffer fb_;

    /** Msh for rendering post processing. */
    Mesh post_process_sprite_;

    /** Post processing material. */
    Material post_process_material_;
};

}
