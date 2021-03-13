#ifndef ENV_SUPPRESS_HPP
#define ENV_SUPPRESS_HPP

#if ENV_CLANG || defined(__JETBRAINS_IDE__) // clang
#define ENV_CLANG_SUPPRESS_PUSH(_warning) \
    ENV_PRAGMA("clang diagnostic push")   \
    ENV_PRAGMA(STRING(clang diagnostic ignored _warning))
#define ENV_CLANG_SUPPRESS_POP \
    ENV_PRAGMA("clang diagnostic pop")
#else // clang
#define ENV_CLANG_SUPPRESS_PUSH(_warning)
#define ENV_CLANG_SUPPRESS_POP
#endif // clang

#if ENV_MSVC && !defined(__JETBRAINS_IDE__) // MSVC
#define ENV_MSVC_SUPPRESS_PUSH(_warning) \
    ENV_PRAGMA(warning(push))            \
    ENV_PRAGMA(warning(disable           \
                       : _warning))
#define ENV_MSVC_SUPPRESS_POP \
    ENV_PRAGMA(warning(pop))
#else // MSVC
#define ENV_MSVC_SUPPRESS_PUSH(_warning)
#define ENV_MSVC_SUPPRESS_POP
#endif // MSVC

#if ENV_GCC && !defined(__JETBRAINS_IDE__) // gcc
#define ENV_GCC_SUPPRESS_PUSH(_warning) \
    ENV_PRAGMA("GCC diagnostic push")   \
    ENV_PRAGMA(STRING(GCC diagnostic ignored _warning))
#define ENV_GCC_SUPPRESS_POP \
    ENV_PRAGMA("GCC diagnostic pop")
#else // gcc
#define ENV_GCC_SUPPRESS_PUSH(_warning)
#define ENV_GCC_SUPPRESS_POP
#endif // gcc

// global warning suppression - please add only when necessary!!

// suppress JetBrains and other inspections added by extensions
ENV_CLANG_SUPPRESS_PUSH("-Wunknown-pragmas")

// TODO - investigate this and fix for JetBrains IDE's
// anonymous empty structs - I have no idea why I can't just put this in the ANON_STRUCT macro
ENV_CLANG_SUPPRESS_PUSH("-Wmissing-declarations")
ENV_MSVC_SUPPRESS_PUSH(4408)
ENV_MSVC_SUPPRESS_PUSH(4201)

// doctest sometimes declares variables with two const qualifiers
ENV_CLANG_SUPPRESS_PUSH("-Wduplicate-decl-specifier")
ENV_MSVC_SUPPRESS_PUSH(4114)

#endif // ENV_SUPPRESS_HPP
