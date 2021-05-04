#ifndef ENV_MACROS_INCLUDED
#define ENV_MACROS_INCLUDED


// Basic macros ---------------------------------------------------------------

#define ENV_EVAL(...) __VA_ARGS__

#define ENV_CAT_IMPL(_lhs, _rhs) _lhs##_rhs
#define ENV_CAT(_lhs, _rhs)      ENV_CAT_IMPL(_lhs, _rhs)

#define ENV_INTER_IMPL(_lhs, _middle, _rhs) _lhs##_middle##_rhs
#define ENV_INTER(_lhs, _middle, _rhs)      ENV_INTER_IMPL(_lhs, _middle, _rhs)

#define ENV_STRING_IMPL(...) #__VA_ARGS__
#define ENV_STRING_VAR(...)  ENV_STRING_IMPL(__VA_ARGS__)
#define ENV_STRING(...)      ENV_STRING_VAR(__VA_ARGS__)

#define ENV_SPREAD_IMPL(_tuple) ENV_EVAL(ENV_EVAL _tuple)
#define ENV_SPREAD(_tuple)      ENV_SPREAD_IMPL(_tuple)

#define ENV_UID(_prefix) ENV_CAT(__z_env_uid, __COUNTER__)

#define ENV_SEMI static_assert(true, "Require semicolon.")


// Architecture ---------------------------------------------------------------

// https://sourceforge.net/p/predef/wiki/Architectures/
// https://stackoverflow.com/questions/37244202/detect-arm-64-in-preprocessor

#if defined(__amd64__) || defined(_M_AMD64) // x64
    #define ENV_ARCH_NAME "x64"
    #define ENV_X64       1

#elif defined(__i386__) || defined(_M_IX86) // x86
    #define ENV_ARCH_NAME "x86"
    #define ENV_X86       1

#elif defined(__arm__) || defined(_M_ARM) // arm
    #define ENV_ARCH_NAME "arm"
    #define ENV_ARM       1

#elif defined(__aarch64__) || defined(_M_ARM64) // arm64
    #define ENV_ARCH_NAME "arm64"
    #define ENV_ARM64     1

#else // arch
    #error "Unsupported computer architecture."
#endif // arch


#ifndef ENV_X86
    #define ENV_X86 0
#endif // ENV_X86
#ifndef ENV_X64
    #define ENV_X64 0
#endif // ENV_X64
#ifndef ENV_ARM
    #define ENV_ARM 0
#endif // ENV_ARM
#ifndef ENV_ARM64
    #define ENV_ARM64 0
#endif // ENV_ARM64


// Compiler -------------------------------------------------------------------

// GNU/Clang and GNU/MSVC are mutually exclusive,
// but Clang/MSVC are not because of clang-cl...


#if defined(__clang__) && defined(__clang_minor__) // clang


    #define ENV_COMPILER_MAJOR __clang_major__
    #define ENV_COMPILER_MINOR __clang_minor__
    #define ENV_COMPILER_PATCH __clang_patchlevel__
    #define ENV_CLANG          1
    #define ENV_COMPILER_NAME  "clang"


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

#elif defined(__GNUC__) && defined(__GNUC_MINOR__) && \
        defined(__GNUC_PATCHLEVEL__) && !defined(__INTEL_COMPILER) // gnu


    #define ENV_COMPILER_MAJOR __GNUC__
    #define ENV_COMPILER_MINOR __GNUC_MINOR__
    #define ENV_COMPILER_PATCH __GNUC_PATCHLEVEL__
    #define ENV_GNU            1
    #define ENV_COMPILER_NAME  "gnu"


#else // compiler


    #error "Unsupported C++ compiler."


#endif // compiler


#ifndef ENV_CLANG
    #define ENV_CLANG 0
#endif // ENV_CLANG
#ifndef ENV_MSVC
    #define ENV_MSVC 0
#endif // ENV_MSVC
#ifndef ENV_GNU
    #define ENV_GNU 0
#endif // ENV_GNU


#define ENV_COMPILER_VER              \
    ((ENV_COMPILER_MAJOR) *10000000 + \
     (ENV_COMPILER_MINOR) *100000 +   \
     (ENV_COMPILER_PATCH))


// OS -------------------------------------------------------------------------

// https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor


#if defined(WIN32) || defined(_WIN32) || \
        defined(__WIN32__) || defined(__NT__) // win

    #define ENV_WIN 1

    #ifdef _WIN64 // win

        #define ENV_WIN64   1
        #define ENV_OS_NAME "Windows x64"

    #else // win

        #define ENV_WIN32   1
        #define ENV_OS_NAME "Windows x86"

    #endif // win


#elif __APPLE__ // apple

    #define ENV_APPLE 1

    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR // apple

        #define ENV_IOS_SIM 1
        #define ENV_OS_NAME "iOS Simulator"

    #elif TARGET_OS_IPHONE // apple

        #define ENV_IOS     1
        #define ENV_OS_NAME "iOS"

    #elif TARGET_OS_MAC // apple

        #define ENV_MAC     1
        #define ENV_OS_NAME "Mac"

    #else // apple

        #error "Unsupported Apple OS."

    #endif // apple


#elif __linux__ // linux

    #define ENV_LINUX 1

    #ifdef __ANDROID__ // linux

        #define ENV_OS_NAME "Android"
        #define ENV_ANDROID 1

    #else // linux

        #define ENV_OS_NAME "Linux"

    #endif // linux

#else // os

    #error "Unsupported OS."

#endif // os


#ifndef ENV_WIN
    #define ENV_WIN 0
#endif // ENV_WIN

#ifndef ENV_WIN32
    #define ENV_WIN32 0
#endif // ENV_WIN32

#ifndef ENV_WIN64
    #define ENV_WIN64 0
#endif // ENV_WIN64


#ifndef ENV_APPLE
    #define ENV_APPLE 0
#endif // ENV_APPLE

#ifndef ENV_IOS_SIM
    #define ENV_IOS_SIM 0
#endif // ENV_IOS_SIM

#ifndef ENV_IOS
    #define ENV_IOS 0
#endif // ENV_IOS

#ifndef ENV_MAC
    #define ENV_MAC 0
#endif // ENV_MAC


#ifndef ENV_LINUX
    #define ENV_LINUX 0
#endif // ENV_LINUX

#ifndef ENV_ANDROID
    #define ENV_ANDROID 0
#endif // ENV_ANDROID


// Standard -------------------------------------------------------------------

// https://sourceforge.net/p/predef/wiki/Standards/

#ifdef __cplusplus             // language
    #if __cplusplus == 199711L // c++98
        #define ENV_CPP   98
        #define ENV_CPP98 1
        // #error "Unsupported C++ standard."

    #elif __cplusplus == 201103L // c++11
        #define ENV_CPP   11
        #define ENV_CPP11 1
        #error "Unsupported C++ standard."

    #elif __cplusplus == 201402L // c++14
        #define ENV_CPP   14
        #define ENV_CPP14 1
        #error "Unsupported C++ standard."

    #elif __cplusplus == 201703L // c++17
        #define ENV_CPP   17
        #define ENV_CPP17 1

    #elif __cplusplus == 202003L // c++20
        #define ENV_CPP   20
        #define ENV_CPP20 1

    #elif __cplusplus == 202303L // c++23
        #define ENV_CPP   23
        #define ENV_CPP23 1

    #else // c++
        #error "Unsupported C++ standard."
    #endif // c++

#else // language
    #error "Unsupported language."
#endif // language


#ifndef ENV_CPP98
    #define ENV_CPP98 0
#endif // ENV_CPP98
#ifndef ENV_CPP11
    #define ENV_CPP11 0
#endif // ENV_CPP11
#ifndef ENV_CPP14
    #define ENV_CPP14 0
#endif // ENV_CPP14
#ifndef ENV_CPP17
    #define ENV_CPP17 0
#endif // ENV_CPP17
#ifndef ENV_CPP20
    #define ENV_CPP20 0
#endif // ENV_CPP >= 20
#ifndef ENV_CPP23
    #define ENV_CPP23 0
#endif // ENV_CPP23


// Pragma ---------------------------------------------------------------------

#if ENV_CLANG // clang
    #define ENV_PRAGMA_IMPL(_content) _Pragma(_content)

    #define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)

    #define ENV_PRAGMA(...)                        \
        ENV_PRAGMA_STRING(ENV_STRING(__VA_ARGS__)) \
        ENV_SEMI
#endif // clang


#if ENV_GNU // GNU
    #define ENV_PRAGMA_IMPL(_content) _Pragma(_content)

    #define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)

    #define ENV_PRAGMA(...)                        \
        ENV_PRAGMA_STRING(ENV_STRING(__VA_ARGS__)) \
        ENV_SEMI
#endif // GNU


#if ENV_MSVC // GNU
    #define ENV_PRAGMA_IMPL(_content) __pragma(_content)

    #define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)

    #define ENV_PRAGMA(...)            \
        ENV_PRAGMA_STRING(__VA_ARGS__) \
        ENV_SEMI
#endif // GNU


// Messages -------------------------------------------------------------------

#ifdef ENV_COMPILER_MESSAGES // messages

    #if ENV_MSVC // MSVC
        #define ENV_MESSAGE_IMPL(...) ENV_PRAGMA(message(__VA_ARGS__))
        #define ENV_MESSAGE_VAR(...)  ENV_MESSAGE_IMPL(__VA_ARGS__)
        #define ENV_MESSAGE(...)      ENV_MESSAGE_VAR(ENV_STRING(__VA_ARGS__))

        #define ENV_MSVC_MESSAGE(...)  ENV_MESSAGE(__VA_ARGS__)
        #define ENV_CLANG_MESSAGE(...) ENV_SEMI
        #define ENV_GNU_MESSAGE(...)   ENV_SEMI
    #endif // MSVC

    #if ENV_CLANG // clang
        #define ENV_MESSAGE_IMPL(_content) ENV_PRAGMA(message _content)
        #define ENV_MESSAGE_VAR(_content)  ENV_MESSAGE_IMPL(_content)
        #define ENV_MESSAGE(...)           ENV_MESSAGE_VAR(ENV_STRING(__VA_ARGS__))

        #define ENV_MSVC_MESSAGE(...)  ENV_SEMI
        #define ENV_CLANG_MESSAGE(...) ENV_MESSAGE(__VA_ARGS__)
        #define ENV_GNU_MESSAGE(...)   ENV_SEMI
    #endif // clang

    #if ENV_GNU // gnu
        #define ENV_MESSAGE_IMPL(_content) ENV_PRAGMA(message _content)
        #define ENV_MESSAGE_VAR(_content)  ENV_MESSAGE_IMPL(_content)
        #define ENV_MESSAGE(...)           ENV_MESSAGE_VAR(ENV_STRING(__VA_ARGS__))

        #define ENV_MSVC_MESSAGE(...)  ENV_SEMI
        #define ENV_CLANG_MESSAGE(...) ENV_SEMI
        #define ENV_GNU_MESSAGE(...)   ENV_MESSAGE(__VA_ARGS__)
    #endif // gnu

#else // messages
    #define ENV_MESSAGE(...)       ENV_SEMI
    #define ENV_MSVC_MESSAGE(...)  ENV_SEMI
    #define ENV_CLANG_MESSAGE(...) ENV_SEMI
    #define ENV_GNU_MESSAGE(...)   ENV_SEMI

#endif // messages


// Warnings -------------------------------------------------------------------

#if ENV_CLANG // clang
    #define ENV_CLANG_SUPPRESS_PUSH(_warning) \
        ENV_PRAGMA(clang diagnostic push);    \
        ENV_PRAGMA(clang diagnostic ignored _warning)
    #define ENV_CLANG_SUPPRESS_POP \
        ENV_PRAGMA(clang diagnostic pop)
#else // clang
    #define ENV_CLANG_SUPPRESS_PUSH(_warning) ENV_SEMI
    #define ENV_CLANG_SUPPRESS_POP            ENV_SEMI
#endif // clang

#if ENV_MSVC // MSVC
    #define ENV_MSVC_SUPPRESS_PUSH(_warning) \
        ENV_PRAGMA(warning(push));           \
        ENV_PRAGMA(warning(disable           \
                           : _warning))
    #define ENV_MSVC_SUPPRESS_POP \
        ENV_PRAGMA(warning(pop))
#else // MSVC
    #define ENV_MSVC_SUPPRESS_PUSH(_warning) ENV_SEMI
    #define ENV_MSVC_SUPPRESS_POP            ENV_SEMI
#endif // MSVC

#if ENV_GNU // gnu
    #define ENV_GNU_SUPPRESS_PUSH(_warning) \
        ENV_PRAGMA(GCC diagnostic push);    \
        ENV_PRAGMA(GCC diagnostic ignored _warning)
    #define ENV_GNU_SUPPRESS_POP \
        ENV_PRAGMA(GCC diagnostic pop)
#else // gnu
    #define ENV_GNU_SUPPRESS_PUSH(_warning) ENV_SEMI
    #define ENV_GNU_SUPPRESS_POP            ENV_SEMI
#endif // gnu


ENV_CLANG_SUPPRESS_PUSH("-W#pragma-messages");

ENV_MESSAGE(Env Compiler - ENV_COMPILER_NAME, ENV_COMPILER_VER);
ENV_MESSAGE(Env Arch - ENV_ARCH_NAME);
ENV_MESSAGE(Env OS - ENV_OS_NAME);
ENV_MESSAGE(Env Standard - C++ ENV_CPP);

ENV_CLANG_SUPPRESS_POP;


#endif // ENV_MACROS_INCLUDED
