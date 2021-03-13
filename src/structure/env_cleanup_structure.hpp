#ifndef ENV_CLEANUP_STRUCTURE_HPP
#define ENV_CLEANUP_STRUCTURE_HPP

ENV_NAMESPACE_BEGIN
ENV_NAMESPACE_TEST_BEGIN

[[maybe_unused]] inline constexpr static auto unused_include_workaround{true};

ENV_NAMESPACE_TEST_END
ENV_NAMESPACE_END

// features

// std requirements

#undef STD_REQUIRE
#undef STD_REQUIRE_FALSE

// test macros

#undef REQUIRE_EQT
#undef REQUIRE_NET
#undef REQUIRES
#undef REQUIRES_FALSE

#undef ENV_TEST_SUITE_BEGIN
#undef ENV_TEST_SUITE_END
#undef ENV_TEST_CASE
#undef ENV_TEST_CASE_CLASS
#undef ENV_TEST_CASE_TMP

// namespaces

// core -> std

#undef ENV_STD
#undef ENV_NAMESPACE_STD_END
#undef ENV_NAMESPACE_STD_BEGIN

// core -> global

#undef ENV_GLOBAL
#undef ENV_NAMESPACE_GLOBAL_END
#undef ENV_NAMESPACE_GLOBAL_BEGIN

// native test, benchmark

#undef ENV_DTEST
#undef ENV_GBENCH

// env namespaces

#undef ENV
#undef ENV_NAMESPACE_END
#undef ENV_NAMESPACE_BEGIN

// detail, test, format...

#undef ENV_NAMESPACE_DETAIL_END
#undef ENV_NAMESPACE_DETAIL_BEGIN

#undef ENV_NAMESPACE_TEST_END
#undef ENV_NAMESPACE_TEST_BEGIN

#undef ENV_NAMESPACE_BENCH_END
#undef ENV_NAMESPACE_BENCH_BEGIN

#undef ENV_CUSTOM_NAMESPACE_BEGIN
#undef ENV_CUSTOM_NAMESPACE_END

#undef ENV_FORMAT_NAMESPACE

#endif // ENV_CLEANUP_STRUCTURE_HPP
