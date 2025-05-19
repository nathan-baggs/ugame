#include "player.h"

#include "events/key_event.h"
#include "events/mouse_event.h"
#include "graphics/camera.h"
#include "maths/vector3.h"
#include "messaging/message_bus.h"
#include "messaging/subscriber.h"

namespace game
{

Player::Player(MessageBus &bus, Camera camera)
    : camera_{std::move(camera)}
    , key_state_{}
{
    bus.subscribe(MessageType::KEY_PRESS, this);
    bus.subscribe(MessageType::MOUSE_MOVE, this);
}

auto Player::handle_key_press(const KeyEvent &event) -> void
{
    key_state_[event.key()] = event.state() == game::KeyState::DOWN;
}

auto Player::handle_mouse_move(const MouseEvent &event) -> void
{
    static constexpr auto sensitivity = float{0.002f};
    const auto delta_x = event.delta_x() * sensitivity;
    const auto delta_y = event.delta_y() * sensitivity;
    camera_.adjust_yaw(delta_x);
    camera_.adjust_pitch(-delta_y);
}

auto Player::camera() const -> const Camera &
{
    return camera_;
}

auto Player::position() const -> Vector3
{
    return camera_.position();
}

auto Player::update() -> void
{
    auto walk_direction = game::Vector3{0.0f, 0.0f, 0.0f};

    walk_direction.y = 0.0f;

    if (key_state_[Key::W])
    {
        walk_direction += camera_.direction();
    }

    if (key_state_[Key::S])
    {
        walk_direction += -camera_.direction();
    }

    if (key_state_[Key::A])
    {
        walk_direction += -camera_.right();
    }

    if (key_state_[Key::D])
    {
        walk_direction += camera_.right();
    }

    walk_direction.y = 0.0f;

    if (walk_direction.length() > 0.0f)
    {
        constexpr static auto speed = 0.5f;
        const auto velocity = game::Vector3::normalise(walk_direction) * speed;
        camera_.translate(velocity);
    }
}

}
