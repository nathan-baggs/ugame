#include <gtest/gtest.h>

#include "maths/vector3.h"
#include "scripting/lua_script.h"
#include "scripting/script_runner.h"

TEST(lua_interop, vector3_constructor_simple)
{
    auto script = game::LuaScript{R"(
function print_vec()
        local v = Vector3(1.0, 2.0, 3.0)
        print(v.x, v.y, v.z)
end
    )"};

    const auto runner = game::ScriptRunner{script};
    runner.execute("print_vec");
}
