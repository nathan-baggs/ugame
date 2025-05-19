#pragma once

#include <cstddef>
#include <memory>
#include <string_view>

#include "game/levels/level.h"
#include "messaging/subscriber.h"

namespace game
{

/**
 * Game class to encapsulate the game logic and state.
 */
class Game : public Subscriber
{
  public:
    /**
     * Construct a new Game object.
     */
    Game();

    /**
     * Setup and run the game.
     *
     * Runs an infinite loop until the game is stopped.
     *
     * @param resource_root
     *   The root directory for the game resources.
     */
    auto run(std::string_view resource_root) -> void;

    /**
     * Handle a level completion event.
     *
     * @param level_name
     *   The name of the completed level.
     */
    auto handle_level_complete(std::string_view level_name) -> void override;

  private:
    /** Flag to indicate if the game is running. */
    bool running_;

    /** The current level number. */
    std::size_t level_num_;
};
}
