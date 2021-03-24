#ifndef ENV_SUPPRESS_HPP
#define ENV_SUPPRESS_HPP


#if defined(ENV_SUPPRESS_PUSH) && !defined(ENV_SUPPRESS_POP)


#ifdef ENV_MESSAGES // messages
ENV_CLANG_SUPPRESS_PUSH("-W#pragma-messages")
ENV_CLANG_MESSAGE(Globally suppressed "-W#pragma-messages".)
#endif // messages

ENV_CLANG_SUPPRESS_PUSH("-Wunknown-pragmas") // clang-tidy
ENV_CLANG_MESSAGE(Globally suppressed "-Wunknown-pragmas"(Clang).)


// doctest
ENV_CLANG_SUPPRESS_PUSH("-Wduplicate-decl-specifier")
ENV_CLANG_MESSAGE(Globally suppressed "-Wduplicate-decl-specifier"(doctest).)

ENV_GCC_SUPPRESS_PUSH("-Wmisleading-indentation")
ENV_GCC_MESSAGE(Globally suppressed "-Wmisleading-indentation"(doctest).)


#elif !defined(ENV_SUPPRESS_PUSH) && defined(ENV_SUPPRESS_POP)


#ifdef ENV_MESSAGES // messages
ENV_CLANG_SUPPRESS_POP
#endif // messages

ENV_CLANG_SUPPRESS_POP // unknown pragmas


// doctest
ENV_CLANG_SUPPRESS_POP
ENV_GCC_SUPPRESS_POP


#endif // push, pop


#endif // ENV_SUPPRESS_HPP
