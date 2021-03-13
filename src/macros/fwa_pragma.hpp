#ifndef FWA_CORE_PRAGMA_HPP
#define FWA_CORE_PRAGMA_HPP


#if FWA_CLANG || defined(__JETBRAINS_IDE__) // clang
#define FWA_PRAGMA(...) _Pragma(__VA_ARGS__)
#endif // clang

#if FWA_MSVC && !defined(__JETBRAINS_IDE__) // MSVC
#define FWA_PRAGMA(...) __pragma(__VA_ARGS__)
#endif // MSVC

#if FWA_GCC && !defined(__JETBRAINS_IDE__) // GCC
#define FWA_PRAGMA(...) _Pragma(__VA_ARGS__)
#endif // GCC


#endif //FAST_WEB_AUDIO_FWA_PRAGMA_HPP
