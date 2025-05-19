#pragma once

#include <unordered_map>

#include "events/key_event.h"
#include "events/mouse_event.h"
#include "graphics/camera.h"
#include "maths/vector3.h"
#include "messaging/message_bus.h"
#include "messaging/subscriber.h"

namespace game
{

class Player : public Subscriber
{
  public:
    Player(MessageBus &bus, Camera camera);

    auto handle_key_press(const KeyEvent &event) -> void override;
    auto handle_mouse_move(const MouseEvent &event) -> void override;

    auto camera() const -> const Camera &;

    auto position() const -> Vector3;

    auto update() -> void;

  private:
    Camera camera_;
    std::unordered_map<Key, bool> key_state_;
};

}
