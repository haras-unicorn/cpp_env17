#ifndef ENV_PRAGMA_HPP
#define ENV_PRAGMA_HPP


#if ENV_CLANG // clang
#define ENV_PRAGMA_IMPL(_content) _Pragma(_content)
#define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)
#define ENV_PRAGMA(...) ENV_PRAGMA_STRING(STRING(__VA_ARGS__))
#endif // clang

#if ENV_GCC // GCC
#define ENV_PRAGMA_IMPL(_content) _Pragma(_content)
#define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)
#define ENV_PRAGMA(...) ENV_PRAGMA_STRING(STRING(__VA_ARGS__))
#endif // GCC

#if ENV_MSVC // GCC
#define ENV_PRAGMA_IMPL(_content) __pragma(_content)
#define ENV_PRAGMA_STRING(_content) ENV_PRAGMA_IMPL(_content)
#define ENV_PRAGMA(...) ENV_PRAGMA_STRING(__VA_ARGS__)
#endif // GCC


#endif //FAST_WEB_AUDIO_ENV_PRAGMA_HPP
