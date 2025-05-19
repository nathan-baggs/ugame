#pragma once

#include <vector>

#include "graphics/camera.h"
#include "graphics/line_data.h"
#include "maths/aabb.h"
#include "maths/colour.h"
#include "maths/vector3.h"

namespace game
{

/**
 * This class simplifies the process of rendering wireframe shapes. Note that this class does not perform an OpenGL
 * calls, the "rendering" here means more like recording.
 *
 * The API allows you to draw various shapes. Internally these are stored on a collection. You can "yield" the
 * collection which returns a copy and clears the internal collection.
 *
 * The idea is that each frame you draw whatever debug info you need, then at the end you yield it and are ready to do
 * the same next frame.
 */
class ShapeWireframeRenderer
{
  public:
    /**
     * Draw a line between two points.
     *
     * @param start
     *   The start point of the line.
     * @param end
     *   The end point of the line.
     * @param colour
     *   The colour of the line.
     */
    auto draw(const Vector3 &start, const Vector3 &end, const Colour &colour) -> void;

    /**
     * Draw the camera frustum.
     *
     * @param camera
     *   The camera to draw the frustum for.
     */
    auto draw(const Camera &camera) -> void;

    /**
     * Draw a wireframe box.
     *
     * @param aabb
     *   The axis aligned bounding box to draw.
     */
    auto draw(const AABB &aabb) -> void;

    /**
     * Yield the current collection of lines. This clears the internal collection.
     *
     * @returns
     *   The current collection of lines.
     */
    auto yield() -> std::vector<LineData>;

  private:
    /** Collection of lines to draw. */
    std::vector<LineData> lines_;
};

}
