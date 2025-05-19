#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "utils/error.h"

TEST(error, expect_fail)
{
    ASSERT_DEATH({ game::expect(false, "something bad"); }, "");
}

TEST(error, ensure_fail)
{
    ASSERT_THROW({ game::ensure(false, "something bad"); }, game::Exception);
}
