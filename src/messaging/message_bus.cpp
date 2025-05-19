#include "message_bus.h"

#include "events/key_event.h"
#include "events/mouse_event.h"
#include "messaging/subscriber.h"
#include "utils/error.h"

namespace
{

/**
 * Helper function to post a message to all subscribers of a specific type.
 *
 * @param type
 *   The type of message to post.
 * @param subscribers
 *   The map of subscribers to post the message to.
 * @param func
 *   The function to call on each subscriber.
 * @param args
 *   The arguments to pass to the function.
 */
template <class... Args>
auto post_message(game::MessageType type, auto &subscribers, auto func, Args &&...args) -> void
{
    for (auto *subscriber : subscribers[type])
    {
        func(subscriber, std::forward<Args>(args)...);
    }
}

}

namespace game
{

auto MessageBus::subscribe(MessageType type, Subscriber *subscriber) -> void
{
    auto &subscribers = subscribers_[type];

    expect(!std::ranges::contains(subscribers, subscriber), "subscriber already subscribed");

    subscribers.push_back(subscriber);
}

auto MessageBus::post_key_press(const KeyEvent &event) -> void
{
    post_message(
        MessageType::KEY_PRESS,
        subscribers_,
        [](auto *sub, const auto &event) { sub->handle_key_press(event); },
        event);
}

auto MessageBus::post_mouse_move(const MouseEvent &event) -> void
{
    post_message(
        MessageType::MOUSE_MOVE,
        subscribers_,
        [](auto *sub, const auto &event) { sub->handle_mouse_move(event); },
        event);
}

auto MessageBus::post_level_complete(std::string_view level_name) -> void
{
    post_message(
        MessageType::LEVEL_COMPLETE,
        subscribers_,
        [](auto *sub, const auto &level_name) { sub->handle_level_complete(level_name); },
        level_name);
}

}
