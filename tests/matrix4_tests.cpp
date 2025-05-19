#include <array>
#include <numbers>

#include <gtest/gtest.h>

#include "maths/matrix4.h"
#include "maths/vector3.h"
#include "utils.h"

TEST(matrix4, identity_ctor)
{
    const auto m = game::Matrix4{};

    const auto expected = std::array<float, 16u>{
        {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};

    const auto result = m.data();
    ASSERT_EQ(result.size(), expected.size());
    ASSERT_EQ(std::ranges::equal(result, expected), true);
}

TEST(matrix4, element_ctor)
{
    const auto expected = std::array<float, 16u>{
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f}};

    const auto m = game::Matrix4{expected};

    const auto result = m.data();
    ASSERT_EQ(result.size(), expected.size());
    ASSERT_EQ(std::ranges::equal(result, expected), true);
}

TEST(matrix4, translation_ctor)
{
    const auto translation = game::Vector3{1.0f, 2.0f, 3.0f};
    const auto m = game::Matrix4{translation};

    const auto expected = game::Matrix4{
        {1.0f,
         0.0f,
         0.0f,
         0.0f,
         0.0f,
         1.0f,
         0.0f,
         0.0f,
         0.0f,
         0.0f,
         1.0f,
         0.0f,
         translation.x,
         translation.y,
         translation.z,
         1.0f}};

    ASSERT_EQ(m, expected);
}

TEST(matrix4, scale_ctor)
{
    const auto scale = game::Vector3{1.0f, 2.0f, 3.0f};
    const auto m = game::Matrix4{scale, game::Matrix4::Scale{}};

    const auto expected = game::Matrix4{
        {scale.x, 0.0f, 0.0f, 0.0f, 0.0f, scale.y, 0.0f, 0.0f, 0.0f, 0.0f, scale.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};

    ASSERT_EQ(m, expected);
}

TEST(matrix4, translation_scale_ctor)
{
    const auto translation = game::Vector3{1.0f, 2.0f, 3.0f};
    const auto scale = game::Vector3{4.0f, 5.0f, 6.0f};
    const auto m = game::Matrix4{translation, scale};

    const auto expected = game::Matrix4{
        {scale.x,
         0.0f,
         0.0f,
         0.0f,
         0.0f,
         scale.y,
         0.0f,
         0.0f,
         0.0f,
         0.0f,
         scale.z,
         0.0f,
         translation.x,
         translation.y,
         translation.z,
         1.0f}};

    ASSERT_EQ(m, expected);
}

TEST(matrix4, quaternion_ctor)
{
    const auto q = game::Quaternion{0.0f, 0.0f, 0.0f, 1.0f};
    const auto m = game::Matrix4{q};

    const auto expected =
        game::Matrix4{{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};

    ASSERT_EQ(m, expected);
}

TEST(matrix4, multiply)
{
    const auto m1 = game::Matrix4{
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f}};
    const auto m2 = game::Matrix4{
        {1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 15.5f, 11.5f, 12.5f, 13.5f, 14.5f, 15.5f, 16.5f}};
    const auto expected = game::Matrix4{
        {104.0f,
         116.0f,
         128.0f,
         140.0f,
         216.0f,
         244.0f,
         272.0f,
         300.0f,
         353.0f,
         402.0f,
         451.0f,
         500.0f,
         440.0f,
         500.0f,
         560.0f,
         620.0f}};
    ASSERT_EQ(m1 * m2, expected);
}

TEST(matrix4, multiply_assign)
{
    auto m = game::Matrix4{
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f}};
    m *= game::Matrix4{
        {1.5f, 2.5f, 3.5f, 4.5f, 5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 15.5f, 11.5f, 12.5f, 13.5f, 14.5f, 15.5f, 16.5f}};
    const auto expected = game::Matrix4{
        {104.0f,
         116.0f,
         128.0f,
         140.0f,
         216.0f,
         244.0f,
         272.0f,
         300.0f,
         353.0f,
         402.0f,
         451.0f,
         500.0f,
         440.0f,
         500.0f,
         560.0f,
         620.0f}};
    ASSERT_EQ(m, expected);
}

TEST(matrix4, look_at)
{
    const auto view = game::Matrix4::look_at({1.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

    const auto expected = game::Matrix4{
        {0.9805f,
         0.0000f,
         0.1961f,
         0.0000f,
         0.0000f,
         1.000000f,
         -0.0000f,
         0.0000f,
         -0.1961f,
         0.0000f,
         0.9805f,
         0.0000f,
         -0.0000f,
         -0.0000f,
         -5.0990f,
         1.0000f}};

    utils::assert_matrix4_equal(view, expected);
}

TEST(matrix4, perspective)
{
    const auto proj = game::Matrix4::perspective(std::numbers::pi_v<float> / 4.0f, 800.0f, 600.0f, 0.1f, 100.0f);

    const auto expected = game::Matrix4{
        {1.810660f,
         0.000000f,
         0.000000f,
         0.000000f,
         0.000000f,
         2.414213f,
         0.000000f,
         0.000000f,
         0.000000f,
         0.000000f,
         -1.002002f,
         -1.000000f,
         0.000000f,
         0.000000f,
         -0.200200f,
         0.000000f}};

    utils::assert_matrix4_equal(proj, expected);
}

TEST(matrix4, index)
{
    const auto m = game::Matrix4{
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f}};
    ASSERT_EQ(m[0], 1.0f);
    ASSERT_EQ(m[1], 2.0f);
    ASSERT_EQ(m[2], 3.0f);
    ASSERT_EQ(m[3], 4.0f);
    ASSERT_EQ(m[4], 5.0f);
    ASSERT_EQ(m[5], 6.0f);
    ASSERT_EQ(m[6], 7.0f);
    ASSERT_EQ(m[7], 8.0f);
    ASSERT_EQ(m[8], 9.0f);
    ASSERT_EQ(m[9], 10.0f);
    ASSERT_EQ(m[10], 11.0f);
    ASSERT_EQ(m[11], 12.0f);
    ASSERT_EQ(m[12], 13.0f);
    ASSERT_EQ(m[13], 14.0f);
    ASSERT_EQ(m[14], 15.0f);
    ASSERT_EQ(m[15], 16.0f);
}

TEST(matrix4, row)
{
    const auto m = game::Matrix4{
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f}};
    ASSERT_EQ(m.row(0), game::Vector4(1.0f, 5.0f, 9.0f, 13.0f));
    ASSERT_EQ(m.row(1), game::Vector4(2.0f, 6.0f, 10.0f, 14.0f));
    ASSERT_EQ(m.row(2), game::Vector4(3.0f, 7.0f, 11.0f, 15.0f));
    ASSERT_EQ(m.row(3), game::Vector4(4.0f, 8.0f, 12.0f, 16.0f));
}
