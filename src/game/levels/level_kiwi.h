#pragma once

#include <span>
#include <vector>

#include "game/levels/level.h"
#include "game/transformed_entity.h"
#include "graphics/cube_map.h"
#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/sampler.h"
#include "graphics/texture.h"
#include "resources/resource_cache.h"
#include "tlv/tlv_reader.h"

namespace game
{

class Player;
class MessageBus;

class LevelKiwi : public Level
{
  public:
    LevelKiwi(DefaultCache &resource_cache, const TLVReader &reader, const Player &player, MessageBus &bus);
    ~LevelKiwi() override = default;

    auto update(const Player &player) -> void override;
    auto restart() -> void override;

  private:
    std::vector<TransformedEntity> entities_;
    Entity floor_;
    CubeMap skybox_;
    Sampler skybox_sampler_;
    GameTransformState state_;
    MessageBus &bus_;
    DefaultCache &resource_cache_;
};

}
