#ifndef FWA_CORE_STANDARD_HPP
#define FWA_CORE_STANDARD_HPP


// https://sourceforge.net/p/predef/wiki/Standards/

#ifdef __cplusplus // language
#if __cplusplus == 199711L // c++98
#define FWA_CPP 98
#define FWA_CPP98 1
// TODO: fix?!
// #error "Unsupported C++ standard."

#elif __cplusplus == 201103L // c++11
#define FWA_CPP 11
#define FWA_CPP11 1
#error "Unsupported C++ standard."

#elif __cplusplus == 201402L // c++14
#define FWA_CPP 14
#define FWA_CPP14 1
#error "Unsupported C++ standard."

#elif __cplusplus == 201703L // c++17
#define FWA_CPP 17
#define FWA_CPP17 1

#elif __cplusplus == 202003L // c++20
#define FWA_CPP 20
#define FWA_CPP20 1

#elif __cplusplus == 202303L // c++23
#define FWA_CPP 23
#define FWA_CPP23 1

#else // c++
#error "Unsupported C++ standard."
#endif // c++

#else // language
#error "Unsupported language."
#endif // language


#ifndef FWA_CPP98
#define FWA_CPP98 0
#endif // FWA_CPP98
#ifndef FWA_CPP11
#define FWA_CPP11 0
#endif // FWA_CPP11
#ifndef FWA_CPP14
#define FWA_CPP14 0
#endif // FWA_CPP14
#ifndef FWA_CPP17
#define FWA_CPP17 0
#endif // FWA_CPP17
#ifndef FWA_CPP20
#define FWA_CPP20 0
#endif // FWA_CPP >= 20
#ifndef FWA_CPP23
#define FWA_CPP23 0
#endif // FWA_CPP23


#endif // FWA_CORE_STANDARD_HPP
