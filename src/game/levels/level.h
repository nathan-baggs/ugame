#pragma once

#include "graphics/scene.h"

namespace game
{

class Player;

class Level
{
  public:
    virtual ~Level() = default;

    virtual auto update(const Player &player) -> void = 0;
    virtual auto restart() -> void = 0;

    auto scene() -> Scene &
    {
        return scene_;
    }

  protected:
    Scene scene_;
};

}
