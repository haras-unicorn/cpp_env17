#ifndef ENV_MESSAGES_HPP
#define ENV_MESSAGES_HPP

#ifdef FWA_MESSAGES // messages
#if FWA_MSVC        // MSVC
#define FWA_MESSAGE_IMPL(_message) FWA_PRAGMA(message(_message))
#define FWA_MESSAGE_VAR(_message) FWA_MESSAGE_IMPL(PACK(_message))
#define FWA_MESSAGE(...) FWA_MESSAGE_VAR(PACK(__VA_ARGS__))
#endif        // MSVC
#if FWA_CLANG // clang
#define FWA_MESSAGE_IMPL(_message) FWA_PRAGMA(message _message)
#define FWA_MESSAGE_VAR(_message) FWA_MESSAGE_IMPL(PACK(_message))
#define FWA_MESSAGE(...) FWA_MESSAGE_VAR(PACK(__VA_ARGS__))
#endif      // clang
#if FWA_GCC // gcc
#define FWA_MESSAGE_IMPL(_message) FWA_PRAGMA(message _message)
#define FWA_MESSAGE_VAR(_message) FWA_MESSAGE_IMPL(PACK(_message))
#define FWA_MESSAGE(...) FWA_MESSAGE_VAR(PACK(__VA_ARGS__))
#endif // gcc

#else        // messages
#if FWA_MSVC // MSVC
#define FWA_MESSAGE(...)
#endif        // MSVC
#if FWA_CLANG // clang
#define FWA_MESSAGE(...)
#endif      // clang
#if FWA_GCC // gcc
#define FWA_MESSAGE(...)
#endif // gcc
#endif // messages

// ":" after label formats it badly

FWA_MESSAGE(STRING(FWA Compiler - FWA_COMPILER_NAME, FWA_COMPILER_VER))
FWA_MESSAGE(STRING(FWA Arch - FWA_ARCH_NAME))
FWA_MESSAGE(STRING(FWA OS - FWA_OS_NAME))
FWA_MESSAGE(STRING(FWA Standard - C++ FWA_CPP))

#endif // ENV_MESSAGES_HPP
