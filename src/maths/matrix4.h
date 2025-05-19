#pragma once

#include <array>
#include <format>
#include <ranges>
#include <span>

#include "maths/quaternion.h"
#include "maths/vector3.h"
#include "maths/vector4.h"
#include "utils/error.h"

namespace game
{

/**
 * A 4x4 matrix. Elements are stored in column-major order, i.e. the same as OpenGL expects.
 */
class Matrix4
{
  public:
    // emptry type to disambiguate the constructor
    struct Scale
    {
    };

    /**
     * Construct the identity matrix.
     */
    constexpr Matrix4()
        : elements_({
              1.0f,
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
              0.0f,
              0.0f,
              0.0f,
              1.0f,
          })
    {
    }

    /**
     * Construct a matrix from the given elements.
     *
     * @param elements
     *   The elements of the matrix in column-major order.
     */
    explicit Matrix4(const std::array<float, 16u> &elements)
        : Matrix4{std::span<const float>{elements}}
    {
    }

    /**
     * Construct a matrix from the given elements.
     *
     * @param elements
     *   The elements of the matrix in column-major order. It is undefined behaviour if the size of elements is not 16.
     */
    explicit Matrix4(const std::span<const float> &elements)
        : Matrix4{}
    {
        expect(elements.size() == 16u, "not enough elements");
        std::ranges::copy(elements, std::ranges::begin(elements_));
    }

    /**
     * Construct a translation matrix.
     *
     * @param translation
     *   The translation vector.
     */
    explicit constexpr Matrix4(const Vector3 &translation)
        : elements_({
              1.0f,
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
              1.0f,
          })
    {
    }

    /**
     * Construct a scale matrix. Last argument is used to disambiguate the constructor.
     *
     * @param scale
     *   The scale vector.
     */
    constexpr Matrix4(const Vector3 &scale, Scale)
        : elements_({
              scale.x,
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
              0.0f,
              0.0f,
              0.0f,
              1.0f,
          })
    {
    }

    /**
     * Construct a translation and scale matrix.
     *
     * @param translation
     *   The translation vector.
     * @param scale
     *   The scale vector.
     */
    constexpr Matrix4(const Vector3 &translation, const Vector3 &scale)
        : elements_({
              scale.x,
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
              1.0f,
          })
    {
    }

    /**
     * Construct a rotation matrix.
     *
     * @param rotation
     *   The rotation quaternion.
     */
    constexpr Matrix4(const Quaternion &rotation)
        : Matrix4{}
    {
        elements_[0] = 1.0f - 2.0f * rotation.y * rotation.y - 2.0f * rotation.z * rotation.z;
        elements_[1] = 2.0f * rotation.x * rotation.y + 2.0f * rotation.z * rotation.w;
        elements_[2] = 2.0f * rotation.x * rotation.z - 2.0f * rotation.y * rotation.w;

        elements_[4] = 2.0f * rotation.x * rotation.y - 2.0f * rotation.z * rotation.w;
        elements_[5] = 1.0f - 2.0f * rotation.x * rotation.x - 2.0f * rotation.z * rotation.z;
        elements_[6] = 2.0f * rotation.y * rotation.z + 2.0f * rotation.x * rotation.w;

        elements_[8] = 2.0f * rotation.x * rotation.z + 2.0f * rotation.y * rotation.w;
        elements_[9] = 2.0f * rotation.y * rotation.z - 2.0f * rotation.x * rotation.w;
        elements_[10] = 1.0f - 2.0f * rotation.x * rotation.x - 2.0f * rotation.y * rotation.y;
    }

    /**
     * Construct a look at (view) matrix.
     *
     * @param eye
     *   The position of the camera.
     * @param look_at
     *   The position the camera is looking at.
     * @param up
     *   The up vector of the camera.
     *
     * @returns
     *   The look at matrix.
     */
    static auto look_at(const Vector3 &eye, const Vector3 &look_at, const Vector3 &up) -> Matrix4;

    /**
     * Construct a perspective projection matrix.
     *
     * @param fov
     *   The field of view in radians.
     * @param width
     *   The width of the viewport.
     * @param height
     *   The height of the viewport.
     * @param near_plane
     *   The near plane distance.
     * @param far_plane
     *   The far plane distance.
     *
     * @returns
     *   The perspective projection matrix.
     */
    static auto perspective(float fov, float width, float height, float near_plane, float far_plane) -> Matrix4;

    /**
     * Get the elements of the matrix.
     *
     * @returns
     *   The elements of the matrix in column-major order.
     */
    constexpr auto data() const -> std::span<const float>
    {
        return elements_;
    }

    /**
     * Get a single element of the matrix. It is undefined behaviour if index is not in [0, 15].
     *
     * @param index
     *   The index of the element to get.
     *
     * @returns
     *   The element at the given index.
     */
    constexpr auto operator[](this auto &&self, std::size_t index) -> auto &
    {
        expect(index <= 15, "index out of range");

        return self.elements_[index];
    }

    /**
     * Get a row of the matrix.
     *
     * @param index
     *   The index of the row to get. Undefined behaviour if index is not in [0, 3].
     *
     * @returns
     *   The row at the given index as a Vector4.
     */
    auto row(std::size_t index) const -> Vector4
    {
        ensure(index <= 3, "index out of range");

        return {elements_[index], elements_[index + 4u], elements_[index + 8u], elements_[index + 12u]};
    }

    friend constexpr auto operator*=(Matrix4 &m1, const Matrix4 &m2) -> Matrix4 &;

    constexpr auto operator==(const Matrix4 &) const -> bool = default;

    /**
     * Convert the matrix to a string. This is formatted like the mathematical notation.
     *
     * @returns
     *  The matrix as a string.
     */
    auto to_string() const -> std::string;

  private:
    /** The elements of the matrix in column-major order. */
    std::array<float, 16u> elements_;
};

// maths operators

constexpr auto operator*=(Matrix4 &m1, const Matrix4 &m2) -> Matrix4 &
{
    auto result = Matrix4{};
    for (auto i = 0u; i < 4u; ++i)
    {
        for (auto j = 0u; j < 4u; ++j)
        {
            auto sum = 0.0f;
            for (auto k = 0u; k < 4u; ++k)
            {
                sum += m1.elements_[i + k * 4] * m2.elements_[k + j * 4];
            }
            result.elements_[i + j * 4] = sum;
        }
    }

    m1 = result;
    return m1;
}

constexpr auto operator*(const Matrix4 &m1, const Matrix4 &m2) -> Matrix4
{
    auto tmp{m1};
    return tmp *= m2;
}

inline auto Matrix4::look_at(const Vector3 &eye, const Vector3 &look_at, const Vector3 &up) -> Matrix4
{
    const auto f = Vector3::normalise(look_at - eye);
    const auto up_normalised = Vector3::normalise(up);

    const auto s = Vector3::normalise(Vector3::cross(f, up_normalised));
    const auto u = Vector3::normalise(Vector3::cross(s, f));

    auto m = Matrix4{};
    m.elements_ = {{s.x, u.x, -f.x, 0.0f, s.y, u.y, -f.y, 0.0f, s.z, u.z, -f.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};

    return m * Matrix4{-eye};
}

inline auto Matrix4::perspective(float fov, float width, float height, float near_plane, float far_plane) -> Matrix4
{
    Matrix4 m;

    const auto aspect_ratio = width / height;
    const auto tmp = std::tan(fov / 2.0f);
    const auto t = tmp * near_plane;
    const auto b = -t;
    const auto r = t * aspect_ratio;
    const auto l = b * aspect_ratio;

    m.elements_ = {
        {(2.0f * near_plane) / (r - l),
         0.0f,
         0.0f,
         0.0f,
         0.0f,
         (2.0f * near_plane) / (t - b),
         0.0f,
         0.0f,
         (r + l) / (r - l),
         (t + b) / (t - b),
         -(far_plane + near_plane) / (far_plane - near_plane),
         -1.0f,
         0.0f,
         0.0f,
         -(2.0f * far_plane * near_plane) / (far_plane - near_plane),
         0.0f}};
    return m;
}

inline auto Matrix4::to_string() const -> std::string
{
    const auto *d = data().data();
    return std::format(
        "{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}",
        d[0],
        d[4],
        d[8],
        d[12],
        d[1],
        d[5],
        d[9],
        d[13],
        d[2],
        d[6],
        d[10],
        d[14],
        d[3],
        d[7],
        d[11],
        d[15]);
}
}
