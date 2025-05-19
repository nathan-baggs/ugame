#include <cmath>

#include <gtest/gtest.h>

#include "maths/frustum_plane.h"

TEST(frustum_plane, ctor)
{
    const auto plane = game::FrustumPlane{1.0f, 2.0f, 3.0f, 4.0f};

    const auto expected = game::Vector3::normalise(game::Vector3(1.0f, 2.0f, 3.0f));

    ASSERT_NEAR(plane.normal.x, expected.x, 0.001f);
    ASSERT_NEAR(plane.normal.y, expected.y, 0.001f);
    ASSERT_NEAR(plane.normal.z, expected.z, 0.001f);
    ASSERT_NEAR(plane.distance, 4.0f / std::hypot(1.0f, 2.0f, 3.0f), 0.001f);
}
