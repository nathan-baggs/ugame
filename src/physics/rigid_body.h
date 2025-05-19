#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include "maths/quaternion.h"
#include "maths/vector3.h"
#include "physics/shape.h"
#include "utils/pass_key.h"

namespace game
{

class PhysicsSystem;

/**
 * The type of rigid body. This is used to determine the collision layer of the body.
 */
enum class RigidBodyType
{
    STATIC,
    DYNAMIC
};

/**
 * A rigid body in the physics system. This is a wrapper around Jolt's Body class.
 */
class RigidBody
{
  public:
    /**
     * Construct a new rigid body.
     *
     * @param shape
     *   The shape of the rigid body.
     * @param position
     *   The position of the rigid body.
     * @param type
     *   The type of rigid body.
     */
    RigidBody(
        const Shape &shape,
        const Vector3 &position,
        RigidBodyType type,
        ::JPH::BodyInterface &body_interface,
        PassKey<PhysicsSystem>);

    RigidBody(const RigidBody &) = delete;
    auto operator=(const RigidBody &) -> RigidBody & = delete;
    RigidBody(RigidBody &&) = default;
    auto operator=(RigidBody &&) -> RigidBody & = default;

    /**
     * Set the linear velocity of the rigid body.
     *
     * @param linear_velocity
     *   The linear velocity to set.
     */
    auto set_linear_velocity(const Vector3 &linear_velocity) const -> void;

    /**
     * Get the world position of the rigid body.
     *
     * @returns
     *   The world position of the rigid body.
     */
    auto position() const -> Vector3;

    /**
     * Get the world rotation of the rigid body.
     *
     * @returns
     *   The world rotation of the rigid body.
     */
    auto rotation() const -> Quaternion;

    /**
     * Get the type of the rigid body.
     *
     * @returns
     *   The type of the rigid body.
     */
    auto type() const -> RigidBodyType;

    /**
     * Get the native handle of the rigid body.
     *
     * @returns
     *   The native handle of the rigid body.
     */
    auto native_handle() const -> ::JPH::Body *;

  private:
    /** Jolt body handle. */
    ::JPH::Body *body_;

    /** Type of the rigid body. */
    RigidBodyType type_;

    /** Jolt interface handle. */
    ::JPH::BodyInterface *body_interface_;
};
}
