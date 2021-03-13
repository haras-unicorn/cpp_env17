#ifndef ENV_ATTRIBUTES_HPP
#define ENV_ATTRIBUTES_HPP

// attributes

#if ENV_CPP >= 17
#define TYPE_ATTRIBUTES
#define OBJECT_ATTRIBUTES
#define RETURN_ATTRIBUTES
#define NO_RETURN_ATTRIBUTES
#else // ENV_CPP >= 17
#define TYPE_ATTRIBUTES
#define OBJECT_ATTRIBUTES
#define RETURN_ATTRIBUTES
#define NO_RETURN_ATTRIBUTES
#endif // ENV_CPP >= 17

ENV_TEST_CASE("attributes")
{
    [[OBJECT_ATTRIBUTES]] constexpr int a{ };
    static_cast<void>(a);

    struct [[TYPE_ATTRIBUTES]] test_t
    {
        [[RETURN_ATTRIBUTES]] int f() { return 0; }

        NO_RETURN_ATTRIBUTES void g() { }
    };
}

// tag

#if ENV_CPP >= 17
#define TAG(_name)                            \
    struct [[TYPE_ATTRIBUTES]] CAT(_name, _t) \
    {                                         \
    } inline constexpr PACK(_name) {}
#else // ENV_CPP >= 17
#define TAG(_name)                            \
    struct [[TYPE_ATTRIBUTES]] CAT(_name, _t) \
    {                                         \
    } constexpr PACK(_name) {}
#endif // ENV_CPP >= 17

ENV_NAMESPACE_TEST_BEGIN

TAG(tag_test);

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("tag")
{
    REQUIRE_EQT(decltype(test::tag_test), const test::tag_test_t);
}

// unique

#define UNIQUE(_name, ...)                                             \
    using CAT(_name, _underlying_t) [[TYPE_ATTRIBUTES]] = __VA_ARGS__; \
    enum [[TYPE_ATTRIBUTES]] CAT(_name, _t) : CAT(_name, _underlying_t) {}

ENV_NAMESPACE_TEST_BEGIN

UNIQUE(unique_test, int);

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("unique")
{
    [[OBJECT_ATTRIBUTES]] static constexpr test::unique_test_t _unique{20};

    REQUIRES(_unique == test::unique_test_t{20});
    REQUIRE_EQT(ENV_STD::underlying_type_t < decltype(_unique) >, int);
}

// unique tag

#if ENV_CPP >= 17
#define UTAG(_name, ...)                                                         \
    using CAT(_name, _underlying_t) [[TYPE_ATTRIBUTES]] = __VA_ARGS__;           \
    enum class [[TYPE_ATTRIBUTES]] CAT(_name, _t) : CAT(_name, _underlying_t) {} \
    inline constexpr PACK(_name) {}
#else // ENV_CPP >= 17
#define UTAG(_name, ...)                                               \
    using CAT(_name, _underlying_t) [[TYPE_ATTRIBUTES]] = __VA_ARGS__; \
    enum class CAT(_name, _t) : CAT(_name, _underlying_t) {}           \
    constexpr PACK(_name) {}
#endif // ENV_CPP >= 17

ENV_NAMESPACE_TEST_BEGIN

UTAG(unique_tag_test, int);

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("unique")
{
    REQUIRES(test::unique_tag_test == test::unique_tag_test_t{ });
    REQUIRE_EQT(ENV_STD::underlying_type_t < decltype(test::unique_tag_test) >, int);
}

#endif // ENV_ATTRIBUTES_HPP
