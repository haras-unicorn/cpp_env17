#include <env/test.hpp>

ENV_TEST(env, second)
{
    env::std::vector<int> ints{10};
    EXPECT_EQ(ints.capacity(), 1);
}
