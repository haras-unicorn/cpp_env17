#include <env/test.hpp>


ENV_MSVC_SUPPRESS_PUSH(6326);

ENV_TEST_CASE(env, second)
{
    EXPECT_EQ(1, 1);
}

ENV_MSVC_SUPPRESS_POP;
