#pragma once

#include <windows.h>

#include "imgui.h"

#include "events/mouse_button_event.h"
#include "graphics/camera.h"
#include "graphics/scene.h"

namespace game
{

/**
 * Class representing the debug UI for the game. Used for rendering debug information. Currently uses a hard coded
 * layout. Internally uses DearImGui.
 */
class DebugUI
{
  public:
    /**
     * Construct a new DebugUI object.
     *
     * @param window
     *   The window handle to render the UI to.
     * @param scene
     *   The scene to render the UI for. May manipulate the scene.
     * @param camera
     *   The camera to render the UI for.
     * @param gamma
     *   The gamma value to use for rendering. May manipulate the gamma value.
     */
    DebugUI(HWND window, Scene &scene, const Camera &camera, float &gamma);

    /**
     * Destroy the DebugUI object.
     */
    ~DebugUI();

    /**
     * Render the debug UI. Will issue OpenGL calls.
     */
    auto render() const -> void;

    /**
     * Add a mouse event to the debug UI. This is used for handling mouse events in the UI.
     *
     * @param evt
     *   The mouse event to add.
     */
    auto add_mouse_event(const MouseButtonEvent &evt) const -> void;

  private:
    /** Scene to render the UI for. */
    Scene &scene_;

    /** Camera to render the UI for. */
    const Camera &camera_;

    /** Gamma value to use for rendering. */
    float &gamma_;
};

}
