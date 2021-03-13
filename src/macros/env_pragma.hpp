#ifndef ENV_PRAGMA_HPP
#define ENV_PRAGMA_HPP

#if ENV_CLANG || defined(__JETBRAINS_IDE__) // clang
#define ENV_PRAGMA(...) _Pragma(__VA_ARGS__)
#endif // clang

#if ENV_MSVC && !defined(__JETBRAINS_IDE__) // MSVC
#define ENV_PRAGMA(...) __pragma(__VA_ARGS__)
#endif // MSVC

#if ENV_GCC && !defined(__JETBRAINS_IDE__) // GCC
#define ENV_PRAGMA(...) _Pragma(__VA_ARGS__)
#endif // GCC

#endif //FAST_WEB_AUDIO_ENV_PRAGMA_HPP
