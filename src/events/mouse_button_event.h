#pragma once

#include <string>

namespace game
{

/**
 * Enumeration of mouse button states.
 */
enum class MouseButtonState
{
    UP,
    DOWN
};

/**
 * Mouse button event. Represents a mouse button press or release.
 */
class MouseButtonEvent
{
  public:
    /**
     * Construct a new mouse button event.
     *
     * @param x
     *   The x position of the mouse.
     * @param y
     *   The y position of the mouse.
     * @param state
     *   The state of the mouse button.
     */
    MouseButtonEvent(float x, float y, MouseButtonState state);

    /**
     * Get the x position of the mouse.
     *
     * @returns
     *   The x position of the mouse.
     */
    auto x() const -> float;

    /**
     * Get the y position of the mouse.
     *
     * @returns
     *   The y position of the mouse.
     */
    auto y() const -> float;

    /**
     * Get the state of the mouse button.
     *
     * @returns
     *   The state of the mouse button.
     */
    auto state() const -> MouseButtonState;

    /**
     * Convert the mouse button event to a string.
     *
     * @returns
     *   The string representation of the mouse button event.
     */
    auto to_string() const -> std::string;

    auto operator==(const MouseButtonEvent &) const -> bool = default;

  private:
    /** The x position of the mouse. */
    float x_;

    /** The y position of the mouse. */
    float y_;

    /** The state of the mouse button. */
    MouseButtonState state_;
};

/**
 * Format mouse button state as a string.
 *
 * @param obj
 *   The mouse button state to format.
 *
 * @returns
 *   The formatted string.
 */
auto to_string(MouseButtonState obj) -> std::string;

}
