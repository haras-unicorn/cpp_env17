#ifndef ENV_PRAGMA_HPP
#define ENV_PRAGMA_HPP

#if ENV_CLANG || defined(__JETBRAINS_IDE__) // clang
#define ENV_PRAGMA_IMPL(_content) _Pragma(_content)
#define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)
#define ENV_PRAGMA(...) ENV_PRAGMA_STRING(STRING(__VA_ARGS__))
#endif // clang

#if ENV_GCC && !defined(__JETBRAINS_IDE__) // GCC
#define ENV_PRAGMA_IMPL(_content) _Pragma(_content)
#define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)
#define ENV_PRAGMA(...) ENV_PRAGMA_STRING(STRING(__VA_ARGS__))
#endif // GCC

#if ENV_MSVC && !defined(__JETBRAINS_IDE__) // GCC
#define ENV_PRAGMA_IMPL(_content) __pragma(_content)
#define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)
#define ENV_PRAGMA(...) ENV_PRAGMA_STRING(__VA_ARGS__)
#endif // GCC

#endif //FAST_WEB_AUDIO_ENV_PRAGMA_HPP
