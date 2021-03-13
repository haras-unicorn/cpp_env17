#ifndef ENV_TEST_HPP
#define ENV_TEST_HPP

// doctest entry point

#if defined(FWA_TESTS) || defined(FWA_STATIC_TESTS)
#define DOCTEST_CONFIG_IMPLEMENT
#else // FWA_TESTS
#define DOCTEST_CONFIG_DISABLE
#endif // FWA_TESTS

#if !defined(DOCTEST_LIBRARY_INCLUDED)
#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS
#define DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS

#include <doctest/doctest.h>
#endif // !defined(DOCTEST_LIBRARY_INCLUDED)

// assertion macros

#if defined(FWA_TESTS)
#define REQUIRE_EQT(...)                            \
    static_assert(FWA_STD::is_same_v<__VA_ARGS__>); \
    DOCTEST_REQUIRE(FWA_STD::is_same_v<__VA_ARGS__>)
#define REQUIRE_NET(...)                             \
    static_assert(!FWA_STD::is_same_v<__VA_ARGS__>); \
    DOCTEST_REQUIRE_FALSE(FWA_STD::is_same_v<__VA_ARGS__>)
#define REQUIRES(...)           \
    static_assert(__VA_ARGS__); \
    DOCTEST_REQUIRE(__VA_ARGS__)
#define REQUIRES_FALSE(...)      \
    static_assert(!__VA_ARGS__); \
    DOCTEST_REQUIRE_FALSE(__VA_ARGS__)
#elif defined(FWA_STATIC_TESTS)
#define REQUIRE_EQT(...) static_assert(FWA_STD::is_same_v<__VA_ARGS__>)
#define REQUIRE_NET(...) static_assert(!FWA_STD::is_same_v<__VA_ARGS__>)
#define REQUIRES(...) static_assert(__VA_ARGS__)
#define REQUIRES_FALSE(...) static_assert(!__VA_ARGS__)
#else // FWA_TESTS
#define REQUIRE_EQT(...)
#define REQUIRE_NET(...)
#define REQUIRES(...)
#define REQUIRES_FALSE(...)
#endif // FWA_TESTS

// test macros

#if defined(ENV_SKIP_TESTS) || defined(FWA_SKIP_TESTS) || (defined(FWA_STATIC_TESTS) && !defined(FWA_TESTS))
#define ENV_TEST_SUITE_BEGIN(_name) DOCTEST_TEST_SUITE_BEGIN(_name *FWA_DOCT::skip(true))
#define ENV_TEST_SUITE_END() DOCTEST_TEST_SUITE_END()
#define ENV_TEST_CASE(_name) DOCTEST_TEST_CASE(_name *FWA_DOCT::skip(true))
#define ENV_TEST_CASE_CLASS(_name) DOCTEST_TEST_CASE_CLASS(_name *FWA_DOCT::skip(true))
#define ENV_TEST_CASE_TMP(_name, _tmp, ...) DOCTEST_TEST_CASE_TEMPLATE(_name *FWA_DOCT::skip(true), _tmp, __VA_ARGS__)
#else // ENV_SKIP_TESTS
#define ENV_TEST_SUITE_BEGIN(_name) DOCTEST_TEST_SUITE_BEGIN(_name)
#define ENV_TEST_SUITE_END() DOCTEST_TEST_SUITE_END()
#define ENV_TEST_CASE(_name) DOCTEST_TEST_CASE(_name)
#define ENV_TEST_CASE_CLASS(_name) DOCTEST_TEST_CASE_CLASS(_name)
#define ENV_TEST_CASE_TMP(_name, _tmp, ...) DOCTEST_TEST_CASE_TEMPLATE(_name, _tmp, __VA_ARGS__)
#endif // ENV_SKIP_TESTS

#if defined(FWA_GRAPH_SKIP_TESTS) || defined(FWA_SKIP_TESTS) || (defined(FWA_STATIC_TESTS) && !defined(FWA_TESTS))
#define FWA_GRAPH_TEST_SUITE_BEGIN(_name) DOCTEST_TEST_SUITE_BEGIN(_name *FWA_DOCT::skip(true))
#define FWA_GRAPH_TEST_SUITE_END() DOCTEST_TEST_SUITE_END()
#define FWA_GRAPH_TEST_CASE(_name) DOCTEST_TEST_CASE(_name *FWA_DOCT::skip(true))
#define FWA_GRAPH_TEST_CASE_CLASS(_name) DOCTEST_TEST_CASE_CLASS(_name *FWA_DOCT::skip(true))
#define FWA_GRAPH_TEST_CASE_TMP(_name, _tmp, ...) DOCTEST_TEST_CASE_TEMPLATE(_name *FWA_DOCT::skip(true), _tmp, __VA_ARGS__)
#else // FWA_GRAPH_SKIP_TESTS
#define FWA_GRAPH_TEST_SUITE_BEGIN(_name) DOCTEST_TEST_SUITE_BEGIN(_name)
#define FWA_GRAPH_TEST_SUITE_END() DOCTEST_TEST_SUITE_END()
#define FWA_GRAPH_TEST_CASE(_name) DOCTEST_TEST_CASE(_name)
#define FWA_GRAPH_TEST_CASE_CLASS(_name) DOCTEST_TEST_CASE_CLASS(_name)
#define FWA_GRAPH_TEST_CASE_TMP(_name, _tmp, ...) DOCTEST_TEST_CASE_TEMPLATE(_name, _tmp, __VA_ARGS__)
#endif // FWA_GRAPH_SKIP_TESTS

#if defined(FWA_EXEC_SKIP_TESTS) || defined(FWA_SKIP_TESTS) || (defined(FWA_STATIC_TESTS) && !defined(FWA_TESTS))
#define FWA_EXEC_TEST_SUITE_BEGIN(_name) DOCTEST_TEST_SUITE_BEGIN(_name *FWA_DOCT::skip(true))
#define FWA_EXEC_TEST_SUITE_END() DOCTEST_TEST_SUITE_END()
#define FWA_EXEC_TEST_CASE(_name) DOCTEST_TEST_CASE(_name *FWA_DOCT::skip(true))
#define FWA_EXEC_TEST_CASE_CLASS(_name) DOCTEST_TEST_CASE_CLASS(_name *FWA_DOCT::skip(true))
#define FWA_EXEC_TEST_CASE_TMP(_name, _tmp, ...) DOCTEST_TEST_CASE_TEMPLATE(_name *FWA_DOCT::skip(true), _tmp, __VA_ARGS__)
#else // FWA_EXEC_SKIP_TESTS
#define FWA_EXEC_TEST_SUITE_BEGIN(_name) DOCTEST_TEST_SUITE_BEGIN(_name)
#define FWA_EXEC_TEST_SUITE_END() DOCTEST_TEST_SUITE_END()
#define FWA_EXEC_TEST_CASE(_name) DOCTEST_TEST_CASE(_name)
#define FWA_EXEC_TEST_CASE_CLASS(_name) DOCTEST_TEST_CASE_CLASS(_name)
#define FWA_EXEC_TEST_CASE_TMP(_name, _tmp, ...) DOCTEST_TEST_CASE_TEMPLATE(_name, _tmp, __VA_ARGS__)
#endif // FWA_EXEC_SKIP_TESTS

#endif // ENV_TEST_HPP
