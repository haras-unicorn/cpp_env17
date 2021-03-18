#ifndef ENV_WARNINGS_HPP
#define ENV_WARNINGS_HPP


#if ENV_CLANG || defined(__JETBRAINS_IDE__) // clang
#define ENV_CLANG_SUPPRESS_PUSH(_warning) \
    ENV_PRAGMA(clang diagnostic push) \
    ENV_PRAGMA(clang diagnostic ignored _warning)
#define ENV_CLANG_SUPPRESS_POP \
    ENV_PRAGMA(clang diagnostic pop)
#else // clang
#define ENV_CLANG_SUPPRESS_PUSH(_warning)
#define ENV_CLANG_SUPPRESS_POP
#endif // clang

#if ENV_MSVC && !defined(__JETBRAINS_IDE__) // MSVC
#define ENV_MSVC_SUPPRESS_PUSH(_warning) \
    ENV_PRAGMA(warning(push))            \
    ENV_PRAGMA(warning(disable : _warning))
#define ENV_MSVC_SUPPRESS_POP \
    ENV_PRAGMA(warning(pop))
#else // MSVC
#define ENV_MSVC_SUPPRESS_PUSH(_warning)
#define ENV_MSVC_SUPPRESS_POP
#endif // MSVC

#if ENV_GCC && !defined(__JETBRAINS_IDE__) // gcc
#define ENV_GCC_SUPPRESS_PUSH(_warning) \
    ENV_PRAGMA(GCC diagnostic push)   \
    ENV_PRAGMA(GCC diagnostic ignored _warning)
#define ENV_GCC_SUPPRESS_POP \
    ENV_PRAGMA(GCC diagnostic pop)
#else // gcc
#define ENV_GCC_SUPPRESS_PUSH(_warning)
#define ENV_GCC_SUPPRESS_POP
#endif // gcc


#endif // ENV_WARNINGS_HPP
