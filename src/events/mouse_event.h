#pragma once

#include <string>

namespace game
{

/**
 * Mouse event. Represents a mouse movement.
 */
class MouseEvent
{
  public:
    /**
     * Construct a new mouse event.
     *
     * @param delta_x
     *   The change in x position of the mouse.
     * @param delta_y
     *   The change in y position of the mouse.
     */
    MouseEvent(float delta_x, float delta_y);

    /**
     * Get the change in x position of the mouse.
     *
     * @returns
     *   The change in x position of the mouse.
     */
    auto delta_x() const -> float;

    /**
     * Get the change in y position of the mouse.
     *
     * @returns
     *   The change in y position of the mouse.
     */
    auto delta_y() const -> float;

    /**
     * Convert the mouse event to a string.
     *
     * @returns
     *   The string representation of the mouse event.
     */
    auto to_string() const -> std::string;

    auto operator==(const MouseEvent &) const -> bool = default;

  private:
    /** The change in x position of the mouse. */
    float delta_x_;

    /** The change in y position of the mouse. */
    float delta_y_;
};

}
