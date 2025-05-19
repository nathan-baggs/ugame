#pragma once

#include <format>
#include <memory>
#include <stacktrace>
#include <string_view>
#include <utility>

#include "utils/auto_release.h"
#include "utils/exception.h"
#include "utils/formatter.h"
#include "utils/log.h"

namespace game
{

/**
 * Verify something is true and terminate if not. Will (eventually) be removed in release. Treat the same as assert.
 *
 * @param predicate
 *   The thing to test.
 * @param msg
 *   The message to print if the predicate is false.
 * @param args
 *   The arguments to format the message with.
 */
#pragma warning(push)
#pragma warning(disable : 4702)
template <class... Args>
auto expect(bool predicate, std::format_string<Args...> msg, Args &&...args) -> void
{
    if (!predicate)
    {
        log::error("{}", std::format(msg, std::forward<Args>(args)...));
        log::error("{}", std::stacktrace::current(2));
        std::terminate();
        std::unreachable();
    }
}
#pragma warning(pop)

/**
 * Verify something is true and throw an exception if not. Use for error handling.
 *
 * @param predicate
 *   The thing to test.
 * @param msg
 *   The message to print if the predicate is false.
 * @param args
 *   The arguments to format the message with.
 */
template <class... Args>
auto ensure(bool predicate, std::format_string<Args...> msg, Args &&...args) -> void
{
    if (!predicate)
    {
        throw Exception(msg, std::forward<Args>(args)...);
    }
}

/**
 * Overload of ensure for checking AutoRelease contains a valid object.
 *
 * @param obj
 *   The object to check.
 * @param msg
 *   The message to print if the predicate is false.
 * @param args
 *   The arguments to format the message with.
 */
template <class T, T Invalid, class... Args>
auto ensure(AutoRelease<T, Invalid> &obj, std::format_string<Args...> msg, Args &&...args) -> void
{
    ensure(!!obj, msg, std::forward<Args>(args)...);
}

/**
 * Overload of ensure for checking unique_ptr contains a valid object.
 *
 * @param obj
 *   The object to check.
 * @param msg
 *   The message to print if the predicate is false.
 * @param args
 *   The arguments to format the message with.
 */
template <class T, class D, class... Args>
auto ensure(std::unique_ptr<T, D> &obj, std::format_string<Args...> msg, Args &&...args) -> void
{
    ensure(!!obj, msg, std::forward<Args>(args)...);
}

}
