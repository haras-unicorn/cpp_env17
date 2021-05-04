#include <env/test.hpp>

ENV_TEST_CASE(env, second)
{
    env::std::vector<int> ints{10};
    EXPECT_EQ(ints.capacity(), 1);
}
