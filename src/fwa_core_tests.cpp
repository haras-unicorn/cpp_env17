#define FWA_TESTS
#define FWA_MESSAGES

#include "fwa_core.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
#if defined(FWA_TESTS) || defined(FWA_STATIC_TESTS)

    return doctest::Context{argc, argv}.run();

#else // defined(FWA_TESTS)
    return 0;
#endif // defined(FWA_TESTS)
}


#define FWA_UNDEF_MACROS

#include "fwa_cleanup.hpp"

TEST_CASE("user macros && unused include")
{
    REQUIRE(::fwa::core::test::unused_include_workaround);
}

#ifdef SKIP
#error "Macros shouldn't be defined now."
#endif // FWA_UNDEF_META
#ifdef PI
#error "Kinds shouldn't be defined now."
#endif // FWA_UNDEF_STACK
#ifdef DECL_THIS
#error "Operations shouldn't be defined now."
#endif // FWA_UNDEF_META
