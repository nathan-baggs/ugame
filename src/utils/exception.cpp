#include "exception.h"

#include <cstdint>
#include <format>

namespace game
{

auto Exception::stacktrace() const -> std::string
{
    return std::to_string(trace_);
}

auto Exception::what() const -> std::string
{
    return what_;
}

auto Exception::to_string() const -> std::string
{
    return std::format("{}\n{}", what(), stacktrace());
}
}
