#include <gtest/gtest.h>

#include "maths/vector3.h"
#include "scripting/lua_script.h"
#include "utils/exception.h"

TEST(lua_script, simple_script)
{
    ::testing::internal::CaptureStdout();
    auto script = game::LuaScript{R"(print("hello world"))"};
    const auto output = ::testing::internal::GetCapturedStdout();

    ASSERT_EQ(output, "hello world\n");
}

TEST(lua_script, bad_script)
{
    ASSERT_THROW(game::LuaScript(R"(print("hello world)"), game::Exception);
}

TEST(lua_script, simple_function_call)
{
    auto script = game::LuaScript{R"(
function hello()
        print("hello world")
end)"};

    script.set_function("hello");
    ::testing::internal::CaptureStdout();
    script.execute(0u, 0u);
    const auto output = ::testing::internal::GetCapturedStdout();

    ASSERT_EQ(output, "hello world\n");
}

TEST(lua_script, missing_function_call)
{
    auto script = game::LuaScript{R"(
function hello()
        print("hello world")
end)"};

    ASSERT_THROW(script.set_function("hell"), game::Exception);
}

TEST(lua_script, function_call_single_arg)
{
    auto script = game::LuaScript{R"(
function my_print(msg)
        print(msg)
end)"};

    script.set_function("my_print");
    script.set_argument("hello world");
    ::testing::internal::CaptureStdout();
    script.execute(1u, 0u);
    const auto output = ::testing::internal::GetCapturedStdout();

    ASSERT_EQ(output, "hello world\n");
}

TEST(lua_script, function_call_get_single_arg)
{
    auto script = game::LuaScript{R"(
function get_num()
        return 42
end)"};

    script.set_function("get_num");
    script.execute(0u, 1u);
    auto res = std::int64_t{};
    script.get_result(res);

    ASSERT_EQ(res, 42);
}

TEST(lua_script, function_call_get_no_arg)
{
    auto script = game::LuaScript{R"(
function get_num()
end)"};

    script.set_function("get_num");
    script.execute(0u, 1u);
    auto res = std::int64_t{};
    ASSERT_THROW(script.get_result(res), game::Exception);
}

TEST(lua_script, function_call_multiple_args_multiple_returns)
{
    auto script = game::LuaScript{R"(
function get_nums(val1, val2, val3)
        return val1 + 1, val2 + 2, val3 + 3
end)"};

    script.set_function("get_nums");
    script.set_argument(1ll);
    script.set_argument(2ll);
    script.set_argument(3ll);
    script.execute(3u, 3u);
    auto res1 = std::int64_t{};
    script.get_result(res1);
    auto res2 = std::int64_t{};
    script.get_result(res2);
    auto res3 = std::int64_t{};
    script.get_result(res3);

    ASSERT_EQ(res1, 6);
    ASSERT_EQ(res2, 4);
    ASSERT_EQ(res3, 2);
}

TEST(lua_script, multiple_function_calls)
{
    auto script = game::LuaScript{R"(
function get_num1(num)
        return num + 1
end
function get_num2(num)
        return num + 2
end
    )"};

    {
        script.set_function("get_num1");
        script.set_argument(42ll);
        script.execute(1u, 1u);
        auto res = std::int64_t{};
        script.get_result(res);

        ASSERT_EQ(res, 43);
    }

    {
        script.set_function("get_num2");
        script.set_argument(43ll);
        script.execute(1u, 1u);
        auto res = std::int64_t{};
        script.get_result(res);

        ASSERT_EQ(res, 45);
    }
}

TEST(lua_script, function_call_vector3_arg)
{
    auto script = game::LuaScript{R"(
 function print_v(v)
         print(v.x, v.y, v.z)
 end)"};

    script.set_function("print_v");
    script.set_argument(game::Vector3{1.0f, 2.0f, 3.0f});
    script.execute(1u, 0u);
}
