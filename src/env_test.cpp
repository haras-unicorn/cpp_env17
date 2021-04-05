#define ENV_MESSAGES

#include <env/env.hpp>


TEST(env, deps) // NOLINT(cert-err58-cpp)
{
    using namespace env;
    using namespace env::meta;
    using namespace env::literals;

    constexpr auto constant_one = 1_c;
    EXPECT_EQ((_ + 1)(constant_one), 2);
}


int main(int argc, char** argv)
{
    env::test::InitGoogleMock(&argc, argv);
    env::test::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
