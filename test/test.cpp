#include <env/test.hpp>


ENV_MSVC_SUPPRESS_PUSH(6326);

ENV_TEST_CASE(env, second)
{
    std::vector<int> ints{10};
    EXPECT_EQ(ints.capacity(), 1);
}

ENV_MSVC_SUPPRESS_POP;
