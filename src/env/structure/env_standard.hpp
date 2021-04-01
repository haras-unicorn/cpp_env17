#ifndef ENV_STANDARD_HPP
#define ENV_STANDARD_HPP


// https://sourceforge.net/p/predef/wiki/Standards/

#ifdef __cplusplus             // language
    #if __cplusplus == 199711L // c++98
        #define ENV_CPP   98
        #define ENV_CPP98 1
        // TODO: fix?!
        // #error "Unsupported C++ standard."

    #elif __cplusplus == 201103L // c++11
        #define ENV_CPP   11
        #define ENV_CPP11 1
        #error "Unsupported C++ standard."

    #elif __cplusplus == 201402L // c++14
        #define ENV_CPP   14
        #define ENV_CPP14 1
        #error "Unsupported C++ standard."

    #elif __cplusplus == 201703L // c++17
        #define ENV_CPP   17
        #define ENV_CPP17 1

    #elif __cplusplus == 202003L // c++20
        #define ENV_CPP   20
        #define ENV_CPP20 1

    #elif __cplusplus == 202303L // c++23
        #define ENV_CPP   23
        #define ENV_CPP23 1

    #else // c++
        #error "Unsupported C++ standard."
    #endif // c++

#else // language
    #error "Unsupported language."
#endif // language


#ifndef ENV_CPP98
    #define ENV_CPP98 0
#endif // ENV_CPP98
#ifndef ENV_CPP11
    #define ENV_CPP11 0
#endif // ENV_CPP11
#ifndef ENV_CPP14
    #define ENV_CPP14 0
#endif // ENV_CPP14
#ifndef ENV_CPP17
    #define ENV_CPP17 0
#endif // ENV_CPP17
#ifndef ENV_CPP20
    #define ENV_CPP20 0
#endif // ENV_CPP >= 20
#ifndef ENV_CPP23
    #define ENV_CPP23 0
#endif // ENV_CPP23


#endif // ENV_STANDARD_HPP
