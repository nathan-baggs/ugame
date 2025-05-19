#include "lua_script.h"

#include <memory>
#include <ranges>
#include <sstream>
#include <string_view>

extern "C"
{
#include <lauxlib.h>
#include <lstate.h>
#include <lua.h>
#include <lualib.h>
}

#include "scripting/vector3_interop.h"
#include "utils/error.h"
#include "utils/exception.h"

namespace
{

// wrapper for state needed to load a lua script from a std::string_view
struct LoadData
{
    std::string_view source;
    std::uint32_t counter;
};

/**
 * Callback function to load a lua script. This allows use a std::string_view as the source of the script.
 */
auto load_string_view(::lua_State *, void *data, std::size_t *size) -> const char *
{
    // get or struct back from the void pointer
    auto *load_data = reinterpret_cast<LoadData *>(data);

    // lua will keep calling this function until it returns nullptr
    // as we load everything on the first call we can return nullptr on the second
    if (load_data->counter != 0u)
    {
        return nullptr;
    }

    // bookkeeping
    *size = load_data->source.size();
    load_data->counter++;

    // return the lua script
    return load_data->source.data();
}

}

namespace game
{

LuaScript::LuaScript(std::string_view source)
    : lua_(::luaL_newstate(), &::lua_close)
{
    ensure(lua_, "failed to create lua state\n{}", *this);

    // load default libs
    ::luaL_openlibs(lua_.get());

    // register "global" functions
    lua_register(lua_.get(), "Vector3", &vector3_constructor);
    lua_register(lua_.get(), "distance", &vector3_distance);

    // create new metatable for Vector3, this allows us to overload operators
    ::luaL_newmetatable(lua_.get(), "Vector3");

    // metatable entries
    ::lua_pushcfunction(lua_.get(), &vector3_add);
    ::lua_setfield(lua_.get(), -2, "__add");
    ::lua_pushcfunction(lua_.get(), &vector3_sub);
    ::lua_setfield(lua_.get(), -2, "__sub");
    ::lua_pushcfunction(lua_.get(), &vector3_mul);
    ::lua_setfield(lua_.get(), -2, "__mul");
    ::lua_pushcfunction(lua_.get(), &vector3_unm);
    ::lua_setfield(lua_.get(), -2, "__unm");
    ::lua_pushcfunction(lua_.get(), &vector3_eq);
    ::lua_setfield(lua_.get(), -2, "__eq");
    ::lua_pushcfunction(lua_.get(), &vector3_tostring);
    ::lua_setfield(lua_.get(), -2, "__tostring");

    ::lua_setmetatable(lua_.get(), -1);

    auto load_data = LoadData{.source = source, .counter = 0};

    ensure(
        ::lua_load(lua_.get(), &load_string_view, reinterpret_cast<void *>(&load_data), "lua_script", "t") == LUA_OK,
        "failed to load lua source\n{}",
        *this);

    ensure(::lua_pcall(lua_.get(), 0, 0, 0) == LUA_OK, "failed to initialise script\n{}", *this);
}

auto LuaScript::set_function(const std::string &name) const -> void
{
    const auto ret_type = ::lua_getglobal(lua_.get(), name.c_str());
    ensure(ret_type == LUA_TFUNCTION, "missing function {}\n{}", name, *this);
}

auto LuaScript::execute(std::uint32_t num_args, std::uint32_t num_results) const -> void
{
    ensure(::lua_gettop(lua_.get()) >= static_cast<int>(num_args), "arg count mismatch {}\n{}", num_args, *this);
    if (::lua_pcall(lua_.get(), num_args, num_results, 0) != LUA_OK)
    {
        const auto res = ::lua_tostring(lua_.get(), -1);
        ::lua_pop(lua_.get(), 1);

        throw Exception("failed execute ({})", res);
    }
}

auto LuaScript::set_argument(std::string_view value) const -> void
{
    ::lua_pushlstring(lua_.get(), value.data(), value.size());
}

auto LuaScript::set_argument(std::int64_t value) const -> void
{
    ::lua_pushinteger(lua_.get(), value);
}

auto LuaScript::set_argument(float value) const -> void
{
    ::lua_pushnumber(lua_.get(), value);
}

auto LuaScript::set_argument(const Vector3 &value) const -> void
{
    set_argument(value.x);
    set_argument(value.y);
    set_argument(value.z);

    vector3_constructor(lua_.get());
}

auto LuaScript::get_result(std::int64_t &result) const -> void
{
    ensure(::lua_gettop(lua_.get()) != 0, "no results to get\n{}", *this);
    ensure(::lua_isinteger(lua_.get(), -1) == 1, "result not an integer\n{}", *this);
    result = ::lua_tointeger(lua_.get(), -1);
    ::lua_pop(lua_.get(), 1);
}

auto LuaScript::get_result(float &result) const -> void
{
    ensure(::lua_gettop(lua_.get()) != 0, "no results to get\n{}", *this);
    ensure(::lua_isnumber(lua_.get(), -1) == 1, "result not a float\n{}", *this);
    result = static_cast<float>(::lua_tonumber(lua_.get(), -1));
    ::lua_pop(lua_.get(), 1);
}

auto LuaScript::get_result(std::string &result) const -> void
{
    ensure(::lua_gettop(lua_.get()) != 0, "no results to get\n{}", *this);
    ensure(::lua_isstring(lua_.get(), -1) == 1, "result not a string\n{}", *this);
    result = ::lua_tostring(lua_.get(), -1);
    ::lua_pop(lua_.get(), 1);
}

auto LuaScript::get_result(Vector3 &result) const -> void
{
    ensure(::lua_gettop(lua_.get()) != 0, "no results to get\n{}", *this);
    ensure(::lua_type(lua_.get(), -1) == LUA_TTABLE, "no table at top of stack\n{}", *this);

    ensure(::lua_getfield(lua_.get(), -1, "x") == LUA_TNUMBER, "could not get x field\n{}", *this);
    result.x = static_cast<float>(::lua_tonumber(lua_.get(), -1));
    ::lua_pop(lua_.get(), 1);

    ensure(::lua_getfield(lua_.get(), -1, "y") == LUA_TNUMBER, "could not get y field\n{}", *this);
    result.y = static_cast<float>(::lua_tonumber(lua_.get(), -1));
    ::lua_pop(lua_.get(), 1);

    ensure(::lua_getfield(lua_.get(), -1, "z") == LUA_TNUMBER, "could not get z field\n{}", *this);
    result.z = static_cast<float>(::lua_tonumber(lua_.get(), -1));
    ::lua_pop(lua_.get(), 1);

    ::lua_pop(lua_.get(), 1);
}

auto LuaScript::get_result(bool &result) const -> void
{
    ensure(::lua_gettop(lua_.get()) != 0, "no results to get\n{}", *this);
    ensure(lua_isboolean(lua_.get(), -1) == 1, "result not a bool\n{}", *this);
    result = ::lua_toboolean(lua_.get(), -1);
    ::lua_pop(lua_.get(), 1);
}

auto LuaScript::to_string() const -> std::string
{
    return game::to_string({lua_.get()});
}

auto to_string(LuaStateObjWrapper obj) -> std::string
{
    auto *state = obj.state;
    auto strm = std::stringstream{};

    const auto stack_size = ::lua_gettop(state);
    if (stack_size == 0)
    {
        return "<empty stack>";
    }

    for (const auto index : std::views::iota(1, stack_size + 1) | std::views::reverse)
    {
        const auto type = ::lua_type(state, index);
        switch (type)
        {
            case LUA_TNIL: strm << "LUA_TNIL"; break;
            case LUA_TNUMBER:
            {
                if (::lua_isinteger(state, index) == 1)
                {
                    strm << std::format("LUA_TNUMBER (int) {}", ::lua_tointeger(state, index));
                }
                else
                {
                    strm << std::format("LUA_TNUMBER (float) {}", ::lua_tonumber(state, index));
                }
                break;
            }
            case LUA_TBOOLEAN: strm << std::format("LUA_TBOOLEAN {}", ::lua_toboolean(state, index)); break;
            case LUA_TSTRING: strm << std::format("LUA_TSTRING '{}'", ::lua_tostring(state, index)); break;
            case LUA_TTABLE: strm << "LUA_TTABLE"; break;
            case LUA_TFUNCTION: strm << "LUA_TFUNCTION"; break;
            case LUA_TUSERDATA: strm << "LUA_TUSERDATA"; break;
            case LUA_TTHREAD: strm << "LUA_TTHREAD"; break;
            case LUA_TLIGHTUSERDATA: strm << "LUA_TLIGHTUSERDATA"; break;
            default: strm << "unknown"; break;
        }

        strm << "\n";
    }

    return strm.str();
}

}
