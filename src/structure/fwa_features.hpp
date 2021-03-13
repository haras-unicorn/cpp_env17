#ifndef ENV_FEATURES_HPP
#define ENV_FEATURES_HPP

// std requirements

#if (defined(FWA_TESTS) || defined(FWA_STATIC_TESTS)) && !defined(FWA_STD_REQUIREMENTS)
#define FWA_STD_REQUIREMENTS
#endif // (defined(FWA_TESTS) || defined(FWA_STATIC_TESTS)) && !defined(FWA_STD_REQUIREMENTS)

#ifdef FWA_STD_REQUIREMENTS
#define STD_REQUIRE(...) REQUIRES(__VA_ARGS__)
#define STD_REQUIRE_FALSE(...) REQUIRES_FALSE(__VA_ARGS__)
#else // FWA_STD_REQUIREMENTS
#define STD_REQUIRE(...)
#define STD_REQUIRE_FALSE(...)
#endif // FWA_STD_REQUIREMENTS

#endif // ENV_FEATURES_HPP
