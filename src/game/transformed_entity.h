#pragma once

#include <memory>

#include "game/chain.h"
#include "graphics/camera.h"
#include "graphics/entity.h"
#include "maths/aabb.h"
#include "maths/vector3.h"

namespace game
{

struct GameTransformState
{
    const Camera &camera;
    AABB aabb;
    Vector3 last_camera_pos;
};

struct TransformedEntity
{
    Entity entity;
    AABB bounding_box;
    std::unique_ptr<ChainBase<GameTransformState>> transformer_chain;
};

}
