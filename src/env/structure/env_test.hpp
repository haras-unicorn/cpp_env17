#ifndef ENV_TEST_HPP
#define ENV_TEST_HPP


// doctest entry point

#if defined(ENV_TESTS) || defined(ENV_STATIC_TESTS)
#define DOCTEST_CONFIG_IMPLEMENT
#else // ENV_TESTS
#define DOCTEST_CONFIG_DISABLE
#endif // ENV_TESTS

#if !defined(DOCTEST_LIBRARY_INCLUDED)
#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS

#include <doctest/doctest.h>

#endif // !defined(DOCTEST_LIBRARY_INCLUDED)


// assertion macros

#if defined(ENV_TESTS)
#define REQUIRE_EQT(...) static_assert(ENV_STD::is_same_v<__VA_ARGS__>); DOCTEST_REQUIRE(ENV_STD::is_same_v<__VA_ARGS__>)
#define REQUIRE_NET(...) static_assert(!ENV_STD::is_same_v<__VA_ARGS__>); DOCTEST_REQUIRE_FALSE(ENV_STD::is_same_v<__VA_ARGS__>)
#define REQUIRES(...) static_assert(__VA_ARGS__); DOCTEST_REQUIRE(__VA_ARGS__)
#define REQUIRES_FALSE(...) static_assert(!__VA_ARGS__); DOCTEST_REQUIRE_FALSE(__VA_ARGS__)
#elif defined(ENV_STATIC_TESTS)
#define REQUIRE_EQT(...) static_assert(ENV_STD::is_same_v<__VA_ARGS__>)
#define REQUIRE_NET(...) static_assert(!ENV_STD::is_same_v<__VA_ARGS__>)
#define REQUIRES(...) static_assert(__VA_ARGS__)
#define REQUIRES_FALSE(...) static_assert(!__VA_ARGS__)
#else // ENV_TESTS
#define REQUIRE_EQT(...) static_cast<void>(ENV_STD::is_same_v<__VA_ARGS__>)
#define REQUIRE_NET(...) static_cast<void>(ENV_STD::is_same_v<__VA_ARGS__>)
#define REQUIRES(...) static_cast<void>(__VA_ARGS__)
#define REQUIRES_FALSE(...) static_cast<void>(__VA_ARGS__)
#endif // ENV_TESTS


// test macros

#if defined(ENV_SKIP_TESTS) || defined(ENV_SKIP_TESTS) || (defined(ENV_STATIC_TESTS) && !defined(ENV_TESTS))
#define ENV_TEST_SUITE_BEGIN(_name) DOCTEST_TEST_SUITE_BEGIN(_name * ENV_DTEST::skip(true))
#define ENV_TEST_SUITE_END() DOCTEST_TEST_SUITE_END()
#define ENV_TEST_CASE(_name) DOCTEST_TEST_CASE(_name * ENV_DTEST::skip(true))
#define ENV_TEST_CASE_CLASS(_name) DOCTEST_TEST_CASE_CLASS(_name * ENV_DTEST::skip(true))
#define ENV_TEST_CASE_TMP(_name, _tmp, ...) DOCTEST_TEST_CASE_TEMPLATE(_name * ENV_DTEST::skip(true), _tmp, __VA_ARGS__)
#else // ENV_SKIP_TESTS
#define ENV_TEST_SUITE_BEGIN(_name) DOCTEST_TEST_SUITE_BEGIN(_name)
#define ENV_TEST_SUITE_END() DOCTEST_TEST_SUITE_END()
#define ENV_TEST_CASE(_name) DOCTEST_TEST_CASE(_name)
#define ENV_TEST_CASE_CLASS(_name) DOCTEST_TEST_CASE_CLASS(_name)
#define ENV_TEST_CASE_TMP(_name, _tmp, ...) DOCTEST_TEST_CASE_TEMPLATE(_name, _tmp, __VA_ARGS__)
#endif // ENV_SKIP_TESTS


#endif // ENV_TEST_HPP
