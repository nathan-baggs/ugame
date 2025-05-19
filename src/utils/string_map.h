#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>

namespace game
{

struct StringHash
{
    using is_transparent = void;
    std::size_t operator()(const char *str) const
    {
        return std::hash<std::string_view>{}(str);
    }

    std::size_t operator()(std::string_view str) const
    {
        return std::hash<std::string_view>{}(str);
    }

    std::size_t operator()(const std::string &str) const
    {
        return std::hash<std::string_view>{}(str);
    }
};

// std::unordered_map with string keys that supports heterogeneous operations
template <class T>
using StringMap = std::unordered_map<std::string, T, StringHash, std::equal_to<>>;

}
