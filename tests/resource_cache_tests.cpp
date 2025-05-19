#include <gtest/gtest.h>

#include "resources/resource_cache.h"

TEST(resource_cache, insert)
{
    auto cache = game::ResourceCache<int>{};

    const auto *v = cache.insert<int>("num", 3);

    ASSERT_EQ(*v, 3);
}

TEST(resource_cache, get)
{
    auto cache = game::ResourceCache<int>{};

    cache.insert<int>("num", 3);
    const auto *v = cache.get<int>("num");

    ASSERT_EQ(*v, 3);
}
