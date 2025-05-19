#pragma once

#include <memory>

#include <Jolt/Jolt.h>

#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Renderer/DebugRenderer.h>

#include "maths/vector3.h"
#include "utils/pass_key.h"

namespace game
{

class PhysicsSystem;

/**
 * A very basic character controller that uses Jolt's character controller. It is a kinematic character controller that
 * uses a capsule shape.
 */
class CharacterController : public JPH::CharacterContactListener
{
  public:
    /**
     * Construct a new character controller.
     *
     * @param physics_system
     *   The physics system to use.
     *
     * @param pass_key
     *  A pass key to allow construction only from the PhysicsSystem.
     */
    CharacterController(::JPH::PhysicsSystem *physics_system, PassKey<PhysicsSystem>);

    /**
     * Get the position of the character controller.
     *
     * @returns
     *   The position of the character controller.
     */
    auto position() const -> Vector3;

    /**
     * Debug draw the character controller.
     *
     * @param debug_renderer
     *   The debug renderer to use.
     *
     *   @param pass_key
     *   A pass key to allow only PhysicsSystem to call this function.
     */
    auto debug_draw(::JPH::DebugRenderer *debug_renderer, PassKey<PhysicsSystem>) const -> void;

    /**
     * Update the character controller.
     *
     * @param delta
     *   The time since the last update.
     * @param broad_phase_layer_filter
     *   The broad phase layer filter to use.
     * @param object_layer_filter
     *   The object layer filter to use.
     * @param pass_key
     *   A pass key to allow only PhysicsSystem to call this function.
     */
    auto update(
        float delta,
        const ::JPH::BroadPhaseLayerFilter &broad_phase_layer_filter,
        const ::JPH::ObjectLayerFilter &object_layer_filter,
        PassKey<PhysicsSystem>) -> void;

    /**
     * Set the linear velocity of the character controller.
     *
     * @param velocity
     *   The linear velocity to set.
     */
    auto set_linear_velocity(const Vector3 &velocity) -> void;

    /**
     * Jolt contact listener callback. Called when a contact is added. Unimplemented.
     */
    void OnContactAdded(
        const ::JPH::CharacterVirtual *inCharacter,
        const ::JPH::BodyID &inBodyID2,
        const ::JPH::SubShapeID &inSubShapeID2,
        ::JPH::RVec3Arg inContactPosition,
        ::JPH::Vec3Arg inContactNormal,
        ::JPH::CharacterContactSettings &ioSettings) override;

  private:
    /** The character controller. */
    ::JPH::Ref<::JPH::CharacterVirtual> character_;

    /** Jolt allocator. */
    std::unique_ptr<::JPH::TempAllocator> tmp_allocator_;
};

}
