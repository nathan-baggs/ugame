#include "scripting/vector3_interop.h"

extern "C"
{
#include <lauxlib.h>
#include <lstate.h>
#include <lua.h>
}

#include "maths/vector3.h"
#include "scripting/lua_script.h"
#include "utils/error.h"
#include "utils/formatter.h"

namespace
{

/**
 * Helper funcction to pop a Vector3 from the Lua stack.
 *
 * @param state
 *   The Lua state to pop the Vector3 from.
 *
 * @returns
 *   The Vector3 popped from the Lua stack.
 */
auto pop_vector3(::lua_State *state) -> game::Vector3
{
    auto wrapped_state = game::LuaStateObjWrapper{state};

    game::ensure(::lua_type(state, -1) == LUA_TTABLE, "no table at top of stack\n{}", wrapped_state);

    auto result = game::Vector3{};

    game::ensure(::lua_getfield(state, -1, "x") == LUA_TNUMBER, "could not get x field\n{}", wrapped_state);
    result.x = static_cast<float>(::lua_tonumber(state, -1));
    ::lua_pop(state, 1);

    game::ensure(::lua_getfield(state, -1, "y") == LUA_TNUMBER, "could not get y field\n{}", wrapped_state);
    result.y = static_cast<float>(::lua_tonumber(state, -1));
    ::lua_pop(state, 1);

    game::ensure(::lua_getfield(state, -1, "z") == LUA_TNUMBER, "could not get z field\n{}", wrapped_state);
    result.z = static_cast<float>(::lua_tonumber(state, -1));
    ::lua_pop(state, 1);

    ::lua_pop(state, 1);

    return result;
}

/**
 * Helper function to push a Vector3 to the Lua stack.
 *
 * @param state
 *   The Lua state to push the Vector3 to.
 * @param v
 *   The Vector3 to push to the Lua stack.
 */
auto push_vector3(::lua_State *state, const game::Vector3 &v) -> void
{
    ::lua_newtable(state);
    ::luaL_setmetatable(state, "Vector3");

    ::lua_pushnumber(state, v.x);
    ::lua_setfield(state, -2, "x");

    ::lua_pushnumber(state, v.y);
    ::lua_setfield(state, -2, "y");

    ::lua_pushnumber(state, v.z);
    ::lua_setfield(state, -2, "z");
}

}

namespace game
{

auto vector3_constructor(::lua_State *state) -> int
{
    auto wrapped_state = LuaStateObjWrapper{state};
    ensure(::lua_isnumber(state, -3) == 1, "stack index 1 not number:\n{}", wrapped_state);
    const auto x = static_cast<float>(::lua_tonumber(state, -3));
    ensure(::lua_isnumber(state, -2) == 1, "stack index 2 not number:\n{}", wrapped_state);
    const auto y = static_cast<float>(::lua_tonumber(state, -2));
    ensure(::lua_isnumber(state, -1) == 1, "stack index 3 not number:\n{}", wrapped_state);
    const auto z = static_cast<float>(::lua_tonumber(state, -1));

    ::lua_pop(state, 3);

    push_vector3(state, {x, y, z});

    return 1;
}

auto vector3_add(::lua_State *state) -> int
{
    auto wrapped_state = LuaStateObjWrapper{state};
    ensure(::lua_gettop(state) >= 2, "no results to get\n{}", wrapped_state);

    const auto vec2 = pop_vector3(state);
    const auto vec1 = pop_vector3(state);
    push_vector3(state, vec1 + vec2);

    return 1;
}

auto vector3_sub(::lua_State *state) -> int
{
    auto wrapped_state = LuaStateObjWrapper{state};
    ensure(::lua_gettop(state) >= 2, "no results to get\n{}", wrapped_state);

    const auto vec2 = pop_vector3(state);
    const auto vec1 = pop_vector3(state);
    push_vector3(state, vec1 - vec2);

    return 1;
}

auto vector3_mul(::lua_State *state) -> int
{
    auto wrapped_state = LuaStateObjWrapper{state};
    ensure(::lua_gettop(state) >= 2, "no results to get\n{}", wrapped_state);

    const auto vec2 = pop_vector3(state);
    const auto vec1 = pop_vector3(state);
    push_vector3(state, vec1 * vec2);

    return 1;
}

auto vector3_unm(::lua_State *state) -> int
{
    auto wrapped_state = LuaStateObjWrapper{state};
    ensure(::lua_gettop(state) >= 1, "no results to get\n{}", wrapped_state);

    const auto vec = pop_vector3(state);
    push_vector3(state, -vec);

    return 1;
}

auto vector3_eq(::lua_State *state) -> int
{
    auto wrapped_state = LuaStateObjWrapper{state};
    ensure(::lua_gettop(state) >= 2, "no results to get\n{}", wrapped_state);

    const auto vec2 = pop_vector3(state);
    const auto vec1 = pop_vector3(state);
    ::lua_pushboolean(state, vec1 == vec2);

    return 1;
}

auto vector3_tostring(::lua_State *state) -> int
{
    auto wrapped_state = LuaStateObjWrapper{state};
    ensure(::lua_gettop(state) >= 1, "no results to get\n{}", wrapped_state);

    const auto vec = pop_vector3(state);
    ::lua_pushstring(state, vec.to_string().c_str());

    return 1;
}

auto vector3_distance(::lua_State *state) -> int
{
    auto wrapped_state = LuaStateObjWrapper{state};
    ensure(::lua_gettop(state) >= 2, "no results to get\n{}", wrapped_state);

    const auto vec2 = pop_vector3(state);
    const auto vec1 = pop_vector3(state);
    ::lua_pushnumber(state, Vector3::distance(vec1, vec2));

    return 1;
}

}
