#ifndef ENV_CLEANUP_STRUCTURE_HPP
#define ENV_CLEANUP_STRUCTURE_HPP


ENV_NAMESPACE_BEGIN
ENV_TEST_BEGIN

[[maybe_unused]] inline constexpr static auto unused_include_workaround{true};

ENV_TEST_END
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

#undef ENV_DETAIL_END
#undef ENV_DETAIL_BEGIN

#undef ENV_TEST_END
#undef ENV_TEST_BEGIN

#undef ENV_BENCH_END
#undef ENV_BENCH_BEGIN

#undef ENV_CUSTOM_NAMESPACE_BEGIN
#undef ENV_CUSTOM_NAMESPACE_END

#undef ENV_FORMAT_NAMESPACE


// global suppress

#undef ENV_SUPPRESS_PUSH
#undef ENV_SUPPRESS_POP
#define ENV_SUPPRESS_POP

#include "env_suppress.hpp"

#undef ENV_SUPPRESS_POP


// compiler

#undef ENV_COMPILER_MAJOR
#undef ENV_COMPILER_MINOR
#undef ENV_COMPILER_PATCH
#undef ENV_COMPILER_VER
#undef ENV_COMPILER_NAME

#undef ENV_MSVC
#undef ENV_CLANG
#undef ENV_GCC

// arch

#undef ENV_ARCH_NAME

#undef ENV_X64
#undef ENV_X86
#undef ENV_ARM

// os

#undef ENV_OS_NAME

#undef ENV_WIN
#undef ENV_WIN32
#undef ENV_WIN64

#undef ENV_APPLE
#undef ENV_MAC
#undef ENV_IOS
#undef ENV_IOS_SIM

#undef ENV_LINUX
#undef ENV_ANDROID

// standard

#undef ENV_CPP

#undef ENV_CPP98
#undef ENV_CPP11
#undef ENV_CPP14
#undef ENV_CPP17
#undef ENV_CPP20
#undef ENV_CPP23

// pragma

#undef ENV_PRAGMA

#undef ENV_MESSAGE

#undef ENV_CLANG_SUPPRESS_PUSH
#undef ENV_CLANG_SUPPRESS_POP

#undef ENV_GCC_SUPPRESS_PUSH
#undef ENV_GCC_SUPPRESS_POP

#undef ENV_MSVC_SUPPRESS_PUSH
#undef ENV_MSVC_SUPPRESS_POP


// tokens

// basic

#undef SKIP
#undef EMPTY
#undef COMMA
#undef OPEN
#undef CLOSE

#undef SINGLE
#undef PACK
#undef ERASE
#undef CONSUME

// strings

#undef CAT_IMPL
#undef CAT

#undef INTER_IMPL
#undef INTER

#undef STRING_IMPL
#undef STRING_VAR
#undef STRING

// spread

#undef SPREAD_IMPL
#undef SPREAD

// uid

#undef UID


#endif // ENV_CLEANUP_STRUCTURE_HPP
