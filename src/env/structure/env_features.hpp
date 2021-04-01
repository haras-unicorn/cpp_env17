#ifndef ENV_FEATURES_HPP
#define ENV_FEATURES_HPP


// std requirements

#if (defined(ENV_TESTS) || defined(ENV_STATIC_TESTS)) && \
        !defined(ENV_STD_REQUIREMENTS)
    #define ENV_STD_REQUIREMENTS
#endif // (defined(ENV_TESTS) || defined(ENV_STATIC_TESTS)) && !defined(ENV_STD_REQUIREMENTS)

#ifdef ENV_STD_REQUIREMENTS
    #define STD_REQUIRE(...)       REQUIRES(__VA_ARGS__)
    #define STD_REQUIRE_FALSE(...) REQUIRES_FALSE(__VA_ARGS__)

#else // ENV_STD_REQUIREMENTS
    #define STD_REQUIRE(...)
    #define STD_REQUIRE_FALSE(...)

#endif // ENV_STD_REQUIREMENTS


// base allocator

#ifndef ENV_ALLOCATOR
    #define ENV_ALLOCATOR ENV_STD::allocator
#endif // ENV_ALLOCATOR

#ifndef ENV_DELETER
    #define ENV_DELETER ENV_STD::default_delete
#endif // ENV_ALLOCATOR


#endif // ENV_FEATURES_HPP
