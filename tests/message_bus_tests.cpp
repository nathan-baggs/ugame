#include <optional>

#include <gtest/gtest.h>

#include "events/key_event.h"
#include "messaging/message_bus.h"
#include "messaging/subscriber.h"

struct TestSub : public game::Subscriber
{
    auto handle_key_press(const game::KeyEvent &event) -> void override
    {
        key_event = event;
    }

    auto handle_mouse_move(const game::MouseEvent &event) -> void override
    {
        mouse_event = event;
    }

    auto handle_level_complete(std::string_view name) -> void override
    {
        level_name = name;
    }

    std::optional<game::KeyEvent> key_event;
    std::optional<game::MouseEvent> mouse_event;
    std::optional<std::string> level_name;
};

TEST(message_bus, post_key_message)
{
    auto bus = game::MessageBus{};
    auto sub = TestSub{};
    bus.subscribe(game::MessageType::KEY_PRESS, &sub);
    const auto event = game::KeyEvent{game::Key::A, game::KeyState::DOWN};

    bus.post_key_press(event);

    ASSERT_TRUE(!!sub.key_event);
    ASSERT_EQ(*sub.key_event, event);
}

TEST(message_bus, post_mouse_message)
{
    auto bus = game::MessageBus{};
    auto sub = TestSub{};
    bus.subscribe(game::MessageType::MOUSE_MOVE, &sub);
    const auto event = game::MouseEvent{1.0f, 2.0f};

    bus.post_mouse_move(event);

    ASSERT_TRUE(!!sub.mouse_event);
    ASSERT_EQ(*sub.mouse_event, event);
}

TEST(message_bus, post_level_complete_message)
{
    auto bus = game::MessageBus{};
    auto sub = TestSub{};
    bus.subscribe(game::MessageType::LEVEL_COMPLETE, &sub);
    const auto level_name = "level_1";

    bus.post_level_complete(level_name);

    ASSERT_TRUE(!!sub.level_name);
    ASSERT_EQ(*sub.level_name, level_name);
}
