#pragma once

#include <array>
#include <span>

#include "maths/frustum_plane.h"
#include "maths/matrix4.h"
#include "maths/vector3.h"

namespace game
{

/**
 * Camera class to represent a camera in 3D space. This what the world is rendered from.
 */
class Camera
{
  public:
    /**
     * Construct a new Camera object.
     *
     * @param position
     *   The position of the camera in world space.
     * @param look_at
     *   The point in world space that the camera is looking at.
     * @param up
     *   The up vector of the camera in world space.
     * @param fov
     *   The field of view of the camera in radians.
     * @param width
     *   The width of the camera view in pixels.
     * @param height
     *   The height of the camera view in pixels.
     * @param near_plane
     *   The distance to the near clipping plane.
     * @param far_plane
     *   The distance to the far clipping plane.
     */
    Camera(
        const Vector3 &position,
        const Vector3 &look_at,
        const Vector3 &up,
        float fov,
        float width,
        float height,
        float near_plane,
        float far_plane);

    /**
     * Get the position of the camera in world space.
     *
     * @returns
     *  The position of the camera in world space.
     */
    auto position() const -> Vector3;

    /**
     * Set the position of the camera in world space. Will recalculate the view matrix.
     *
     * @param position
     *   The new position of the camera in world space.
     */
    auto set_position(const Vector3 &position) -> void;

    /**
     * Get the direction of the camera in world space.
     *
     * @returns
     *   The direction of the camera in world space.
     */
    auto direction() const -> Vector3;

    /**
     * Get the up vector of the camera in world space.
     *
     * @returns
     *   The up vector of the camera in world space.
     */
    auto up() const -> Vector3;

    /**
     * Get the right vector of the camera in world space.
     *
     * @returns
     *   The right vector of the camera in world space.
     */
    auto right() const -> Vector3;

    /**
     * Adjust the yaw of the camera. Will recalculate the view matrix.
     *
     * @param adjust
     *   The amount to adjust the yaw by in radians.
     */
    auto adjust_yaw(float adjust) -> void;

    /**
     * Adjust the pitch of the camera. Will recalculate the view matrix.
     *
     * @param adjust
     *   The amount to adjust the pitch by in radians.
     */
    auto adjust_pitch(float adjust) -> void;

    /**
     * Translate the camera in world space. Will recalculate the view matrix.
     *
     * @param translation
     *   The translation vector in world space.
     */
    auto translate(const Vector3 &translation) -> void;

    /**
     * Get the view matrix of the camera.
     *
     * @returns
     *   The view matrix of the camera.
     */
    auto view() const -> const Matrix4 &;

    /**
     * Get the projection matrix of the camera.
     *
     * @returns
     *   The projection matrix of the camera.
     */
    auto projection() const -> const Matrix4 &;

    /**
     * Get the field of view of the camera.
     *
     * @returns
     *   The field of view of the camera in radians.
     */
    auto fov() const -> float;

    /**
     * Get the width of the camera view in pixels.
     *
     * @returns
     *   The width of the camera view in pixels.
     */
    auto width() const -> float;

    /**
     * Get the height of the camera view in pixels.
     *
     * @returns
     *   The height of the camera view in pixels.
     */
    auto height() const -> float;

    /**
     * Get the near clipping plane distance of the camera.
     *
     * @returns
     *   The near clipping plane distance of the camera.
     */
    auto near_plane() const -> float;

    /**
     * Get the far clipping plane distance of the camera.
     *
     * @returns
     *   The far clipping plane distance of the camera.
     */
    auto far_plane() const -> float;

    /**
     * Calulates the frustum planes of the camera.
     *
     * @returns
     *   The frustum planes of the camera.
     */
    auto frustum_planes() const -> std::array<FrustumPlane, 6u>;

    /**
     * Calculates the frustum corners of the camera.
     *
     * @returns
     *   The frustum corners of the camera.
     */
    auto frustum_corners() const -> std::array<Vector3, 8u>;

  private:
    /** View matrix of the camera. */
    Matrix4 view_;

    /** Projection matrix of the camera. */
    Matrix4 projection_;

    /** The position of the camera in world space. */
    Vector3 position_;

    /** The direction of the camera in world space. */
    Vector3 direction_;

    /** The up vector of the camera in world space. */
    Vector3 up_;

    /** The right vector of the camera in world space. */
    Vector3 right_;

    /** The yaw of the camera in radians. */
    float pitch_;

    /** The pitch of the camera in radians. */
    float yaw_;

    /** The field of view of the camera in radians. */
    float fov_;

    /** The width of the camera view in pixels. */
    float width_;

    /** The height of the camera view in pixels. */
    float height_;

    /** The near clipping plane distance of the camera. */
    float near_plane_;

    /** The far clipping plane distance of the camera. */
    float far_plane_;
};

}
