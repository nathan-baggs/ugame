#pragma once

#include <memory>

#include "physics/character_controller.h"
#include "physics/debug_renderer.h"
#include "physics/rigid_body.h"
#include "utils/pass_key.h"

namespace game
{

/**
 * Jolt has the concept of "layers" which are used to filter collisions. These are the hard coded layers that are used
 * in the game.
 */
enum class PhysicsLayer
{
    /** Static geometry. */
    NON_MOVING,

    /** Dynamic objects. */
    MOVING,

    MAX_LAYER
};

/**
 * A physics system that uses Jolt for physics simulation. It is responsible for creating and managing the physics
 * world, shapes and rigid bodies.
 */
class PhysicsSystem
{
  public:
    /**
     * Construct a new physics system.
     */
    PhysicsSystem();
    ~PhysicsSystem();

    /**
     * Advance the simulation by one tick.
     */
    auto update() -> void;

    /**
     * Get the debug renderer.
     *
     * @returns
     *   The debug renderer.
     */
    auto debug_renderer() const -> const DebugRenderer &;

    /**
     * Create a new shape of the given type.
     *
     * @param args
     *   The arguments to pass to the shape constructor.
     *
     * @returns
     *   The new shape.
     */
    template <class T, class... Args>
    auto create_shape(Args &&...args) const -> T
    {
        return T{std::forward<Args>(args)..., PassKey<PhysicsSystem>{}};
    }

    /**
     * Create a new rigid body.
     *
     * @param shape
     *   The shape of the rigid body.
     * @param position
     *   The position of the rigid body.
     * @param type
     *   The type of the rigid body.
     */
    auto create_rigid_body(const Shape &shape, const Vector3 &position, RigidBodyType type) const -> RigidBody;

    /**
     * Get a reference to the single character controller.
     *
     * @returns
     *   The character controller.
     */
    auto character_controller() const -> CharacterController &;

  private:
    struct implementation;
    std::unique_ptr<implementation> impl_;
};
}
