#pragma once

#include <string>

#include <Jolt/Jolt.h>

#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include "maths/vector3.h"
#include "physics/shape.h"
#include "utils/pass_key.h"

namespace game
{
class PhysicsSystem;

/**
 * A box shape for use with rigid bodies.
 */
class BoxShape : public Shape
{
  public:
    /**
     * Construct a new box shape.
     *
     * @param dimensions
     *   The dimensions of the box: width, height, depth.
     * @param pass_key
     *   A pass key to allow construction only from the PhysicsSystem.
     */
    BoxShape(const Vector3 &dimensions, PassKey<PhysicsSystem>);

    /**
     * Get the dimensions of the box.
     *
     * @returns
     *   The dimensions of the box.
     */
    auto dimensions() const -> Vector3;

    /**
     * Get the native handle of the shape.
     *
     * @returns
     *   The native handle of the shape.
     */
    auto native_handle() const -> const ::JPH::ShapeSettings * override;

    /**
     * Format the box shape as a string.
     *
     * @returns
     *   The string representation of the box shape.
     */
    auto to_string() const -> std::string;

  private:
    /** Dimensions of the box. */
    Vector3 dimensions_;

    /** The native Jolt box shape settings. */
    ::JPH::BoxShapeSettings box_shape_settings_;
};
}
