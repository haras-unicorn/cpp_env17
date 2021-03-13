#ifndef ENV_MESSAGES_HPP
#define ENV_MESSAGES_HPP

#ifdef ENV_MESSAGES // messages
#if ENV_MSVC        // MSVC
#define ENV_MESSAGE_IMPL(...) ENV_PRAGMA(message(__VA_ARGS__))
#define ENV_MESSAGE_VAR(...) ENV_MESSAGE_IMPL(__VA_ARGS__)
#define ENV_MESSAGE(...) ENV_MESSAGE_VAR(STRING(__VA_ARGS__))
#endif        // MSVC
#if ENV_CLANG // clang
#define ENV_MESSAGE_IMPL(_content) ENV_PRAGMA(message _content)
#define ENV_MESSAGE_VAR(_content) ENV_MESSAGE_IMPL(_content)
#define ENV_MESSAGE(...) ENV_MESSAGE_VAR(STRING(__VA_ARGS__))
#endif      // clang
#if ENV_GCC // gcc
#define ENV_MESSAGE_IMPL(_content) ENV_PRAGMA(message _content)
#define ENV_MESSAGE_VAR(_content) ENV_MESSAGE_IMPL(_content)
#define ENV_MESSAGE(...) ENV_MESSAGE_VAR(STRING(__VA_ARGS__))
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

// GCC makes a ton of notes about these...

ENV_MESSAGE(Env Compiler - ENV_COMPILER_NAME, ENV_COMPILER_VER)
ENV_MESSAGE(Env Arch - ENV_ARCH_NAME)
ENV_MESSAGE(Env OS - ENV_OS_NAME)
ENV_MESSAGE(Env Standard - C++ ENV_CPP)

#endif // ENV_MESSAGES_HPP
