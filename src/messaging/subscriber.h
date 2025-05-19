#pragma once

#include "events/key_event.h"
#include "events/mouse_event.h"
#include "messaging/message_bus.h"
#include "utils/log.h"

namespace game
{

/**
 * Abstract base class for subscribers to the message bus.
 */
class Subscriber
{
  public:
    ~Subscriber() = default;

    // handlers for messages, ovverride the ones you want to handle

    virtual auto handle_key_press(const KeyEvent &) -> void
    {
        log::warn("unhandled message");
    }

    virtual auto handle_mouse_move(const MouseEvent &) -> void
    {
        log::warn("unhandled message");
    }

    virtual auto handle_level_complete(std::string_view) -> void
    {
        log::warn("unhandled message");
    }
};
}
