#include <print>
#include <ranges>

#include <gtest/gtest.h>

#include "maths/matrix3.h"
#include "maths/vector3.h"

TEST(matrix3, default_constructor)
{
    const auto m = game::Matrix3{};
    const auto expected = game::Matrix3{{1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}};
    ASSERT_EQ(m, expected);
}

TEST(matrix3, value_constructor)
{
    const auto m = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    const auto expected = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    ASSERT_EQ(m, expected);
}

TEST(matrix3, column_constructor)
{
    const auto m = game::Matrix3{{0.5f, 1.0f, 2.0f}, {3.0f, 4.0f, 5.0f}, {6.0f, 7.0f, 8.0f}};
    const auto expected = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    ASSERT_EQ(m, expected);
}

TEST(matrix3, matrix_matrix_multiplication)
{
    const auto m1 = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    const auto m2 = game::Matrix3{{0.5f, 1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f}};

    const auto expected = game::Matrix3{
        {19.750000, 24.000000, 28.500000, 48.250000, 60.000000, 73.500000, 76.750000, 96.000000, 118.50000}};

    ASSERT_EQ(m1 * m2, expected);
}

TEST(matrix3, index)
{
    const auto m = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    ASSERT_EQ(m[0], 0.5f);
    ASSERT_EQ(m[1], 1.0f);
    ASSERT_EQ(m[2], 2.0f);
    ASSERT_EQ(m[3], 3.0f);
    ASSERT_EQ(m[4], 4.0f);
    ASSERT_EQ(m[5], 5.0f);
    ASSERT_EQ(m[6], 6.0f);
    ASSERT_EQ(m[7], 7.0f);
    ASSERT_EQ(m[8], 8.0f);
}

TEST(matrix3, invert)
{
    const auto m = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};

    const auto expected = game::Matrix3{
        {2.000000f, -4.000000f, 2.000000f, -4.000000f, 5.333333f, -2.333333f, 2.000000f, -1.666667f, 0.666667f}};

    const auto res = game::Matrix3::invert(m);
    for (const auto &[i, e] : std::views::enumerate(res.data()))
    {
        EXPECT_NEAR(e, expected[i], 0.001f);
    }
}

TEST(matrix3, transpose)
{
    const auto m = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    const auto expected = game::Matrix3{{0.5f, 3.0f, 6.0f, 1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f}};
    ASSERT_EQ(game::Matrix3::transpose(m), expected);
}

TEST(matrix3, row)
{
    const auto m = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    ASSERT_EQ(m.row(0), game::Vector3(0.5f, 3.0f, 6.0f));
    ASSERT_EQ(m.row(1), game::Vector3(1.0f, 4.0f, 7.0f));
    ASSERT_EQ(m.row(2), game::Vector3(2.0f, 5.0f, 8.0f));
}

TEST(matrix3, matrix_vector3_multiplication)
{
    const auto m = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    const auto v = game::Vector3{0.5, 1.5f, 2.5f};

    const auto expected = game::Vector3{19.750000, 24.000000, 28.500000};
    ASSERT_EQ(m * v, expected);
}

TEST(matrix3, to_string)
{
    const auto m = game::Matrix3{{0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f}};
    const auto expected = std::string{"0.5 3 6\n1 4 7\n2 5 8"};
    ASSERT_EQ(m.to_string(), expected);
}
