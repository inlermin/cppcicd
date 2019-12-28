#include <iostream>
#include "lib.h"

#include "gtest/gtest.h"

TEST(g_test_version, test_valid_version)
{
    EXPECT_TRUE(version() > 0);
}
