#include <gtest/gtest.h>

#include <numbers>

#include "game/graphics/camera_object_transformer.h"
#include "graphics/camera.h"
#include "maths/vector3.h"

TEST(graphics / camera_object_transformer, position)
{
    const auto position = game::Vector3{1.0f, 2.0f, 3.0f};
    const auto graphics / camera = game::graphics / camera{
                                                        {0.0f, 5.0f, 50.0f},
                                                        {0.0f, 0.0f, 0.0f},
                                                        {0.0f, 1.0f, 0.0f},
                                                        std::numbers::pi_v<float> / 4.0f,
                                                        800.0f,
                                                        600.0f,
                                                        0.1f,
                                                        100.0f};
    const auto transformer = game::graphics / cameraObjectTransformer{position, graphics / camera};

    ASSERT_EQ(transformer.position(), position);
}

TEST(graphics / camera_object_transformer, update)
{
    const auto position = game::Vector3{1.0f, 2.0f, 3.0f};
    auto graphics / camera = game::graphics / camera{
                                                  {0.0f, 5.0f, 50.0f},
                                                  {0.0f, 0.0f, 0.0f},
                                                  {0.0f, 1.0f, 0.0f},
                                                  std::numbers::pi_v<float> / 4.0f,
                                                  800.0f,
                                                  600.0f,
                                                  0.1f,
                                                  100.0f};
    auto transformer = game::graphics / cameraObjectTransformer{position, graphics / camera};
    const auto translate = game::Vector3{2.0f, 3.0f, 4.0f};

    graphics / camera.translate(translate);

    transformer.update();

    ASSERT_EQ(transformer.position(), position + translate);
}
