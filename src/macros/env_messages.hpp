#ifndef ENV_MESSAGES_HPP
#define ENV_MESSAGES_HPP

#ifdef ENV_MESSAGES // messages
#if ENV_MSVC        // MSVC
#define ENV_MESSAGE_IMPL(_message) ENV_PRAGMA(message(_message))
#define ENV_MESSAGE_VAR(_message) ENV_MESSAGE_IMPL(PACK(_message))
#define ENV_MESSAGE(...) ENV_MESSAGE_VAR(PACK(__VA_ARGS__))
#endif        // MSVC
#if ENV_CLANG // clang
#define ENV_MESSAGE_IMPL(_message) ENV_PRAGMA(message _message)
#define ENV_MESSAGE_VAR(_message) ENV_MESSAGE_IMPL(PACK(_message))
#define ENV_MESSAGE(...) ENV_MESSAGE_VAR(PACK(__VA_ARGS__))
#endif      // clang
#if ENV_GCC // gcc
#define ENV_MESSAGE_IMPL(_message) ENV_PRAGMA(message _message)
#define ENV_MESSAGE_VAR(_message) ENV_MESSAGE_IMPL(PACK(_message))
#define ENV_MESSAGE(...) ENV_MESSAGE_VAR(PACK(__VA_ARGS__))
#endif // gcc

#else        // messages
#if ENV_MSVC // MSVC
#define ENV_MESSAGE(...)
#endif        // MSVC
#if ENV_CLANG // clang
#define ENV_MESSAGE(...)
#endif      // clang
#if ENV_GCC // gcc
#define ENV_MESSAGE(...)
#endif // gcc
#endif // messages

// ":" after label formats it badly

ENV_MESSAGE(STRING(ENV Compiler - ENV_COMPILER_NAME, ENV_COMPILER_VER))
ENV_MESSAGE(STRING(ENV Arch - ENV_ARCH_NAME))
ENV_MESSAGE(STRING(ENV OS - ENV_OS_NAME))
ENV_MESSAGE(STRING(ENV Standard - C++ ENV_CPP))

#endif // ENV_MESSAGES_HPP
