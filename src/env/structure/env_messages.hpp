#ifndef ENV_MESSAGES_HPP
#define ENV_MESSAGES_HPP


#ifdef ENV_MESSAGES // messages

    #if ENV_MSVC // MSVC
        #define ENV_MESSAGE_IMPL(...) ENV_PRAGMA(message(__VA_ARGS__))
        #define ENV_MESSAGE_VAR(...)  ENV_MESSAGE_IMPL(__VA_ARGS__)
        #define ENV_MESSAGE(...)      ENV_MESSAGE_VAR(STRING(__VA_ARGS__))

        #define ENV_MSVC_MESSAGE(...)  ENV_MESSAGE(__VA_ARGS__)
        #define ENV_CLANG_MESSAGE(...) SEMI
        #define ENV_GCC_MESSAGE(...)   SEMI
    #endif // MSVC

    #if ENV_CLANG // clang
        #define ENV_MESSAGE_IMPL(_content) ENV_PRAGMA(message _content)
        #define ENV_MESSAGE_VAR(_content)  ENV_MESSAGE_IMPL(_content)
        #define ENV_MESSAGE(...)           ENV_MESSAGE_VAR(STRING(__VA_ARGS__))

        #define ENV_MSVC_MESSAGE(...)  SEMI
        #define ENV_CLANG_MESSAGE(...) ENV_MESSAGE(__VA_ARGS__)
        #define ENV_GCC_MESSAGE(...)   SEMI
    #endif // clang

    #if ENV_GCC // gcc
        #define ENV_MESSAGE_IMPL(_content) ENV_PRAGMA(message _content)
        #define ENV_MESSAGE_VAR(_content)  ENV_MESSAGE_IMPL(_content)
        #define ENV_MESSAGE(...)           ENV_MESSAGE_VAR(STRING(__VA_ARGS__))

        #define ENV_MSVC_MESSAGE(...)  SEMI
        #define ENV_CLANG_MESSAGE(...) SEMI
        #define ENV_GCC_MESSAGE(...)   ENV_MESSAGE(__VA_ARGS__)
    #endif // gcc

#else // messages
    #define ENV_MESSAGE(...)       SEMI
    #define ENV_MSVC_MESSAGE(...)  SEMI
    #define ENV_CLANG_MESSAGE(...) SEMI
    #define ENV_GCC_MESSAGE(...)   SEMI

#endif // messages


#endif // ENV_MESSAGES_HPP
