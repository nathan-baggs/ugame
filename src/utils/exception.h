#pragma once

#include <cstdint>
#include <format>
#include <stacktrace>
#include <string>

namespace game
{

/**
 * Custom exception class that stores a stack trace and a message.
 */
class Exception
{
  public:
    /**
     * Constructor a new Exception. Captures the current stack trace.
     *
     * @param what
     *   The message to format.
     * @param args
     *   The arguments to format the message with.
     */
    template <class... Args>
    constexpr Exception(std::format_string<Args...> what, Args &&...args)
        : trace_(std::stacktrace::current(1))
        , what_(std::format(what, std::forward<Args>(args)...))
    {
    }

    /**
     * Constructor a new Exception. Captures the current stack trace but allows to skip a number of frames.
     *
     * @param what
     *   The message to format.
     * @param args
     *   The arguments to format the message with.
     * @param skip
     *   The number of frames to skip in the stack trace.
     */
    template <class... Args>
    constexpr Exception(std::format_string<Args...> what, Args &&...args, std::uint32_t skip)
        : trace_(std::stacktrace::current(skip))
        , what_(std::format(what, std::forward<Args>(args)...))
    {
    }

    /**
     * Get the stack trace of the exception.
     *
     * @returns
     *   The stack trace of the exception.
     */
    auto stacktrace() const -> std::string;

    /**
     * Get the message of the exception.
     *
     * @returns
     *   The message of the exception.
     */
    auto what() const -> std::string;

    /**
     * Convert the exception to a string.
     *
     * @returns
     *   The exception as a string.
     */
    auto to_string() const -> std::string;

  private:
    /** Stack trace of the exception. */
    std::stacktrace trace_;

    /** Message of the exception. */
    std::string what_;
};

}
