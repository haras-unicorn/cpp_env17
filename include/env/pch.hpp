#ifndef ENV_PCH
#define ENV_PCH


// push warning suppression

#if defined(__clang__) // clang

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"

#elif defined(_MSC_VER) // MSVC

    #pragma warning(push)
    #pragma warning(disable : 4100) // unreferenced formal parameter
    #pragma warning(disable : 4996) // deprecated std::literal_type
    #pragma warning(disable : 4127) // conditional is constant
    #pragma warning(disable : 4267) // conversion from size_t to whatever
    #pragma warning(disable : 4459) // hidden global declaration
    #pragma warning(disable : 4458) // hidden class member
    #pragma warning(disable : 4456) // hidden local declaration
    #pragma warning(disable : 4702) // unreachable codE

    #pragma warning(disable : 5059) // sanitizers not currently supported

    #pragma warning(disable : 6246) // local declaration hides outer scope declaration
    #pragma warning(disable : 6326) // potential comparison of constants
    #pragma warning(disable : 6297) // potential arithmetic overflow

    #pragma warning(disable : 28020) // expression is not true for a call
    #pragma warning(disable : 28204) // override with different annotation

#elif defined(__GNUC__) // GNU

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-arguments"

#else // compiler

    #error "Unsupported C++ compiler."

#endif // compiler


// std

#include <utility>
#include <type_traits>

#include <memory>
#include <new>
#include <cstring>
#include <iterator>

#include <bitset>

#include <any>
#include <optional>
#include <variant>
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


// intrinsics

// https://docs.microsoft.com/en-us/cpp/intrinsics/compiler-intrinsics?view=msvc-160

// TODO: ARM

#if defined(__amd64__) || defined(_M_AMD64) // x64

    #include <immintrin.h>

#elif defined(__i386__) || defined(_M_IX86) // x86

    #include <immintrin.h>

#elif defined(__arm__) || defined(_M_ARM) // arm

#elif defined(__aarch64__) || defined(_M_ARM64) // arm64

#else // intrinsics

    #error "Unsupported computer architecture."

#endif // intrinsics


// pop warning pragmas

#if defined(__clang__) // clang

    #pragma clang diagnostic pop

#elif defined(_MSC_VER) // MSVC

    #pragma warning(pop)

#elif defined(__GNUC__) // GNU

    #pragma GCC diagnostic pop

#else // compiler

    #error "Unsupported C++ compiler."

#endif // compiler


#endif // ENV_PCH
