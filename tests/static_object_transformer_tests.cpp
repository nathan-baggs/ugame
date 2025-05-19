#include <gtest/gtest.h>

#include "game/static_object_transformer.h"
#include "maths/vector3.h"

TEST(static_object_transformer, position)
{
    const auto position = game::Vector3{1.0f, 2.0f, 3.0f};
    const auto transformer = game::StaticObjectTransformer{position};

    ASSERT_EQ(transformer.position(), position);
}

TEST(static_object_transformer, update)
{
    const auto position = game::Vector3{1.0f, 2.0f, 3.0f};
    auto transformer = game::StaticObjectTransformer{position};

    transformer.update();

    ASSERT_EQ(transformer.position(), position);
}
