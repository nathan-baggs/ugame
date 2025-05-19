#include "physics/rigid_body.h"

#include <memory>

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Body/MotionType.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>

#include "jolt_utils.h"
#include "maths/vector3.h"
#include "physics/physics_system.h"
#include "physics/shape.h"
#include "utils/exception.h"
#include "utils/pass_key.h"

namespace
{

auto to_jolt_type(game::RigidBodyType type) -> ::JPH::EMotionType
{
    switch (type)
    {
        using enum game::RigidBodyType;
        case STATIC: return ::JPH::EMotionType::Static;
        case DYNAMIC: return ::JPH::EMotionType::Dynamic;
    }

    throw game::Exception("unknown type");
}

auto to_layer(game::RigidBodyType type) -> game::PhysicsLayer
{
    switch (type)
    {
        using enum game::RigidBodyType;
        using enum game::PhysicsLayer;

        case STATIC: return NON_MOVING;
        case DYNAMIC: return MOVING;
    }

    throw game::Exception("unknown type");
}

auto to_activation(game::RigidBodyType type) -> ::JPH::EActivation
{
    switch (type)
    {
        using enum game::RigidBodyType;
        case STATIC: return ::JPH::EActivation::DontActivate;
        case DYNAMIC: return ::JPH::EActivation::Activate;
    }

    throw game::Exception("unknown type");
}

}

namespace game
{

RigidBody::RigidBody(
    const Shape &shape,
    const Vector3 &position,
    RigidBodyType type,
    ::JPH::BodyInterface &body_interface,
    PassKey<PhysicsSystem>)
    : body_{}
    , type_{type}
    , body_interface_{std::addressof(body_interface)}
{
    auto body_settings = ::JPH::BodyCreationSettings{
        shape.native_handle(), to_jolt(position), ::JPH::Quat::sIdentity(), to_jolt_type(type_), to_jolt(type)};

    body_ = body_interface.CreateBody(body_settings);
    body_interface_->AddBody(body_->GetID(), to_activation(type_));
}

auto RigidBody::set_linear_velocity(const Vector3 &linear_velocity) const -> void
{
    body_interface_->SetLinearVelocity(body_->GetID(), to_jolt(linear_velocity));
}

auto RigidBody::position() const -> Vector3
{
    const auto position = body_interface_->GetPosition(body_->GetID());

    return to_native(position);
}

auto RigidBody::rotation() const -> Quaternion
{
    const auto rotation = body_interface_->GetRotation(body_->GetID());

    return to_native(rotation);
}

auto RigidBody::type() const -> RigidBodyType
{
    return type_;
}

auto RigidBody::native_handle() const -> ::JPH::Body *
{
    return body_;
}

}
