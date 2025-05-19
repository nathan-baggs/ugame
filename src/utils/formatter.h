#pragma once

#include <concepts>
#include <format>
#include <string>

namespace game
{

template <class T>
concept HasToStringMember = requires(T a) {
    { a.to_string() } -> std::convertible_to<std::string>;
};

template <class T>
concept HasToStringFree = requires(T a) {
    { to_string(a) } -> std::convertible_to<std::string>;
};

namespace util
{

// CPO to find and call to_string
struct ToStringCPO
{
    template <HasToStringMember T>
    auto operator()(T &&obj) const -> std::string
    {
        return obj.to_string();
    }

    template <class T>
        requires(!HasToStringMember<T> && HasToStringFree<T>)
    auto operator()(T &&obj) const -> std::string
    {
        return to_string(obj);
    }
};

inline constexpr auto to_string = ToStringCPO{};

}

// actual implementation of the formatter
template <class T>
struct Formatter
{
    constexpr auto parse(std::format_parse_context &ctx)
    {
        return std::ranges::begin(ctx);
    }

    auto format(const T &obj, std::format_context &ctx) const
    {
        return std::format_to(ctx.out(), "{}", util::to_string(obj));
    }
};
}

// concept to check if a type can be formatted
template <class T>
concept CanFormat = requires(T a) {
    { game::util::to_string(a) } -> std::convertible_to<std::string>;
};

/**
 * A generic specialisation of std::formatter for any type that has a to_string method or a free function to_string.
 *
 * Look above for implementation details but it uses CPO to find the correct to_string function.
 */
template <CanFormat T>
struct std::formatter<T> : game::Formatter<T>
{
};
