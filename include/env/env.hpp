/*
 *
 * Env - environment for all of my C++ projects.
 *
 *
 * Macros you can define to configure the Env:
 *
 * ENV_MESSAGES:
 * Display messages about compiler, architecture, OS, and
 * C++ standard during compilation.
 *
 */


#ifndef ENV_INCLUDED
#define ENV_INCLUDED


// basic macros

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

#define ENV_UID(_prefix) ENV_INTER(_z, _prefix, __COUNTER__)

#define ENV_SEMI using ENV_UID(_env_demand_semicolon) [[maybe_unused]] = void


// arch

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


// intrinsics

// https://docs.microsoft.com/en-us/cpp/intrinsics/compiler-intrinsics?view=msvc-160

// TODO: ARM


#if ENV_X86 // intrinsics

    // pause
    #include <immintrin.h>


#elif ENV_X64 // intrinsics

    // pause
    #include <immintrin.h>


#elif ENV_ARM // intrinsics


#elif ENV_ARM64 // intrinsics


#else // intrinsics

    #error "Unsupported intrinsics."

#endif // intrinsics


// compiler

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


// OS

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


// standard

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


// pragma

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


// messages

#ifdef ENV_MESSAGES // messages

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


// warnings

#if ENV_CLANG || defined(__JETBRAINS_IDE__) // clang
    #define ENV_CLANG_SUPPRESS_PUSH(_warning) \
        ENV_PRAGMA(clang diagnostic push);    \
        ENV_PRAGMA(clang diagnostic ignored _warning)
    #define ENV_CLANG_SUPPRESS_POP \
        ENV_PRAGMA(clang diagnostic pop)
#else // clang
    #define ENV_CLANG_SUPPRESS_PUSH(_warning) ENV_SEMI
    #define ENV_CLANG_SUPPRESS_POP            ENV_SEMI
#endif // clang

#if ENV_MSVC && !defined(__JETBRAINS_IDE__) // MSVC
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

#if ENV_GNU && !defined(__JETBRAINS_IDE__) // gnu
    #define ENV_GNU_SUPPRESS_PUSH(_warning) \
        ENV_PRAGMA(GNU diagnostic push);    \
        ENV_PRAGMA(GNU diagnostic ignored _warning)
    #define ENV_GNU_SUPPRESS_POP \
        ENV_PRAGMA(GNU diagnostic pop)
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


// deps

// std/boost

#include <utility>
#include <type_traits>

#include <memory>
#include <new>
#include <cstring>
#include <iterator>

#include <bitset>

#include <any>
#include <optional>
#include <tuple>

#include <string>
#include <string_view>

#include <stack>
#include <deque>
#include <queue>

#include <list>
#include <forward_list>

#include <array>
#include <vector>

#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <chrono>

#include <cmath>
#include <functional>
#include <algorithm>
#include <exception>

#include <iostream>
#include <filesystem>


#include <boost/hana.hpp>


// other

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <benchmark/benchmark.h>

ENV_CLANG_SUPPRESS_PUSH("-Wdeprecated-declarations");
#include <mimalloc.h>
ENV_CLANG_SUPPRESS_POP;


// namespace

ENV_CLANG_SUPPRESS_PUSH("-Wunknown-pragmas");
ENV_CLANG_SUPPRESS_PUSH("Simplify");
ENV_CLANG_SUPPRESS_PUSH("UnreachableCode");
ENV_CLANG_SUPPRESS_PUSH("google-explicit-constructor");

namespace env
{
// import

using namespace ::std;

namespace chrono
{
using namespace ::std::chrono;
}

namespace pmr
{
using namespace ::std::pmr;
}

namespace meta
{
using namespace ::boost::hana;
} // namespace meta

namespace test
{
using namespace ::testing;
} // namespace test

namespace bench
{
using namespace ::benchmark;
} // namespace bench

namespace literals
{
using namespace ::std::literals;
using namespace ::std::chrono_literals;
using namespace ::std::string_literals;
using namespace ::std::string_view_literals;

using namespace ::boost::hana::literals;
} // namespace literals


// arch

[[maybe_unused]] inline constexpr string_view arch_name = ENV_ARCH_NAME;
[[maybe_unused]] inline constexpr bool        is_x86 = ENV_X86;
[[maybe_unused]] inline constexpr bool        is_x64 = ENV_X64;
[[maybe_unused]] inline constexpr bool        is_arm = ENV_ARM;
[[maybe_unused]] inline constexpr bool        is_arm64 = ENV_ARM64;

enum class arch_value
{
    x86 [[maybe_unused]] = 0b0001,
    x64 [[maybe_unused]] = 0b0010,
    arm [[maybe_unused]] = 0b0100,
    arm64 [[maybe_unused]] = 0b1000,
    other [[maybe_unused]] = 0
};

[[maybe_unused]] struct arch_t
{
    arch_value  value;
    string_view name;

    [[maybe_unused]] inline constexpr operator arch_value() const noexcept
    {
        return value;
    }
} inline constexpr arch{
        arch_value{
                (is_x86 << 0) +
                (is_x64 << 1) +
                (is_arm << 2) +
                (is_arm64 << 3)},
        arch_name};

static_assert(arch != arch_value::other, "Unsupported architecture.");


// compiler

[[maybe_unused]] inline constexpr string_view compiler_name = ENV_COMPILER_NAME;
[[maybe_unused]] inline constexpr bool        is_gnu = ENV_GNU;
[[maybe_unused]] inline constexpr bool        is_clang = ENV_CLANG;
[[maybe_unused]] inline constexpr bool        is_msvc = ENV_MSVC;
[[maybe_unused]] inline constexpr uintmax_t   compiler_major = ENV_COMPILER_MAJOR;
[[maybe_unused]] inline constexpr uintmax_t   compiler_minor = ENV_COMPILER_MINOR;
[[maybe_unused]] inline constexpr uintmax_t   compiler_patch = ENV_COMPILER_PATCH;

enum class compiler_value
{
    gnu = 0b001,
    clang = 0b010,
    msvc = 0b100,
    other = 0
};

[[maybe_unused]] struct compiler_t
{
    string_view    name;
    compiler_value value;

    struct version_t
    {
        [[maybe_unused]] uintmax_t major;
        [[maybe_unused]] uintmax_t minor;
        [[maybe_unused]] uintmax_t patch;
    } version;

    [[maybe_unused]] inline constexpr operator compiler_value() const noexcept
    {
        return value;
    }
} inline constexpr compiler{
        compiler_name,
        compiler_value{
                (is_gnu << 0) +
                (is_clang << 1) +
                (is_msvc << 2)},
        {compiler_major, compiler_minor, compiler_patch}};

static_assert(compiler != compiler_value::other, "Unsupported compiler.");


// os

[[maybe_unused]] inline constexpr string_view os_name = ENV_OS_NAME;
[[maybe_unused]] inline constexpr bool        is_win = ENV_WIN;
[[maybe_unused]] inline constexpr bool        is_win32 = ENV_WIN32;
[[maybe_unused]] inline constexpr bool        is_win64 = ENV_WIN64;
[[maybe_unused]] inline constexpr bool        is_apple = ENV_APPLE;
[[maybe_unused]] inline constexpr bool        is_mac = ENV_MAC;
[[maybe_unused]] inline constexpr bool        is_ios = ENV_IOS;
[[maybe_unused]] inline constexpr bool        is_ios_sim = ENV_IOS_SIM;
[[maybe_unused]] inline constexpr bool        is_linux = ENV_LINUX;
[[maybe_unused]] inline constexpr bool        is_android = ENV_ANDROID;

enum class os_type
{
    win = 0b001,
    apple = 0b010,
    linux = 0b100,
    other = 0
};

enum class win_value
{
    win32 = 0b01,
    win64 = 0b10,
    other = 0
};

enum class apple_value
{
    mac = 0b001,
    ios = 0b010,
    ios_sim = 0b100,
    other = 0
};

enum class linux_value
{
    android = 0b1,
    other = 0
};

enum class other_os_value
{
};

namespace detail
{
inline constexpr auto _os_value() noexcept
{
    if constexpr (is_win)
    {
        return win_value{
                (is_win32 << 0) +
                (is_win64 << 1)};
    }
    else if constexpr (is_apple)
    {
        return apple_value{
                (is_mac << 0) +
                (is_ios << 1) +
                (is_ios_sim << 2)};
    }
    else if constexpr (is_linux)
    {
        return linux_value{
                (is_android << 0)};
    }
    else
    {
        return other_os_value{};
    }
}
} // namespace detail

using os_value =
        conditional_t<
                is_win, win_value,
                conditional_t<
                        is_apple, apple_value,
                        conditional_t<
                                is_linux, linux_value,
                                other_os_value>>>;

[[maybe_unused]] struct os_t
{
    string_view name;
    os_type     type;
    os_value    value;

    [[maybe_unused]] inline constexpr operator os_type() const noexcept
    {
        return type;
    }

    [[maybe_unused]] inline constexpr operator os_value() const noexcept
    {
        return value;
    }
} inline constexpr os{
        os_name,
        os_type{
                (is_win << 0) +
                (is_apple << 1) +
                (is_linux << 2)},
        detail::_os_value()};

static_assert(os != os_type::other, "Unsupported OS type.");
static_assert(os != os_value::other, "Unsupported OS.");
} // namespace env

ENV_CLANG_SUPPRESS_POP;
ENV_CLANG_SUPPRESS_POP;
ENV_CLANG_SUPPRESS_POP;
ENV_CLANG_SUPPRESS_POP;


#endif // ENV_INCLUDED
