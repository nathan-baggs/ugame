#pragma once

#include <string>

#include <Jolt/Jolt.h>

#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>

#include "physics/shape.h"
#include "utils/pass_key.h"

namespace game
{
class PhysicsSystem;

/**
 * A cylinder shape for use with rigid bodies.
 */
class CylinderShape : public Shape
{
  public:
    /**
     * Construct a new cylinder shape.
     *
     * @param half_height
     *   The half height of the cylinder.
     * @param radius
     *   The radius of the cylinder.
     * @param pass_key
     *   A pass key to allow construction only from the PhysicsSystem.
     */
    CylinderShape(float half_height, float radius, PassKey<PhysicsSystem>);

    /**
     * Get the half height of the cylinder.
     *
     * @returns
     *   The half height of the cylinder.
     */
    auto half_height() const -> float;

    /**
     * Get the radius of the cylinder.
     *
     * @returns
     *   The radius of the cylinder.
     */
    auto radius() const -> float;

    /**
     * Get the native handle of the shape.
     *
     * @returns
     *   The native handle of the shape.
     */
    auto native_handle() const -> const ::JPH::ShapeSettings * override;

    /**
     * Format the cylinder shape as a string.
     *
     * @returns
     *   The string representation of the cylinder shape.
     */
    auto to_string() const -> std::string;

  private:
    /** Half height of the cylinder. */
    float half_height_;

    /** Radius of the cylinder. */
    float radius_;

    /** Cylinder shape settings. */
    ::JPH::CylinderShapeSettings cylinder_shape_settings_;
};
}
