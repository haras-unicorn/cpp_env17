#define ENV_TESTS
#define ENV_MESSAGES

#include "env_core.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
#if defined(ENV_TESTS) || defined(ENV_STATIC_TESTS)

    return doctest::Context{argc, argv}.run();

#else  // defined(ENV_TESTS)
    return 0;
#endif // defined(ENV_TESTS)
}

#define ENV_UNDEF_MACROS

#include "env_cleanup.hpp"

TEST_CASE("user macros && unused include")
{
    REQUIRE(::fwa::core::test::unused_include_workaround);
}

#ifdef SKIP
#error "Macros shouldn't be defined now."
#endif // ENV_UNDEF_META
#ifdef PI
#error "Kinds shouldn't be defined now."
#endif // ENV_UNDEF_STACK
#ifdef DECL_THIS
#error "Operations shouldn't be defined now."
#endif // ENV_UNDEF_META
