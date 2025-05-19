#include <cstdint>
#include <gtest/gtest.h>

#include "scripting/lua_script.h"
#include "scripting/script_runner.h"

TEST(script_runner, no_arg_no_return)
{
    auto script = game::LuaScript{R"(
function my_print()
        print("hello world")
end)"};

    const auto runner = game::ScriptRunner{script};

    ::testing::internal::CaptureStdout();
    runner.execute("my_print");
    const auto output = ::testing::internal::GetCapturedStdout();

    ASSERT_EQ(output, "hello world\n");
}

TEST(script_runner, single_arg_no_return)
{
    auto script = game::LuaScript{R"(
function my_print(msg)
        print(msg)
end)"};

    const auto runner = game::ScriptRunner{script};

    ::testing::internal::CaptureStdout();
    runner.execute("my_print", "hello world");
    const auto output = ::testing::internal::GetCapturedStdout();

    ASSERT_EQ(output, "hello world\n");
}

TEST(script_runner, multiple_arg_no_return)
{
    auto script = game::LuaScript{R"(
function my_print(msg, num)
        print(msg .. num)
end)"};

    const auto runner = game::ScriptRunner{script};

    ::testing::internal::CaptureStdout();
    runner.execute("my_print", "hello world ", 11ll);
    const auto output = ::testing::internal::GetCapturedStdout();

    ASSERT_EQ(output, "hello world 11\n");
}

TEST(script_runner, no_arg_single_return)
{
    auto script = game::LuaScript{R"(
function my_print()
        return 42;
end)"};

    const auto runner = game::ScriptRunner{script};

    const auto res = runner.execute<std::int64_t>("my_print");

    ASSERT_EQ(res, 42);
}

TEST(script_runner, no_arg_multple_returns)
{
    auto script = game::LuaScript{R"(
function my_print()
        return 42, 43;
end)"};

    const auto runner = game::ScriptRunner{script};

    const auto res = runner.execute<std::int64_t, std::int64_t>("my_print");

    ASSERT_EQ(res, std::make_tuple(42ll, 43ll));
}

TEST(script_runner, multiple_arg_multple_returns)
{
    auto script = game::LuaScript{R"(
function foo(i, f, s)
        return i + 2, f + (s == "yes" and 2.0 or 3.0), "hello"
end)"};

    const auto runner = game::ScriptRunner{script};

    const auto res = runner.execute<std::int64_t, float, std::string>("foo", 3ll, 4.0f, "yes");

    ASSERT_EQ(res, std::make_tuple(5ll, 6.0f, std::string{"hello"}));
}
