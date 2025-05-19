#pragma once

#include "maths/matrix4.h"
#include "maths/quaternion.h"
#include "maths/vector3.h"

namespace game
{

class Transform
{
  public:
    constexpr Transform(const Vector3 &position, const Vector3 &scale, const Quaternion &rotation)
        : position(position)
        , scale(scale)
        , rotation(rotation)
    {
    }

    constexpr operator Matrix4() const
    {
        return Matrix4{position} * Matrix4{rotation} * Matrix4{scale, Matrix4::Scale{}};
    }

    Vector3 position;
    Vector3 scale;
    Quaternion rotation;
};

};
