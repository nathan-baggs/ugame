#pragma once

#include "maths/matrix4.h"
#include "maths/vector3.h"

namespace utils
{
inline auto assert_vector3_equal(const game::Vector3 &lhs, const game::Vector3 &rhs, float epsilon = 0.001f) -> void
{
    ASSERT_NEAR(lhs.x, rhs.x, epsilon);
    ASSERT_NEAR(lhs.y, rhs.y, epsilon);
    ASSERT_NEAR(lhs.z, rhs.z, epsilon);
}

inline auto assert_matrix4_equal(const game::Matrix4 &lhs, const game::Matrix4 &rhs, float epsilon = 0.001f) -> void
{
    for (std::size_t i = 0; i < 16; ++i)
    {
        ASSERT_NEAR(lhs[i], rhs[i], epsilon);
    }
}
}
