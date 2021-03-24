#ifndef ENV_COMPILER_HPP
#define ENV_COMPILER_HPP


// stolen and modified from doctest.h

// GCC/Clang and GCC/MSVC are mutually exclusive, but Clang/MSVC are not because of clang-cl...
#if defined(__clang__) && defined(__clang_minor__) // clang
#define ENV_COMPILER_MAJOR __clang_major__
#define ENV_COMPILER_MINOR __clang_minor__
#define ENV_COMPILER_PATCH __clang_patchlevel__
#define ENV_CLANG 1
#define ENV_COMPILER_NAME "clang"

#elif defined(_MSC_VER) && defined(_MSC_FULL_VER) // MSVC
#if _MSC_VER == _MSC_FULL_VER / 10000
#define ENV_COMPILER_MAJOR _MSC_VER / 100
#define ENV_COMPILER_MINOR _MSC_VER % 100
#define ENV_COMPILER_PATCH _MSC_FULL_VER % 10000
#else // MSVC
#define ENV_COMPILER_MAJOR _MSC_VER / 100
#define ENV_COMPILER_MINOR (_MSC_FULL_VER / 100000) % 100
#define ENV_COMPILER_PATCH _MSC_FULL_VER % 100000
#endif // MSVC
#define ENV_MSVC 1
#define ENV_COMPILER_NAME "MSVC"

#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__) && !defined(__INTEL_COMPILER) // gcc
#define ENV_COMPILER_MAJOR __GNUC__
#define ENV_COMPILER_MINOR __GNUC_MINOR__
#define ENV_COMPILER_PATCH __GNUC_PATCHLEVEL__
#define ENV_GCC 1
#define ENV_COMPILER_NAME "gcc"

#else // compiler
#error "Unsupported C++ compiler."
#endif // compiler

#ifndef ENV_CLANG
#define ENV_CLANG 0
#endif // ENV_CLANG
#ifndef ENV_MSVC
#define ENV_MSVC 0
#endif // ENV_MSVC
#ifndef ENV_GCC
#define ENV_GCC 0
#endif // ENV_GCC

#define ENV_COMPILER_VER ((ENV_COMPILER_MAJOR)*10000000 + (ENV_COMPILER_MINOR)*100000 + (ENV_COMPILER_PATCH))


#endif // ENV_COMPILER_HPP
