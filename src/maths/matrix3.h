#pragma once

#include <array>
#include <format>
#include <ranges>
#include <span>

#include "maths/vector3.h"
#include "utils/error.h"

namespace game
{

/**
 * A 3x3 matrix. Elements are stored in column-major order, i.e. the same as OpenGL expects.
 */
class Matrix3
{
  public:
    /**
     * Construct the identity matrix.
     */
    constexpr Matrix3()
        : elements_({
              1.0f,
              0.0f,
              0.0f,
              0.0f,
              1.0f,
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
    constexpr Matrix3(const std::array<float, 9u> &elements)
        : elements_(elements)
    {
    }

    /**
     * Construct a matrix from the given vectors.
     *
     * @param v1
     *   The first column of the matrix.
     * @param v2
     *   The second column of the matrix.
     * @param v3
     *   The third column of the matrix.
     */
    constexpr Matrix3(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
        : elements_({
              v1.x,
              v1.y,
              v1.z,
              v2.x,
              v2.y,
              v2.z,
              v3.x,
              v3.y,
              v3.z,
          })
    {
    }

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
     * Get a single element of the matrix. It is undefined behaviour if index is not in [0, 8].
     *
     * @param index
     *   The index of the element to get.
     *
     * @returns
     *   The element at the given index.
     */
    constexpr auto operator[](this auto &&self, std::size_t index) -> auto &
    {
        expect(index <= 8, "index out of range");

        return self.elements_[index];
    }

    /**
     * Invert the matrix.
     *
     * @param m
     *   The matrix to invert.
     *
     * @returns
     *   The inverted matrix.
     */
    static constexpr auto invert(const Matrix3 &m) -> Matrix3
    {
        const auto adjoint = Matrix3{{
            (m[4] * m[8]) - (m[5] * m[7]),
            -((m[1] * m[8]) - (m[2] * m[7])),
            (m[1] * m[5]) - (m[2] * m[4]),
            -((m[3] * m[8]) - (m[5] * m[6])),
            (m[0] * m[8]) - (m[2] * m[6]),
            -((m[0] * m[5]) - (m[2] * m[3])),
            (m[3] * m[7]) - (m[4] * m[6]),
            -((m[0] * m[7]) - (m[1] * m[6])),
            (m[0] * m[4]) - (m[1] * m[3]),
        }};

        const auto determinant = (m[0] * m[4] * m[8]) + (m[3] * m[7] * m[2]) + (m[6] * m[1] * m[5]) -
                                 (m[0] * m[7] * m[5]) - (m[3] * m[1] * m[8]) - (m[6] * m[4] * m[2]);

        const auto inverted_values =
            adjoint.data() | std::views::transform([determinant](auto e) { return e / determinant; });

        auto inverted_array = std::array<float, 9u>{};
        std::ranges::copy(inverted_values, std::ranges::begin(inverted_array));

        return {inverted_array};
    }

    /**
     * Transpose the matrix.
     *
     * @param m
     *   The matrix to transpose.
     *
     * @returns
     *   The transposed matrix.
     */
    static constexpr auto transpose(Matrix3 m) -> Matrix3
    {
        std::ranges::swap(m[1], m[3]);
        std::ranges::swap(m[2], m[6]);
        std::ranges::swap(m[5], m[7]);

        return m;
    }

    /**
     * Get a row of the matrix.
     *
     * @param index
     *   The index of the row to get. Undefined behaviour if index is not in [0, 2].
     *
     *  @returns
     *    The row at the given index as a Vector3.
     */
    auto row(std::size_t index) const -> Vector3
    {
        expect(index <= 2, "index out of range");

        return {elements_[index], elements_[index + 3u], elements_[index + 6u]};
    }

    friend constexpr auto operator*=(Matrix3 &m1, const Matrix3 &m2) -> Matrix3 &;

    constexpr auto operator==(const Matrix3 &) const -> bool = default;

    /**
     * Convert the matrix to a string. This is formatted like the mathematical notation.
     *
     * @returns
     *   The matrix as a string.
     */
    auto to_string() const -> std::string;

  private:
    /** The elements of the matrix in column-major order. */
    std::array<float, 9u> elements_;
};

// maths operators

constexpr auto operator*=(Matrix3 &m1, const Matrix3 &m2) -> Matrix3 &
{
    auto result = Matrix3{};
    for (auto i = 0u; i < 3u; ++i)
    {
        for (auto j = 0u; j < 3u; ++j)
        {
            result.elements_[i + j * 3] = 0.0f;
            for (auto k = 0u; k < 3u; ++k)
            {
                result.elements_[i + j * 3] += m1.elements_[i + k * 3] * m2.elements_[k + j * 3];
            }
        }
    }

    m1 = result;
    return m1;
}

constexpr auto operator*(const Matrix3 &m1, const Matrix3 &m2) -> Matrix3
{
    auto tmp{m1};
    return tmp *= m2;
}

constexpr auto operator*(const Matrix3 &m, const Vector3 &v) -> Vector3
{
    return {
        m[0] * v.x + m[3] * v.y + m[6] * v.z,
        m[1] * v.x + m[4] * v.y + m[7] * v.z,
        m[2] * v.x + m[5] * v.y + m[8] * v.z};
}

inline auto Matrix3::to_string() const -> std::string
{
    const auto *d = data().data();
    return std::format("{} {} {}\n{} {} {}\n{} {} {}", d[0], d[3], d[6], d[1], d[4], d[7], d[2], d[5], d[8]);
}
}
