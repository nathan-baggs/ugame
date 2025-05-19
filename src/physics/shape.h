#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

#include "utils/pass_key.h"

namespace game
{

class PhysicsSystem;

/**
 * Base class for all shapes. This is used to create shapes for rigid bodies.
 */
class Shape
{
  public:
    /**
     * Construct a new shape.
     *
     * @param pass_key
     *   A pass key to allow construction only from the PhysicsSystem.
     */
    Shape(PassKey<PhysicsSystem>);

    /**
     * Get the native handle of the shape.
     *
     * @returns
     *   The native handle of the shape.
     */
    virtual auto native_handle() const -> const ::JPH::ShapeSettings * = 0;
};

}
