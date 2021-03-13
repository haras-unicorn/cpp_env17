#ifndef FWA_CORE_COMPILER_HPP
#define FWA_CORE_COMPILER_HPP


// stolen and modified from doctest.h

// GCC/Clang and GCC/MSVC are mutually exclusive, but Clang/MSVC are not because of clang-cl...
#if defined(_MSC_VER) && defined(_MSC_FULL_VER) // MSVC
#if _MSC_VER == _MSC_FULL_VER / 10000
#define FWA_COMPILER_MAJOR _MSC_VER / 100
#define FWA_COMPILER_MINOR _MSC_VER % 100
#define FWA_COMPILER_PATCH _MSC_FULL_VER % 10000
#else // MSVC
#define FWA_COMPILER_MAJOR _MSC_VER / 100
#define FWA_COMPILER_MINOR (_MSC_FULL_VER / 100000) % 100
#define FWA_COMPILER_PATCH _MSC_FULL_VER % 100000
#endif // MSVC
#define FWA_MSVC 1
#define FWA_COMPILER_NAME "MSVC"

#elif defined(__clang__) && defined(__clang_minor__) // clang
#define FWA_COMPILER_MAJOR __clang_major__
#define FWA_COMPILER_MINOR __clang_minor__
#define FWA_COMPILER_PATCH __clang_patchlevel__
#define FWA_CLANG 1
#define FWA_COMPILER_NAME "clang"

#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__) && !defined(__INTEL_COMPILER) // gcc
#define FWA_COMPILER_MAJOR __GNUC__
#define FWA_COMPILER_MINOR __GNUC_MINOR__
#define FWA_COMPILER_PATCH __GNUC_PATCHLEVEL__
#define FWA_GCC 1
#define FWA_COMPILER_NAME "gcc"

#else // compiler
#error "Unsupported C++ compiler."
#endif // compiler


#ifndef FWA_MSVC
#define FWA_MSVC 0
#endif // FWA_MSVC
#ifndef FWA_CLANG
#define FWA_CLANG 0
#endif // FWA_CLANG
#ifndef FWA_GCC
#define FWA_GCC 0
#endif // FWA_GCC

#define FWA_COMPILER_VER ((FWA_COMPILER_MAJOR)*10000000 + (FWA_COMPILER_MINOR)*100000 + (FWA_COMPILER_PATCH))


#endif // FWA_CORE_COMPILER_HPP
