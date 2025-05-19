#include <numbers>
#include <print>

#include <gtest/gtest.h>

#include "graphics/camera.h"
#include "maths/matrix4.h"
#include "maths/vector3.h"
#include "utils.h"
#include "utils/formatter.h"

TEST(camera, constructor)
{
    auto camera = game::Camera{
        {0.0f, 10.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f},
        std::numbers::pi_v<float> / 4.0f,
        1920.0f,
        1080.0f,
        0.1f,
        100.0f};

    const auto expected_view =
        game::Matrix4::look_at({0.0f, 10.0f, 0.0f}, camera.position() + camera.direction(), {0.0f, 1.0f, 0.0f});
    const auto expected_projection =
        game::Matrix4::perspective(std::numbers::pi_v<float> / 4.0f, 1920.0f, 1080.0f, 0.1f, 100.0f);

    utils::assert_vector3_equal(camera.position(), game::Vector3(0.0f, 10.0f, 0.0f));
    utils::assert_vector3_equal(camera.direction(), game::Vector3(0.0f, 0.0f, -1.0f));
    utils::assert_vector3_equal(camera.up(), game::Vector3(0.0f, 1.0f, 0.0f));
    utils::assert_vector3_equal(camera.right(), game::Vector3(1.0f, 0.0f, 0.0f));
    ASSERT_EQ(camera.fov(), std::numbers::pi_v<float> / 4.0f);
    ASSERT_EQ(camera.width(), 1920.0f);
    ASSERT_EQ(camera.height(), 1080.0f);
    ASSERT_EQ(camera.near_plane(), 0.1f);
    ASSERT_EQ(camera.far_plane(), 100.0f);
    utils::assert_matrix4_equal(camera.view(), expected_view);
    utils::assert_matrix4_equal(camera.projection(), expected_projection);
}

TEST(camera, adjust_yaw)
{
    auto camera = game::Camera{
        {0.0f, 10.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f},
        std::numbers::pi_v<float> / 4.0f,
        1920.0f,
        1080.0f,
        0.1f,
        100.0f};

    camera.adjust_yaw(std::numbers::pi_v<float> / 2.0f);

    const auto expected_view =
        game::Matrix4::look_at({0.0f, 10.0f, 0.0f}, camera.position() + camera.direction(), {0.0f, 1.0f, 0.0f});

    const auto expected_projection =
        game::Matrix4::perspective(std::numbers::pi_v<float> / 4.0f, 1920.0f, 1080.0f, 0.1f, 100.0f);

    utils::assert_vector3_equal(camera.position(), game::Vector3(0.0f, 10.0f, 0.0f));
    utils::assert_vector3_equal(camera.direction(), game::Vector3(1.0f, 0.0f, 0.0f));
    utils::assert_vector3_equal(camera.up(), game::Vector3(0.0f, 1.0f, 0.0f));
    utils::assert_vector3_equal(camera.right(), game::Vector3(0.0f, 0.0f, 1.0f));
    ASSERT_EQ(camera.fov(), std::numbers::pi_v<float> / 4.0f);
    ASSERT_EQ(camera.width(), 1920.0f);
    ASSERT_EQ(camera.height(), 1080.0f);
    ASSERT_EQ(camera.near_plane(), 0.1f);
    ASSERT_EQ(camera.far_plane(), 100.0f);
    utils::assert_matrix4_equal(camera.view(), expected_view);
    utils::assert_matrix4_equal(camera.projection(), expected_projection);
}

TEST(camera, adjust_pitch)
{
    auto camera = game::Camera{
        {0.0f, 10.0f, 0.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f},
        std::numbers::pi_v<float> / 4.0f,
        1920.0f,
        1080.0f,
        0.1f,
        100.0f};

    camera.adjust_pitch(std::numbers::pi_v<float> / 2.0f);

    const auto expected_view =
        game::Matrix4::look_at({0.0f, 10.0f, 0.0f}, camera.position() + camera.direction(), {0.0f, 1.0f, 0.0f});
    const auto expected_projection =
        game::Matrix4::perspective(std::numbers::pi_v<float> / 4.0f, 1920.0f, 1080.0f, 0.1f, 100.0f);

    utils::assert_vector3_equal(camera.position(), game::Vector3(0.0f, 10.0f, 0.0f));
    utils::assert_vector3_equal(camera.direction(), game::Vector3(0.0f, 1.0f, 0.0f));
    utils::assert_vector3_equal(camera.up(), game::Vector3(0.0f, 0.0f, -1.0f));
    utils::assert_vector3_equal(camera.right(), game::Vector3(-1.0f, 0.0f, 0.0f));
    ASSERT_EQ(camera.fov(), std::numbers::pi_v<float> / 4.0f);
    ASSERT_EQ(camera.width(), 1920.0f);
    ASSERT_EQ(camera.height(), 1080.0f);
    ASSERT_EQ(camera.near_plane(), 0.1f);
    ASSERT_EQ(camera.far_plane(), 100.0f);
    utils::assert_matrix4_equal(camera.view(), expected_view);
    utils::assert_matrix4_equal(camera.projection(), expected_projection);
}
