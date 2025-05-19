#pragma once

#include <string>

#include "events/key.h"

namespace game
{

/**
 * Key event. Represents a key press or release.
 */
class KeyEvent
{
  public:
    /**
     * Construct a new key event.
     *
     * @param key
     *   The key that was pressed or released.
     * @param state
     *   The state of the key.
     */
    KeyEvent(Key key, KeyState state);

    /**
     * Get the key that was pressed or released.
     *
     * @returns
     *   The key that was pressed or released.
     */
    auto key() const -> Key;

    /**
     * Get the state of the key.
     *
     * @returns
     *   The state of the key.
     */
    auto state() const -> KeyState;

    /**
     * Convert the key event to a string.
     *
     * @returns
     *   The string representation of the key event.
     */
    auto to_string() const -> std::string;

    auto operator==(const KeyEvent &) const -> bool = default;

  private:
    /** The key that was pressed or released. */
    Key key_;

    /** The state of the key. */
    KeyState state_;
};

}
