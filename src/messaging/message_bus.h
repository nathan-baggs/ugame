#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

#include "events/key_event.h"
#include "events/mouse_event.h"

namespace game
{

class Subscriber;

/**
 * Enumeration of message types.
 */
enum class MessageType
{
    KEY_PRESS,
    MOUSE_MOVE,
    LEVEL_COMPLETE,
};

/**
 * Message bus for sending messages between different parts of the game.
 */
class MessageBus
{
  public:
    /**
     * Add a subscriber for a specific message type. It is undefined behaviour if the subscriber is already subscribed
     * to the same type.
     *
     * @param type
     *   The type of message to subscribe to.
     * @param subscriber
     *   The subscriber to add.
     */
    auto subscribe(MessageType type, Subscriber *subscriber) -> void;

    /**
     * Post a key press event to all subscribers.
     *
     * @param event
     *   The key event to post.
     */
    auto post_key_press(const KeyEvent &event) -> void;

    /**
     * Post a mouse move event to all subscribers.
     *
     * @param event
     *   The mouse event to post.
     */
    auto post_mouse_move(const MouseEvent &event) -> void;

    /**
     * Post a level complete event to all subscribers.
     *
     * @param level_name
     *   The name of the level that was completed.
     */
    auto post_level_complete(std::string_view level_name) -> void;

  private:
    /** Collection of subscribers for each message type. */
    std::unordered_map<MessageType, std::vector<Subscriber *>> subscribers_;
};

}
