#include "gtest/gtest.h"

#include "maths/vector4.h"

TEST(vector4, empty_ctor)
{
    const auto v = game::Vector4{};

    ASSERT_EQ(v.x, 0.0f);
    ASSERT_EQ(v.y, 0.0f);
    ASSERT_EQ(v.z, 0.0f);
    ASSERT_EQ(v.w, 0.0f);
}

TEST(vector4, single_value_ctor)
{
    const auto v = game::Vector4{1.1f};

    ASSERT_EQ(v.x, 1.1f);
    ASSERT_EQ(v.y, 1.1f);
    ASSERT_EQ(v.z, 1.1f);
    ASSERT_EQ(v.w, 1.1f);
}

TEST(vector4, all_components_ctor)
{
    const auto v = game::Vector4{1.1f, 2.2f, 3.3f, 4.4f};

    ASSERT_EQ(v.x, 1.1f);
    ASSERT_EQ(v.y, 2.2f);
    ASSERT_EQ(v.z, 3.3f);
    ASSERT_EQ(v.w, 4.4f);
}

TEST(vector4, dot)
{
    const auto v1 = game::Vector4{1.0f, 2.0f, 3.0f, 4.0f};
    const auto v2 = game::Vector4{5.0f, 6.0f, 7.0f, 8.0f};

    const auto result = game::Vector4::dot(v1, v2);

    ASSERT_EQ(result, 70.0f);
}

TEST(vector4, addition)
{
    const auto v1 = game::Vector4{1.0f, 2.0f, 3.0f, 4.0f};
    const auto v2 = game::Vector4{5.0f, 6.0f, 7.0f, 8.0f};

    ASSERT_EQ(v1 + v2, game::Vector4(6.0f, 8.0f, 10.0f, 12.0f));
}

TEST(vector4, addition_assignment)
{
    auto v = game::Vector4{1.0f, 2.0f, 3.0f, 4.0f};
    v += game::Vector4{5.0f, 6.0f, 7.0f, 8.0f};

    ASSERT_EQ(v, game::Vector4(6.0f, 8.0f, 10.0f, 12.0f));
}

TEST(vector4, subtraction)
{
    const auto v1 = game::Vector4{2.0f, 10.0f, 12.0f, 14.0f};
    const auto v2 = game::Vector4{4.0f, 5.0f, 6.0f, 7.0f};

    ASSERT_EQ(v1 - v2, game::Vector4(-2.0f, 5.0f, 6.0f, 7.0f));
}

TEST(vector4, subtraction_assignment)
{
    auto v = game::Vector4{2.0f, 10.0f, 12.0f, 14.0f};
    v -= game::Vector4{4.0f, 5.0f, 6.0f, 7.0f};

    ASSERT_EQ(v, game::Vector4(-2.0f, 5.0f, 6.0f, 7.0f));
}
