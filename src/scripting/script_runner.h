#pragma once

#include <string>
#include <tuple>

#include "scripting/lua_script.h"

namespace game
{

namespace impl
{

/**
 * Recursive template function to call get_result for each element in the tuple. This works in reverse order as that is
 * the order they need to be "popped" from the Lua script.
 *
 * @param script
 *   The script to get results from.
 * @param result_tuple
 *  The tuple to store results into.
 */
template <std::size_t I = 0u, class... R>
auto get_result(LuaScript &script, std::tuple<R...> &result_tuple) -> void
{
    script.get_result(std::get<sizeof...(R) - I - 1>(result_tuple));

    if constexpr (I != sizeof...(R) - 1)
    {
        get_result<I + 1>(script, result_tuple);
    }
}

}

/**
 * Convenience class to run a Lua script. This is a wrapper around the LuaScript class that allows you to set function
 * name, arguments and specify the return types all with one function call. It handles the complexity of calling all the
 * specific Lua functions for you.
 *
 * It has a non-owning reference to the LuaScript object, so you need to ensure that the LuaScript object is alive for
 * the duration of the runner.
 */
class ScriptRunner
{
  public:
    /**
     * Construct a new script runner.
     *
     * @param script
     *   The lua script to wrap and execute.
     */
    ScriptRunner(LuaScript &script)
        : script_(script)
    {
    }

    ScriptRunner(const ScriptRunner &) = delete;
    ScriptRunner &operator=(const ScriptRunner &) = delete;
    ScriptRunner(ScriptRunner &&) = delete;
    ScriptRunner &operator=(ScriptRunner &&) = delete;

    /**
     * Execute a function from the script.
     *
     * This handles multiple return types (as lua supports this as a first class feature). Specify the return arguments
     * as template parameters.
     *
     * @param function_name
     *   The name of the lua function to call.
     * @param args
     *   The arguments to pass to the lua function.
     *
     * @returns
     *   The return type depends on the number of template arguments:
     *     -  0 -> void
     *     -  1 -> the return type
     *     - >1 -> std::tuple of the return types
     */
    template <class... R, class... Args>
    auto execute(const std::string &function_name, Args &&...args) const
    {
        // lambda with fold expression to set the arguments
        const auto set_args = [this](Args &&...args) { (script_.set_argument(args), ...); };

        script_.set_function(function_name);
        set_args(std::forward<Args>(args)...);

        script_.execute(sizeof...(args), sizeof...(R));

        // handle the return types

        if constexpr (sizeof...(R) == 0)
        {
            return;
        }
        else if constexpr (sizeof...(R) == 1)
        {
            auto res = std::tuple<R...>{};
            script_.get_result(std::get<0>(res));
            return std::get<0>(res);
        }
        else
        {
            auto res = std::tuple<R...>{};
            impl::get_result(script_, res);
            return res;
        }
    }

  private:
    LuaScript &script_;
};

}
