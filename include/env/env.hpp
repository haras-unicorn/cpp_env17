#ifndef ENV_INCLUDED
#define ENV_INCLUDED


// basic macros - uncomment if needed

// #define ENV_EVAL(...) __VA_ARGS__

// #define ENV_CAT_IMPL(_lhs, _rhs) _lhs##_rhs
// #define ENV_CAT(_lhs, _rhs)      ENV_CAT_IMPL(_lhs, _rhs)

// #define ENV_INTER_IMPL(_lhs, _middle, _rhs) _lhs##_middle##_rhs
// #define ENV_INTER(_lhs, _middle, _rhs)      ENV_INTER_IMPL(_lhs, _middle, _rhs)

#define ENV_STRING_IMPL(...) #__VA_ARGS__
#define ENV_STRING_VAR(...)  ENV_STRING_IMPL(__VA_ARGS__)
#define ENV_STRING(...)      ENV_STRING_VAR(__VA_ARGS__)

// #define ENV_SPREAD_IMPL(_tuple) ENV_EVAL(ENV_EVAL _tuple)
// #define ENV_SPREAD(_tuple)      ENV_SPREAD_IMPL(_tuple)

// #define ENV_UID(_prefix) ENV_INTER(_z, _prefix, __COUNTER__)

#define ENV_SEMI static_assert(true, "Require semicolon.")


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
ENV_GNU_SUPPRESS_PUSH("-W#pragma-messages");

ENV_MESSAGE(Env Compiler - ENV_COMPILER_NAME, ENV_COMPILER_VER);
ENV_MESSAGE(Env Arch - ENV_ARCH_NAME);
ENV_MESSAGE(Env OS - ENV_OS_NAME);
ENV_MESSAGE(Env Standard - C++ ENV_CPP);

ENV_GNU_SUPPRESS_POP;
ENV_CLANG_SUPPRESS_POP;

// Ignore GoogleTest unused static variable warning
#define ENV_TEST(_suite, _test) TEST(_suite, _test) // NOLINT(cert-err58-cpp)


// env

namespace env
{
namespace alloc
{
template<typename T>
using allocator [[maybe_unused]] = mi_stl_allocator<T>;

struct heap
{
    template<typename... Tags>
    static void* allocate(std::size_t size, Tags...)
    {
        return mi_new(size);
    }

    static void deallocate(std::size_t, void* data)
    {
        mi_free(data);
    }
};

using memory_policy =
        ::immer::memory_policy<
                ::immer::heap_policy<heap>,
                ::immer::default_refcount_policy,
                ::immer::default_lock_policy>;
} // namespace alloc

namespace std
{
using namespace ::std;

namespace alloc
{
template<typename T, typename TAlloc = ::env::alloc::allocator<T>>
using deque [[maybe_unused]] = deque<T, TAlloc>;

template<typename T, typename TSeq = deque<T>>
using queue [[maybe_unused]] = queue<T, TSeq>;

template<typename T, typename TSeq = deque<T>>
using stack [[maybe_unused]] = stack<T, TSeq>;


template<typename T, typename TAlloc = ::env::alloc::allocator<T>>
using list [[maybe_unused]] = list<T, TAlloc>;

template<typename T, typename TAlloc = ::env::alloc::allocator<T>>
using forward_list [[maybe_unused]] = forward_list<T, TAlloc>;


template<typename T, typename TAlloc = ::env::alloc::allocator<T>>
using vector [[maybe_unused]] = vector<T, TAlloc>;


template<typename T,
         typename TCompare = std::less<T>,
         typename TAlloc = ::env::alloc::allocator<T>>
using set [[maybe_unused]] = set<T, TCompare, TAlloc>;

template<typename T,
         typename THash = std::hash<T>,
         typename TEquals = std::equal_to<T>,
         typename TAlloc = ::env::alloc::allocator<T>>
using unordered_set [[maybe_unused]] = unordered_set<T, THash, TEquals, TAlloc>;

template<typename T,
         typename TCompare = std::less<T>,
         typename TAlloc = ::env::alloc::allocator<T>>
using map [[maybe_unused]] = map<T, TCompare, TAlloc>;

template<typename T,
         typename THash = std::hash<T>,
         typename TEquals = std::equal_to<T>,
         typename TAlloc = ::env::alloc::allocator<T>>
using unordered_map [[maybe_unused]] = unordered_map<T, THash, TEquals, TAlloc>;
} // namespace alloc
} // namespace std

namespace meta = ::boost::hana;
namespace test = ::testing;
namespace bench = ::benchmark;
namespace json = ::nlohmann;

namespace immut
{
using namespace ::immer;

namespace alloc
{
template<typename T, typename TMem = ::env::alloc::memory_policy>
using box [[maybe_unused]] = box<T, TMem>;


template<typename T, typename TMem = ::env::alloc::memory_policy>
using array [[maybe_unused]] = array<T, TMem>;

template<typename T,
         typename TMem = ::env::alloc::memory_policy,
         detail::rbts::bits_t Bits = default_bits,
         detail::rbts::bits_t BitsLeaf =
                 detail::rbts::derive_bits_leaf<T, TMem, Bits>>
using vector [[maybe_unused]] = vector<T, TMem, Bits, BitsLeaf>;

template<typename T,
         typename TMem = ::env::alloc::memory_policy,
         detail::rbts::bits_t Bits = default_bits,
         detail::rbts::bits_t BitsLeaf =
                 detail::rbts::derive_bits_leaf<T, TMem, Bits>>
using flex_vector [[maybe_unused]] = flex_vector<T, TMem, Bits, BitsLeaf>;


template<typename T,
         typename THash = std::hash<T>,
         typename TEquals = std::equal_to<T>,
         typename TMem = ::env::alloc::memory_policy,
         ::immer::detail::hamts::bits_t Bits = default_bits>
using set [[maybe_unused]] = set<T, THash, TEquals, TMem, Bits>;

template<typename TKey,
         typename TValue,
         typename THash = std::hash<TKey>,
         typename TEquals = std::equal_to<TKey>,
         typename TMem = ::env::alloc::memory_policy,
         ::immer::detail::hamts::bits_t Bits = default_bits>
using map [[maybe_unused]] = map<TKey, TValue, THash, TEquals, TMem, Bits>;
} // namespace alloc
} // namespace immut

namespace trans
{
using namespace ::zug;
}

namespace literals
{
using namespace ::std::literals;
using namespace ::std::string_literals;
using namespace ::std::string_view_literals;
using namespace ::std::chrono_literals;

using namespace ::boost::hana::literals;
} // namespace literals

namespace placeholder
{
using ::boost::hana::_;
} // namespace placeholder

namespace syntax
{
using namespace literals;
using namespace placeholder;
} // namespace syntax


namespace
{
::std::once_flag initialize_flag{};
::std::once_flag finalize_flag{};

struct scoped_initializer
{
    inline scoped_initializer()
    {
        ::std::call_once(
                initialize_flag,
                [] {
                    mi_option_enable(mi_option_reserve_huge_os_pages);
                    mi_option_enable(mi_option_eager_commit);
                    mi_option_enable(mi_option_eager_region_commit);
                });
    }

    inline ~scoped_initializer()
    {
        ::std::call_once(finalize_flag, [] {});
    }
} initializer{}; // NOLINT(cert-err58-cpp)
} // namespace
} // namespace env


// hack for templates using accessors

namespace boost::hana
{
template<typename TTag>
struct accessors_impl<
        TTag, when<struct_detail::is_valid<typename TTag::sample>::value>>
{
private:
    static constexpr auto sample_inner =
            TTag::sample::hana_accessors_impl::apply();

    template<typename T>
    static constexpr decltype(T::hana_accessors_impl::apply()) inner =
            T::hana_accessors_impl::apply();

    template<std::size_t... I, typename... T>
    static constexpr decltype(auto) _apply(
            detail::basic_tuple_impl<std::index_sequence<I...>, T...>
                    _sample_inner)
    {
        constexpr auto getter =
                [](auto i, auto&& x) {
                    using type = decltype(x);
                    using unqualified =
                            std::remove_reference_t<
                                    std::remove_cv_t<type>>;

                    return second(inner<unqualified>[i])(std::forward<type>(x));
                };

        return make_tuple(
                make_pair(first(detail::ebo_get<detail::bti<I>>(_sample_inner)),
                          partial(getter, size_c<I>))...);
    }

public:
    static constexpr decltype(auto) apply()
    {
        return _apply(sample_inner.storage_);
    }
};
} // namespace boost::hana


#endif // ENV_INCLUDED
