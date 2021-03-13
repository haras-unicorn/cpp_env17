#ifndef FWA_CORE_SUPPRESS_HPP
#define FWA_CORE_SUPPRESS_HPP


#if FWA_CLANG || defined(__JETBRAINS_IDE__) // clang
#define FWA_CLANG_SUPPRESS_PUSH(_warning) \
            FWA_PRAGMA("clang diagnostic push") \
            FWA_PRAGMA(STRING(clang diagnostic ignored _warning))
#define FWA_CLANG_SUPPRESS_POP \
            FWA_PRAGMA("clang diagnostic pop")
#else // clang
#define FWA_CLANG_SUPPRESS_PUSH(_warning)
#define FWA_CLANG_SUPPRESS_POP
#endif // clang

#if FWA_MSVC && !defined(__JETBRAINS_IDE__) // MSVC
#define FWA_MSVC_SUPPRESS_PUSH(_warning) \
            FWA_PRAGMA(warning(push)) \
            FWA_PRAGMA(warning(disable : _warning))
#define FWA_MSVC_SUPPRESS_POP \
            FWA_PRAGMA(warning(pop))
#else // MSVC
#define FWA_MSVC_SUPPRESS_PUSH(_warning)
#define FWA_MSVC_SUPPRESS_POP
#endif // MSVC

#if FWA_GCC && !defined(__JETBRAINS_IDE__) // gcc
#define FWA_GCC_SUPPRESS_PUSH(_warning) \
            FWA_PRAGMA("GCC diagnostic push") \
            FWA_PRAGMA(STRING(GCC diagnostic ignored _warning))
#define FWA_GCC_SUPPRESS_POP \
            FWA_PRAGMA("GCC diagnostic pop")
#else // gcc
#define FWA_GCC_SUPPRESS_PUSH(_warning)
#define FWA_GCC_SUPPRESS_POP
#endif // gcc


// global warning suppression - please add only when necessary!!

// suppress JetBrains and other inspections added by extensions
FWA_CLANG_SUPPRESS_PUSH("-Wunknown-pragmas")

// TODO - investigate this and fix for JetBrains IDE's
// anonymous empty structs - I have no idea why I can't just put this in the ANON_STRUCT macro
FWA_CLANG_SUPPRESS_PUSH("-Wmissing-declarations")
FWA_MSVC_SUPPRESS_PUSH(4408)
FWA_MSVC_SUPPRESS_PUSH(4201)

// doctest sometimes declares variables with two const qualifiers
FWA_CLANG_SUPPRESS_PUSH("-Wduplicate-decl-specifier")
FWA_MSVC_SUPPRESS_PUSH(4114)


#endif // FWA_CORE_SUPPRESS_HPP
