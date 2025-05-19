#include "physics/character_controller.h"

#include <Jolt/Jolt.h>

#include <Jolt/Core/Core.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include <utility>

#include "Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h"
#include "physics/jolt_utils.h"
#include "physics/physics_system.h"
#include "physics/rigid_body.h"
#include "utils/log.h"
#include "utils/pass_key.h"

namespace game
{

CharacterController::CharacterController(::JPH::PhysicsSystem *physics_system, PassKey<PhysicsSystem>)
    : character_{}
    , tmp_allocator_{std::make_unique<::JPH::TempAllocatorImpl>(4 * 1024 * 1024)}
{
    static constexpr auto standing_height = 5.0f;
    static constexpr auto standing_radius = 2.5f;

    ::JPH::Ref<::JPH::CharacterVirtualSettings> settings = new ::JPH::CharacterVirtualSettings();
    settings->mShape = ::JPH::RotatedTranslatedShapeSettings(
                           ::JPH::Vec3(0, 0.5f * standing_height + standing_radius, 0),
                           ::JPH::Quat::sIdentity(),
                           new ::JPH::CapsuleShape(0.5f * standing_height, standing_radius))
                           .Create()
                           .Get();
    settings->mInnerBodyLayer = to_jolt(RigidBodyType::DYNAMIC);

    character_ =
        new ::JPH::CharacterVirtual{settings, ::JPH::RVec3::sZero(), ::JPH::Quat::sIdentity(), 0, physics_system};
    character_->SetListener(this);
}

auto CharacterController::position() const -> Vector3
{
    return to_native(character_->GetPosition());
}

auto CharacterController::debug_draw(::JPH::DebugRenderer *debug_renderer, PassKey<PhysicsSystem>) const -> void
{
    const auto transform = character_->GetCenterOfMassTransform();
    character_->GetShape()->Draw(debug_renderer, transform, ::JPH::Vec3(1, 1, 1), ::JPH::Color::sGreen, false, true);
}

auto CharacterController::update(
    float delta,
    const ::JPH::BroadPhaseLayerFilter &broad_phase_layer_filter,
    const ::JPH::ObjectLayerFilter &object_layer_filter,
    PassKey<PhysicsSystem>) -> void
{
    character_->Update(
        delta, ::JPH::Vec3{0.0f, -9.8f, 0.0f}, broad_phase_layer_filter, object_layer_filter, {}, {}, *tmp_allocator_);
}

auto CharacterController::set_linear_velocity(const Vector3 &velocity) -> void
{
    character_->SetLinearVelocity(to_jolt(velocity));
}

void CharacterController::OnContactAdded(
    [[maybe_unused]] const ::JPH::CharacterVirtual *inCharacter,
    [[maybe_unused]] const ::JPH::BodyID &inBodyID2,
    [[maybe_unused]] const ::JPH::SubShapeID &inSubShapeID2,
    [[maybe_unused]] ::JPH::RVec3Arg inContactPosition,
    [[maybe_unused]] ::JPH::Vec3Arg inContactNormal,
    [[maybe_unused]] ::JPH::CharacterContactSettings &ioSettings)
{
    log::debug("contact {}", inBodyID2.GetIndex());
}
}
