#ifndef ENV_DIRECTIVES_HPP
#define ENV_DIRECTIVES_HPP


/*
 *
 *
 * This file is just a documentation of directives that can be passed to ENV.
 *
 *
 * MACRO DIRECTIVES:
 *
 * ENV_MESSAGES:
 * Display messages about compiler, architecture, OS, and
 * C++ standard during compilation.
 *
 * ENV_UNDEF_<MODULE>:
 * Undefine macros from <MODULE> and its children. <MODULE> can be one of:
 * "OPS", "STACK", "META", "MACROS"
 *
 *
 * TESTING DIRECTIVES:
 *
 * ENV_TESTS:
 * Enable static and runtime tests with doctest.
 *
 * ENV_STATIC_TESTS:
 * Same as ENV_TESTS, but only enable the static ones.
 *
 * ENV_SKIP_TESTS:
 * Skip all tests.
 * 
 *
 * BENCHMARKING DIRECTIVES:
 * 
 * ENV_BENCHMARK:
 * Enable benchmarks with Google benchmark.
 *
 *
 * NAMESPACE DIRECTIVES:
 *
 * ENV_NAMESPACE:
 * If defined, env shall be put in this namespace. It can be a nested namespace.
 *
 *
 * FEATURE DIRECTIVES:
 *
 * ENV_STD_REQUIREMENTS:
 * Enables checks for named standard requirements.
 * It is automatically enabled if ENV_TESTS or ENV_STATIC_TESTS are enabled.
 * See: https://en.cppreference.com/w/cpp/named_req.
 *
 *
 */


#endif // ENV_DIRECTIVES_HPP
