#pragma once

#include <filesystem>
#include <format>
#include <print>
#include <source_location>
#include <string_view>

#include "utils/formatter.h"

namespace game::log
{

/**
 * Enumeration of log levels.
 */
enum class Level
{
    /** Should only be used for debugging and temporary messages. */
    DEBUG,

    /** Something you might be interested in. */
    INFO,

    /** Something you might need to action. */
    WARN,

    /** Something has gone wrong. */
    ERR
};

/**
 * Print class that can format a message with custom args but also printout the source location.
 */
template <Level L, class... Args>
struct Print
{
    Print(std::format_string<Args...> msg, Args &&...args, std::source_location loc = std::source_location::current())
    {
        auto c = '?';
        if constexpr (L == Level::DEBUG)
        {
            c = 'D';
        }
        else if constexpr (L == Level::INFO)
        {
            c = 'I';
        }
        else if constexpr (L == Level::WARN)
        {
            c = 'W';
        }
        else if constexpr (L == Level::ERR)
        {
            c = 'E';
        }

        const auto path = std::filesystem::path{loc.file_name()};

        std::println(
            "[{}] {}:{} {}", c, path.filename().string(), loc.line(), std::format(msg, std::forward<Args>(args)...));
    }
};

// deduction guide to support default template argument
template <Level L = {}, class... Args>
Print(std::format_string<Args...>, Args &&...) -> Print<L, Args...>;

// aliases for the supported log levels

template <class... Args>
using debug = Print<Level::DEBUG, Args...>;

template <class... Args>
using info = Print<Level::INFO, Args...>;

template <class... Args>
using warn = Print<Level::WARN, Args...>;

template <class... Args>
using error = Print<Level::ERR, Args...>;

}
