#include <env/test.hpp>
#include <gtest/gtest.h>

TEST(env, second)
{
    std::vector<int> ints{10};
    EXPECT_EQ(ints.capacity(), 1);
}
