#include <iostream>

#include "gtest/gtest.h"

#include "lib.h"

TEST(g_test_version, test_valid_version) {
    EXPECT_TRUE(version() > 0);
}
