#pragma once

#include <format>
#include <string>

#include <Jolt/Jolt.h>

#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include "physics/shape.h"
#include "utils/pass_key.h"

namespace game
{
class PhysicsSystem;

/**
 * A sphere shape for use with rigid bodies.
 */
class SphereShape : public Shape
{
  public:
    /**
     * Construct a new sphere shape.
     *
     * @param radius
     *   The radius of the sphere.
     * @param pass_key
     *   A pass key to allow construction only from the PhysicsSystem.
     */
    SphereShape(float radius, PassKey<PhysicsSystem>);

    /**
     * Get the radius of the sphere.
     *
     * @returns
     *   The radius of the sphere.
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
     * Format the sphere shape as a string.
     *
     * @returns
     *   The string representation of the sphere shape.
     */
    auto to_string() const -> std::string;

  private:
    /** Radius of the sphere. */
    float radius_;

    /** Sphere shape settings. */
    ::JPH::SphereShapeSettings sphere_shape_settings_;
};
}
