#pragma once

#include "graphics/camera.h"
#include "maths/vector3.h"
#include <tuple>

namespace game
{

struct TransformerResult
{
    Vector3 result;
    bool stop = false;
};

template <class S>
struct ChainBase
{
    virtual constexpr auto go(const Vector3 &, const S &) const -> Vector3
    {
        return {};
    };
};

template <class S, auto... T>
struct Chain final : ChainBase<S>
{
};

template <auto Head, auto... Tail, class S>
struct Chain<S, Head, Tail...> final : ChainBase<S>
{
    constexpr auto go(const Vector3 &in, const S &state) const -> Vector3 override
    {
        const auto [result, stop] = Head(in, state);

        return stop ? result : Chain<S, Tail...>{}.go(result, state);
    }
};

template <auto Head, class S>
struct Chain<S, Head> final : ChainBase<S>
{
    constexpr auto go(const Vector3 &in, const S &state) const -> Vector3 override
    {
        return Head(in, state).result;
    }
};

template <class S>
struct Chain<S> final : ChainBase<S>
{
    constexpr auto go(const Vector3 &in, const S &) const -> Vector3 override
    {
        return in;
    }
};

}
