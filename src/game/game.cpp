#include "game/game.h"

#include <format>
#include <iostream>
#include <numbers>
#include <print>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include <Windows.h>

#include "events/key.h"
#include "events/stop_event.h"
#include "game/player.h"
#include "graphics/camera.h"
#include "graphics/cube_map.h"
#include "graphics/debug_ui.h"
#include "graphics/entity.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/mesh_factory.h"
#include "graphics/renderer.h"
#include "graphics/sampler.h"
#include "graphics/shader.h"
#include "graphics/shape_wireframe_renderer.h"
#include "graphics/texture.h"
#include "graphics/window.h"
#include "maths/vector3.h"
#include "messaging/message_bus.h"
#include "messaging/subscriber.h"
#include "resources/resource_cache.h"
#include "resources/resource_loader.h"
#include "tlv/tlv_reader.h"
#include "utils/error.h"
#include "utils/exception.h"
#include "utils/log.h"

namespace game
{

Game::Game()
    : running_{true}
{
}

auto Game::run(std::string_view resource_root) -> void
{
    auto bus = MessageBus{};
    bus.subscribe(MessageType::LEVEL_COMPLETE, this);

    auto window = Window{1920u, 1080u, 1920u, 0u};
    auto camera = Camera{
        {0.0f, 5.0f, 50.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f},
        std::numbers::pi_v<float> / 4.0f,
        static_cast<float>(window.width()),
        static_cast<float>(window.height()),
        0.1f,
        1000.0f};
    auto player = Player{bus, std::move(camera)};

    auto resource_loader = ResourceLoader{resource_root};
    auto mesh_factory = MeshFactory{};
    auto resource_cache = DefaultCache{};

    const auto *sampler = resource_cache.insert<Sampler>("default");

    const auto tlv_file = resource_loader.load("resource");
    const auto reader = TLVReader{tlv_file.as_data()};

    log::info("textures loaded");

    const auto simple_vert_file = resource_loader.load("simple.vert");
    const auto checkerboard_frag_file = resource_loader.load("checkerboard.frag");

    const auto vertex_shader = Shader{simple_vert_file.as_string(), ShaderType::VERTEX};
    const auto checkerboard_shader = Shader{checkerboard_frag_file.as_string(), ShaderType::FRAGMENT};
    resource_cache.insert<Material>("floor", vertex_shader, checkerboard_shader);

    resource_cache.insert<Texture>(
        "floor_albedo",
        TextureDescription{
            .width = 1u,
            .height = 1u,
            .format = TextureFormat::RGB,
            .usage = TextureUsage::SRGB,
            .data = {static_cast<std::byte>(0xff), static_cast<std::byte>(0xff), static_cast<std::byte>(0xff)}},
        sampler);
    resource_cache.insert<Mesh>("floor", mesh_factory.cube());

    const auto renderer = Renderer{resource_loader, mesh_factory, window.width(), window.height()};

    auto entities = std::vector<Entity>{
        {resource_cache.get<Mesh>("floor"),
         resource_cache.get<Material>("floor"),
         {0.0f, -3.0f, 0.0f},
         {100.0f, 1.0f, 100.0f},
         std::vector<const Texture *>{
             resource_cache.get<Texture>("floor_albedo"), resource_cache.get<Texture>("floor_albedo")}}};

    auto skybox = CubeMap{reader, {{"right", "left", "top", "bottom", "front", "back"}}};
    auto skybox_sampler = Sampler{};

    auto scene = Scene{
        .entities = entities | std::views::transform([](const auto &e) { return std::addressof(e); }) |
                    std::ranges::to<std::vector>(),
        .ambient = {.r = 0.3f, .g = 0.3f, .b = 0.3f},
        .directional = {.direction = {-1.0f, -1.0f, -1.0f}, .colour = {.r = 0.5f, .g = 0.5f, .b = 0.5f}},
        .points =
            {{.position = {5.0f, 5.0f, 0.0f},
              .colour = {.r = 1.0f, .g = 0.0f, .b = 0.0f},
              .const_attenuation = 1.0f,
              .linear_attenuation = 0.07f,
              .quad_attenuation = 0.007f},
             {.position = {-5.0f, 5.0f, 0.0f},
              .colour = {.r = 0.0f, .g = 1.0f, .b = 0.0f},
              .const_attenuation = 1.0f,
              .linear_attenuation = 0.07f,
              .quad_attenuation = 0.007f},
             {.position = {-5.0f, 5.0f, 0.0f},
              .colour = {.r = 0.0f, .g = 0.0f, .b = 1.0f},
              .const_attenuation = 1.0f,
              .linear_attenuation = 0.07f,
              .quad_attenuation = 0.007f}},
        .debug_lines = {},
        .skybox = &skybox,
        .skybox_sampler = &skybox_sampler};

    auto gamma = 2.2f;

    auto wireframe_renderer = ShapeWireframeRenderer{};

    while (running_)
    {
        auto event = window.pump_event();
        while (event && running_)
        {
            std::visit(
                [&](auto &&arg)
                {
                    using T = std::decay_t<decltype(arg)>;

                    if constexpr (std::same_as<T, StopEvent>)
                    {
                        running_ = false;
                    }
                    else if constexpr (std::same_as<T, KeyEvent>)
                    {
                        if (arg.key() == Key::ESC)
                        {
                            running_ = false;
                        }

                        bus.post_key_press(arg);
                    }
                    else if constexpr (std::same_as<T, MouseEvent>)
                    {
                        bus.post_mouse_move(arg);
                    }
                },
                *event);
            event = window.pump_event();
        }

        player.update();

        wireframe_renderer.draw(player.camera());
        scene.debug_lines = {wireframe_renderer.yield()};

        renderer.render(player.camera(), scene, gamma);

        window.swap();
    }
}

auto Game::handle_level_complete(std::string_view level_name) -> void
{
    log::info("level complete: {}", level_name);
}
}
