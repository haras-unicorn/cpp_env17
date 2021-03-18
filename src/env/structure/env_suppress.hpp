#ifndef ENV_SUPPRESS_HPP
#define ENV_SUPPRESS_HPP


#if defined(ENV_SUPPRESS_PUSH) && !defined(ENV_SUPPRESS_POP)


#ifdef ENV_MESSAGES // messages
ENV_CLANG_SUPPRESS_PUSH("-W#pragma-messages")
ENV_MESSAGE("Globally suppressed \"-W#pragma-messages\" on Clang.")
ENV_MESSAGE("Globally suppressed \"-W#unknown-pragmas\" on Clang because of Clang raising warnings.")
#endif // messages

ENV_CLANG_SUPPRESS_PUSH("-Wunknown-pragmas") // clang-tidy
ENV_MESSAGE("Globally suppressed \"-Wunknown-pragmas\" on Clang because of clang-tidy raising warnings.")


// doctest
ENV_CLANG_SUPPRESS_PUSH("-Wduplicate-decl-specifier")
ENV_MESSAGE("Globally suppressed \"-Wduplicate-decl-specifier\" on Clang because of doctest raising warnings.")

ENV_GCC_SUPPRESS_PUSH("-Wmisleading-indentation")
ENV_MESSAGE("Globally suppressed \"-Wmisleading-indentation\" on Clang because of doctest raising warnings.")


#elif !defined(ENV_SUPPRESS_PUSH) && defined(ENV_SUPPRESS_POP)


#ifdef ENV_MESSAGES // messages
ENV_CLANG_SUPPRESS_POP
#endif // messages

ENV_CLANG_SUPPRESS_POP // clang-tidy


// doctest
ENV_CLANG_SUPPRESS_POP
ENV_GCC_SUPPRESS_POP


#endif // push, pop


#endif // ENV_SUPPRESS_HPP
