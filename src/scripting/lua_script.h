#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "maths/vector3.h"

extern "C"
{
#include <lstate.h>
}

namespace game
{

/**
 * Lua script wrapper. This class is used to load and execute Lua scripts.
 *
 * The API is deliberately verbose, it's designed to be a thin C++ wrapper around the underlying C API. For a safer and
 * easier to use API see ScriptRunner.
 *
 * As scripts may come from user input the APIs have wide contracts.
 */
class LuaScript
{
  public:
    /**
     * Construct a new Lua script.
     *
     * @param source
     *   The source code of the script to load.
     */
    LuaScript(std::string_view source);

    /**
     * Set the function to call.
     *
     * @param name
     *   The name of the Lua function to call.
     */
    auto set_function(const std::string &name) const -> void;

    /**
     * Execute the script.
     *
     * @param num_args
     *   The number of arguments to pass to the function.
     * @param num_results
     *   The number of results to expect from the function.
     */
    auto execute(std::uint32_t num_args, std::uint32_t num_results) const -> void;

    /**
     * Set the argument to pass to the function.
     *
     * @param value
     *   The value to pass to the function.
     */
    auto set_argument(std::int64_t value) const -> void;

    /**
     * Set the argument to pass to the function.
     *
     * @param value
     *   The value to pass to the function.
     */

    auto set_argument(float value) const -> void;
    /**
     * Set the argument to pass to the function.
     *
     * @param value
     *   The value to pass to the function.
     */
    auto set_argument(std::string_view value) const -> void;

    /**
     * Set the argument to pass to the function.
     *
     * @param value
     *   The value to pass to the function.
     */
    auto set_argument(const Vector3 &value) const -> void;

    /**
     * Get the result of the function.
     *
     * @param result
     *   The result of the function.
     */
    auto get_result(std::int64_t &result) const -> void;

    /**
     * Get the result of the function.
     *
     * @param result
     *   The result of the function.
     */
    auto get_result(float &result) const -> void;

    /**
     * Get the result of the function.
     *
     * @param result
     *   The result of the function.
     */
    auto get_result(std::string &result) const -> void;

    /**
     * Get the result of the function.
     *
     * @param result
     *   The result of the function.
     */
    auto get_result(Vector3 &result) const -> void;

    /**
     * Get the result of the function.
     *
     * @param result
     *   The result of the function.
     */
    auto get_result(bool &result) const -> void;

    /**
     * Format the result of the function as a string. Prints the Lua stack.
     *
     * @returns
     *   The result of the function as a string.
     */
    auto to_string() const -> std::string;

  private:
    /** The Lua state. */
    std::unique_ptr<::lua_State, decltype(&::lua_close)> lua_;
};

/**
 * Value type wrapper for the lua state. This is because you cannot specialise std::formatter with pointer types.
 */
struct LuaStateObjWrapper
{
    ::lua_State *state;
};

/**
 * Format the lua state as a string.
 *
 * @param state
 *  The lua state to format.
 */
auto to_string(LuaStateObjWrapper state) -> std::string;

}
