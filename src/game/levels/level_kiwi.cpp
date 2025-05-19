#include "level_kiwi.h"

#include <span>

#include "game/levels/level.h"
#include "game/player.h"
#include "game/transformed_entity.h"
#include "graphics/entity.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/texture.h"
#include "maths/colour.h"
#include "messaging/message_bus.h"
#include "resources/resource_cache.h"
#include "tlv/tlv_reader.h"

namespace
{
auto intersects_frustum(const game::AABB &aabb, const std::array<game::FrustumPlane, 6u> &planes) -> bool
{
    for (const auto &plane : planes)
    {
        auto positive_vertex = aabb.min;
        if (plane.normal.x >= 0)
        {
            positive_vertex.x = aabb.max.x;
        }
        if (plane.normal.y >= 0)
        {
            positive_vertex.y = aabb.max.y;
        }
        if (plane.normal.z >= 0)
        {
            positive_vertex.z = aabb.max.z;
        }

        if (game::Vector3::dot(plane.normal, positive_vertex) + plane.distance < 0.0f)
        {
            return false;
        }
    }

    return true;
}

constexpr auto CameraDelta = [](const game::Vector3 &in,
                                const game::GameTransformState &state) -> game::TransformerResult
{ return {in + (state.camera.position() - state.last_camera_pos)}; };

constexpr auto Invert = [](const game::Vector3 &in, const game::GameTransformState &) -> game::TransformerResult
{ return {-in}; };

constexpr auto CheckVisible = [](const game::Vector3 &in,
                                 const game::GameTransformState &state) -> game::TransformerResult
{
    const auto planes = state.camera.frustum_planes();
    return {in, !intersects_frustum(state.aabb, planes)};
};

std::vector<const game::Texture *> textures()
{
    return {};
}

}

namespace game
{

LevelKiwi::LevelKiwi(DefaultCache &resource_cache, const TLVReader &reader, const Player &player, MessageBus &bus)
    : entities_{}
    , floor_{resource_cache.get<Mesh>("floor"), resource_cache.get<Material>("floor"), {0.0f, -3.0f, 0.0f}, {100.0f, 1.0f, 100.0f}, std::vector<const Texture*>{
    resource_cache.get<Texture>("floor_albedo"),        
    resource_cache.get<Texture>("floor_albedo")        
    }}
    , skybox_{reader, {{"right", "left", "top", "bottom", "front", "back"}}}
    , skybox_sampler_{}
    , state_{.camera = player.camera(), .aabb = {}, .last_camera_pos = player.camera().position()}
    , bus_{bus}
    , resource_cache_{resource_cache}

{
    const Texture *barrel_textures[]{
        resource_cache.get<Texture>("barrel_albedo"),
        resource_cache.get<Texture>("barrel_specular"),
        resource_cache.get<Texture>("barrel_normal")};

    entities_.emplace_back(
        Entity{
            resource_cache.get<Mesh>("barrel"),
            resource_cache.get<Material>("barrel"),
            {0.0f, 0.0f, 0.0f},
            {0.05f},
            barrel_textures},
        AABB{{-1.0f, -1.0f, -1.0f}, {1.0f, 2.0f, 1.0f}},
        std::make_unique<Chain<GameTransformState>>());
    entities_.emplace_back(
        Entity{
            resource_cache.get<Mesh>("barrel"),
            resource_cache.get<Material>("barrel"),
            {5.0f, 0.0f, 0.0f},
            {0.05f},
            barrel_textures},
        AABB{{3.0f, -1.0f, -1.0f}, {5.0f, 2.0f, 1.0f}},
        std::make_unique<Chain<GameTransformState, CheckVisible, CameraDelta, Invert>>());

    scene_ = Scene{
        .entities = entities_ | std::views::transform([](const auto &e) { return std::addressof(e.entity); }) |
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
        .skybox = &skybox_,
        .skybox_sampler = &skybox_sampler_};

    scene_.entities.push_back(&floor_);
}

auto LevelKiwi::update(const Player &player) -> void
{
    for (auto &transformed_entity : entities_)
    {
        auto &[entity, aabb, transformer] = transformed_entity;

        state_.aabb = aabb;
        const auto entity_delta = transformer->go({}, state_);
        entity.translate(entity_delta);
        aabb.min += entity_delta;
        aabb.max += entity_delta;
    }

    state_.last_camera_pos = player.camera().position();

    if (Vector3::distance(entities_[0].entity.position(), entities_[1].entity.position()) < 1.0f)
    {
        bus_.post_level_complete("apple");
    }
}

auto LevelKiwi::restart() -> void
{
    resource_cache_.get<Material>("barrel")->set_uniform_callback(
        [this](const Material *material, const Entity *entity)
        {
            const auto tint_amount = entity == std::addressof(entities_[0].entity) ? 1.0f : 0.5f;
            material->set_uniform("tint_colour", Colour{.r = 0.0f, .g = 0.0f, .b = 1.0f});
            material->set_uniform("tint_amount", tint_amount);
        });
}

}
