#pragma once

#include "maths/vector3.h"
extern "C"
{
#include <lstate.h>
}

namespace game
{

// lua shaped functions for Vector3 interop

auto vector3_constructor(::lua_State *state) -> int;
auto vector3_add(::lua_State *state) -> int;
auto vector3_sub(::lua_State *state) -> int;
auto vector3_mul(::lua_State *state) -> int;
auto vector3_unm(::lua_State *state) -> int;
auto vector3_eq(::lua_State *state) -> int;
auto vector3_tostring(::lua_State *state) -> int;
auto vector3_distance(::lua_State *state) -> int;

}
