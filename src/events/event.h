#pragma once

#include <variant>

#include "key_event.h"
#include "mouse_button_event.h"
#include "mouse_event.h"
#include "stop_event.h"

namespace game
{

// union of all event types
// note that an event is something raised by the system in relation to user input and different from a message
using Event = std::variant<StopEvent, KeyEvent, MouseEvent, MouseButtonEvent>;

}
