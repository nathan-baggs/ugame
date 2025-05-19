#include "physics/physics_system.h"

#include <algorithm>
#include <cstdarg>
#include <functional>
#include <memory>
#include <ranges>
#include <set>
#include <thread>

#include <Jolt/Jolt.h>

#include <Jolt/Core/Core.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/MotionType.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/ObjectVsBroadPhaseLayerFilterMask.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/EActivation.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>
#include <utility>

#include "physics/box_shape.h"
#include "physics/character_controller.h"
#include "physics/jolt_utils.h"
#include "physics/rigid_body.h"
#include "physics/sphere_shape.h"
#include "utils/error.h"
#include "utils/exception.h"
#include "utils/log.h"

using namespace JPH::literals;

namespace
{

// various jolt specific types we need to implement

class SimpleBroadPhaseLayer : public ::JPH::BroadPhaseLayerInterface
{
  public:
    virtual ::JPH::uint GetNumBroadPhaseLayers() const override
    {
        return std::to_underlying(game::PhysicsLayer::MAX_LAYER);
    }

    virtual ::JPH::BroadPhaseLayer GetBroadPhaseLayer(::JPH::ObjectLayer layer) const override
    {
        return ::JPH::BroadPhaseLayer(static_cast<::JPH::BroadPhaseLayer::Type>(layer));
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char *GetBroadPhaseLayerName(::JPH::BroadPhaseLayer layer) const override
    {

        auto native_layer = game::PhysicsLayer{layer.GetValue()};

        switch (native_layer)
        {
            using enum game::PhysicsLayer;

            case NON_MOVING: return "NON_MOVING";
            case MOVING: return "MOVING";
            default: throw game::Exception("unknown broad phase layer");
        }
    }

#endif
};

class SimpleObjectVsBroadPhaseLayerFilter : public ::JPH::ObjectVsBroadPhaseLayerFilter
{
  public:
    virtual bool ShouldCollide(::JPH::ObjectLayer layer1, ::JPH::BroadPhaseLayer layer2) const override
    {
        // only handle collisions of either (or both) of the layers are moving
        return std::ranges::any_of(
            std::set{game::PhysicsLayer{layer1}, game::PhysicsLayer{layer2.GetValue()}},
            [](const auto &e) { return e == game::PhysicsLayer::MOVING; });
    }
};

class SimpleObjectLayerPairFilter : public ::JPH::ObjectLayerPairFilter
{
  public:
    virtual bool ShouldCollide(::JPH::ObjectLayer layer1, ::JPH::ObjectLayer layer2) const override
    {
        // only handle collisions of either (or both) of the layers are moving
        return std::ranges::any_of(
            std::set{game::PhysicsLayer{layer1}, game::PhysicsLayer{layer2}},
            [](const auto &e) { return e == game::PhysicsLayer::MOVING; });
    }
};

auto jolt_trace(const char *fmt, ...)
{
    // gross but this is what jolt gives us
    va_list list;
    va_start(list, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, list);
    va_end(list);

    game::log::info("jolt trace: {}", buffer);
}

}

namespace game
{

struct PhysicsSystem::implementation
{
    SimpleBroadPhaseLayer broad_phase_layer;
    SimpleObjectVsBroadPhaseLayerFilter object_vs_broadphase_layer_filter;
    SimpleObjectLayerPairFilter object_layer_pair_filter;
    ::JPH::TempAllocatorImpl temp_allocator = ::JPH::TempAllocatorImpl(10u * 1024u * 1024u);
    ::JPH::JobSystemThreadPool job_system = ::JPH::JobSystemThreadPool(
        ::JPH::cMaxPhysicsJobs,
        ::JPH::cMaxPhysicsBarriers,
        std::thread::hardware_concurrency() - 1u);
    ::JPH::PhysicsSystem physics_system;
    ::JPH::BodyID sphere;
    DebugRenderer debug_renderer = {{}};
    std::unique_ptr<CharacterController> character_controller;
};

PhysicsSystem::PhysicsSystem()
    : impl_{}
{
    // jolt has some global setup we need to only do once
    static bool once = false;
    if (!once)
    {
        once = true;

        ::JPH::RegisterDefaultAllocator();
        ::JPH::Trace = jolt_trace;

        ::JPH::Factory::sInstance = new ::JPH::Factory{};

        ::JPH::RegisterTypes();
    }

    impl_ = std::make_unique<implementation>();

    // hard coded limites for now
    constexpr auto max_bodies = 1024u;
    constexpr auto num_body_mutexes = 0u;
    constexpr auto max_body_pairs = 1024u;
    constexpr auto max_contact_constraints = 1024u;

    impl_->physics_system.Init(
        max_bodies,
        num_body_mutexes,
        max_body_pairs,
        max_contact_constraints,
        impl_->broad_phase_layer,
        impl_->object_vs_broadphase_layer_filter,
        impl_->object_layer_pair_filter);

    impl_->physics_system.SetGravity({0.0f, -9.8f, 0.0f});

    impl_->character_controller =
        std::make_unique<CharacterController>(std::addressof(impl_->physics_system), PassKey<PhysicsSystem>());
}

PhysicsSystem::~PhysicsSystem() = default;

auto PhysicsSystem::create_rigid_body(const Shape &shape, const Vector3 &position, RigidBodyType type) const
    -> RigidBody
{
    auto &body_interface = impl_->physics_system.GetBodyInterface();
    return {shape, position, type, body_interface, {}};
}

auto PhysicsSystem::update() -> void
{
    impl_->debug_renderer.clear();

    impl_->character_controller->update(
        1.0f / 60.0f, // hard coded timestep
        impl_->physics_system.GetDefaultBroadPhaseLayerFilter(to_jolt(RigidBodyType::DYNAMIC)),
        impl_->physics_system.GetDefaultLayerFilter(to_jolt(RigidBodyType::DYNAMIC)),
        {});

    impl_->physics_system.Update(1.0f / 60.0f, 1, &impl_->temp_allocator, &impl_->job_system);

    static const auto settings = ::JPH::BodyManager::DrawSettings{};
    impl_->physics_system.DrawBodies(settings, &impl_->debug_renderer);
    impl_->character_controller->debug_draw(&impl_->debug_renderer, {});
}

auto PhysicsSystem::debug_renderer() const -> const DebugRenderer &
{
    return impl_->debug_renderer;
}

auto PhysicsSystem::character_controller() const -> CharacterController &
{
    return *impl_->character_controller;
}

}
