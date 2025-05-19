#pragma once

#include <cstdint>
#include <optional>

#include <Windows.h>

#include "events/event.h"
#include "utils/auto_release.h"

namespace game
{

/**
 * This class handles the creation and management of a native window.
 */
class Window
{
  public:
    /**
     * Construct a new Window.
     *
     * @param width
     *   The width of the window.
     * @param height
     *  The height of the window.
     * @param x
     *   The screen x position of the window.
     * @param y
     *   The screen y position of the window.
     */
    Window(std::uint32_t width, std::uint32_t height, std::uint32_t x, std::uint32_t y);
    ~Window() = default;

    Window(const Window &) = delete;
    auto operator=(const Window &) -> Window & = delete;
    Window(Window &&) = default;
    auto operator=(Window &&) -> Window & = default;

    /**
     * Pump the last event from the window.
     *
     * @returns
     *   The last event from the window, or empty if there are no more events.
     */
    auto pump_event() const -> std::optional<Event>;

    /**
     * Swap the front and back buffers of the window.
     */
    auto swap() const -> void;

    /**
     * Get the native handle of the window.
     *
     * @returns
     *   The native handle of the window.
     */
    auto native_handle() const -> HWND;

    /**
     * Get the width of the window.
     *
     * @returns
     *   The width of the window.
     */
    auto width() const -> std::uint32_t;

    /**
     * Get the height of the window.
     *
     * @returns
     *   The height of the window.
     */
    auto height() const -> std::uint32_t;

  private:
    /** The native window handle. */
    AutoRelease<::HWND, nullptr> window_;

    /** The native window device context. */
    AutoRelease<::HDC> dc_;

    /** The native window class object. */
    ::WNDCLASSA wc_;

    /** Width of the window. */
    std::uint32_t width_;

    /** Height of the window. */
    std::uint32_t height_;
};

}
