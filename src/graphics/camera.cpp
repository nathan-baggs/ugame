#include "camera.h"

#include <array>
#include <cmath>
#include <numbers>
#include <print>
#include <span>

#include "maths/frustum_plane.h"
#include "maths/matrix4.h"
#include "maths/vector3.h"
#include "utils/log.h"

namespace
{

auto create_direction(float pitch, float yaw) -> game::Vector3
{
    return game::Vector3::normalise(
        {std::cos(yaw) * std::cos(pitch), std::sin(pitch), std::sin(yaw) * std::cos(pitch)});
}

}

namespace game
{

Camera::Camera(
    const Vector3 &position,
    const Vector3 &look_at,
    const Vector3 &up,
    float fov,
    float width,
    float height,
    float near_plane,
    float far_plane)
    : view_(Matrix4::look_at(position, look_at, up))
    , projection_(Matrix4::perspective(fov, width, height, near_plane, far_plane))
    , position_(position)
    , direction_(look_at)
    , up_(up)
    , right_(Vector3::normalise(Vector3::cross(direction_, up_)))
    , pitch_{}
    , yaw_{-std::numbers::pi_v<float> / 2.0f}
    , fov_(fov)
    , width_(width)
    , height_(height)
    , near_plane_(near_plane)
    , far_plane_(far_plane)
{
    direction_ = create_direction(pitch_, yaw_);
    view_ = Matrix4::look_at(position_, position_ + direction_, up_);
    adjust_pitch(0.0f);
}

auto Camera::position() const -> Vector3
{
    return position_;
}

auto Camera::set_position(const Vector3 &position) -> void
{
    position_ = position;
    view_ = Matrix4::look_at(position_, position_ + direction_, up_);
}

auto Camera::direction() const -> Vector3
{
    return direction_;
}

auto Camera::up() const -> Vector3
{
    return up_;
}

auto Camera::right() const -> Vector3
{
    return right_;
}

auto Camera::adjust_yaw(float adjust) -> void
{
    yaw_ += adjust;
    direction_ = create_direction(pitch_, yaw_);

    const auto world_up = Vector3{0.0f, 1.0f, 0.0f};
    right_ = Vector3::normalise(Vector3::cross(direction_, world_up));
    up_ = Vector3::normalise(Vector3::cross(right_, direction_));

    view_ = Matrix4::look_at(position_, position_ + direction_, up_);
}

auto Camera::adjust_pitch(float adjust) -> void
{
    pitch_ += adjust;
    direction_ = create_direction(pitch_, yaw_);

    const auto world_up = Vector3{0.0f, 1.0f, 0.0f};
    right_ = Vector3::normalise(Vector3::cross(direction_, world_up));
    up_ = Vector3::normalise(Vector3::cross(right_, direction_));

    view_ = Matrix4::look_at(position_, position_ + direction_, up_);
}

auto Camera::translate(const Vector3 &translation) -> void
{
    position_ += translation;
    direction_ = create_direction(pitch_, yaw_);
    view_ = Matrix4::look_at(position_, position_ + direction_, up_);
}

auto Camera::view() const -> const Matrix4 &
{
    return view_;
}

auto Camera::projection() const -> const Matrix4 &
{
    return projection_;
}

auto Camera::fov() const -> float
{
    return fov_;
}

auto Camera::width() const -> float
{
    return width_;
}

auto Camera::height() const -> float
{
    return height_;
}

auto Camera::near_plane() const -> float
{
    return near_plane_;
}

auto Camera::far_plane() const -> float
{
    return far_plane_;
}

auto Camera::frustum_planes() const -> std::array<FrustumPlane, 6u>
{
    auto planes = std::array<FrustumPlane, 6u>{};

    const auto view_proj = projection_ * view_;

    return {{
        {view_proj[3] - view_proj[2],
         view_proj[7] - view_proj[6],
         view_proj[11] - view_proj[10],
         view_proj[15] - view_proj[14]},
        {view_proj[3] + view_proj[2],
         view_proj[7] + view_proj[6],
         view_proj[11] + view_proj[10],
         view_proj[15] + view_proj[14]},
        {view_proj[3] + view_proj[0],
         view_proj[7] + view_proj[4],
         view_proj[11] + view_proj[8],
         view_proj[15] + view_proj[12]},
        {view_proj[3] - view_proj[0],
         view_proj[7] - view_proj[4],
         view_proj[11] - view_proj[8],
         view_proj[15] - view_proj[12]},
        {view_proj[3] + view_proj[1],
         view_proj[7] + view_proj[5],
         view_proj[11] + view_proj[9],
         view_proj[15] + view_proj[13]},
        {view_proj[3] - view_proj[1],
         view_proj[7] - view_proj[5],
         view_proj[11] - view_proj[9],
         view_proj[15] - view_proj[13]},
    }};

    return planes;
}

auto Camera::frustum_corners() const -> std::array<Vector3, 8u>
{
    auto corners = std::array<Vector3, 8u>{};

    const auto tan_half_fov = std::tan(fov_ / 2.0f);
    const auto aspect = width_ / height_;

    const auto near_height = 2.0f * tan_half_fov * near_plane_;
    const auto near_width = near_height * aspect;

    const auto far_height = 2.0f * tan_half_fov * far_plane_;
    const auto far_width = far_height * aspect;

    const auto forward = Vector3::normalise(direction_);
    const auto right = Vector3::normalise(Vector3::cross(forward, up_));
    const auto up = Vector3::normalise(Vector3::cross(right, forward));

    const auto near_centre = position_ + direction_ * near_plane_;
    corners[0] = near_centre + up * (near_height / 2.0f) - right * (near_width / 2.0f);
    corners[1] = near_centre + up * (near_height / 2.0f) + right * (near_width / 2.0f);
    corners[2] = near_centre - up * (near_height / 2.0f) + right * (near_width / 2.0f);
    corners[3] = near_centre - up * (near_height / 2.0f) - right * (near_width / 2.0f);

    const auto far_centre = position_ + direction_ * far_plane_;
    corners[4] = far_centre + up * (far_height / 2.0f) - right * (far_width / 2.0f);
    corners[5] = far_centre + up * (far_height / 2.0f) + right * (far_width / 2.0f);
    corners[6] = far_centre - up * (far_height / 2.0f) + right * (far_width / 2.0f);
    corners[7] = far_centre - up * (far_height / 2.0f) - right * (far_width / 2.0f);

    return corners;
}

}
