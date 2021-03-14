#ifndef ENV_ENUM_PROXY_HPP
#define ENV_ENUM_PROXY_HPP


// enum concept

ENV_NAMESPACE_TEST_BEGIN

typ(enum_underlying_t) = int;

enm enum_t : enum_underlying_t { A, B, C };

tmp<name T> cmp_fn underlying_test(enum_c <T> _enum) noex -> ENV_STD::underlying_type_t<T> { ret _enum; }

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("enum concept")
{
    REQUIRES(ENV_STD::is_enum_v < test::enum_t >);
    REQUIRES_FALSE(ENV_STD::is_enum_v < int >);

    REQUIRE_EQT(decltype(test::underlying_test(test::enum_t::A)), test::enum_underlying_t);
}


// proxy class

ENV_NAMESPACE_DETAIL_BEGIN

tmp<name TEnum>
cls enum_proxy_gt
{
public:
    typ(enum_t) = TEnum;
    typ(underlying_t) = ENV_STD::underlying_type_t<enum_t>;
    typ(proxy_t) = enum_proxy_gt<enum_t>;

private:
    DECL_THIS(enum_proxy_gt);
    DECL((const enum_t), value);

public:
    con cmp enum_proxy_gt(enum_t value) noex: _value{value} { }
    DEFAULT_CONST_LIFE(enum_proxy_gt, CMP);

    CMP_GETTER_FML((), (enum_t), get_enum, (_value));
    CMP_GETTER_FML((), (underlying_t), get_underlying, (static_cast<underlying_t>(_value)));

    ELABORATE_COND_COMPAT
    (
            (ENV_STD::is_same_v < T, enum_t > || ENV_STD::is_same_v < T, proxy_t >),
            (ENV_STD::is_same_v < T, enum_t > || ENV_STD::is_same_v < T, proxy_t >),
            (ENV_STD::is_same_v < T, proxy_t >) // TODO: enum
    );

    CMP_TMP_HASH { ret ENV::hash(get_underlying()); }
    CMP_TMP_EQUALITY { ret static_cast<underlying_t>(*this) == static_cast<underlying_t>(rhs); }
    CMP_TMP_COMPARISON { ret static_cast<underlying_t>(*this) < static_cast<underlying_t>(rhs); }

    con op enum_t() const noex { ret get_enum(); };
    con op underlying_t() const noex { ret get_underlying(); };
};

ENV_NAMESPACE_DETAIL_END

tmp<name T> typ(enum_proxy_gt) = detail::enum_proxy_gt<enum_c < T>>;


ENV_NAMESPACE_TEST_BEGIN

typ(enum_proxy_t) = enum_proxy_gt<enum_t>;

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("proxy class")
{
    const auto a = test::enum_t::A;
    const test::enum_proxy_t proxy_a{a};
    const test::enum_proxy_t proxy_b{test::enum_t::B};

    SUBCASE("hash")
    {
        REQUIRE(hash(test::enum_t::A) != hash(test::enum_t::B));
        REQUIRE(hash(test::enum_proxy_t{test::enum_t::A}) != hash(test::enum_proxy_t{test::enum_t::B}));
        REQUIRE(hasher(test::enum_t::A) != hasher(test::enum_t::B));
        REQUIRE(hasher(test::enum_proxy_t(test::enum_t::A)) != hasher(test::enum_proxy_t(test::enum_t::B)));
    }

    SUBCASE("equality")
    {
        REQUIRE_EQ(proxy_a, proxy_a);
        REQUIRE_EQ(proxy_a, test::enum_t::A);
        REQUIRE_NE(test::enum_proxy_t{test::enum_t::A}, proxy_b);
        REQUIRE_NE(proxy_a, test::enum_t::B);
    }

    SUBCASE("comparison")
    {
        // TODO: enum

        REQUIRE_LE(proxy_a, proxy_a);
        REQUIRE_LT(test::enum_proxy_t{test::enum_t::A}, proxy_b);
        REQUIRE_GT(proxy_b, proxy_a);
    }

    SUBCASE("conversion")
    {
        REQUIRE_EQ(test::enum_proxy_t{test::enum_t::A}.get_enum(), test::enum_t::A);
        REQUIRE_EQ(static_cast<test::enum_t>(proxy_a), test::enum_t::A);
        REQUIRE_EQ(proxy_a.get_underlying(), static_cast<test::enum_underlying_t>(test::enum_t::A));
    }
}


// proxy concept

ELABORATE_COND_CHECK
(
        is_enum_proxy,
        (name TEnumProxy), (TEnumProxy),
        (name TEnum), (detail::enum_proxy_gt<TEnum>),
        (ENV_STD::is_enum_v < TEnum > )
);

COND_CONCEPT(enum_proxy, (is_enum_proxy_g<C>));

ENV_TEST_CASE("proxy concept")
{
    REQUIRES(is_enum_proxy_g < test::enum_proxy_t >);
    REQUIRES_FALSE(is_enum_proxy_g < int >);

    REQUIRE_EQT(enum_proxy_c < test::enum_proxy_t >, test::enum_proxy_t);
}


// proxy function

tmp<name T> cmp_fn proxy(enum_c <T> end_point) noex { ret detail::enum_proxy_gt<T>{end_point}; }

ENV_TEST_CASE("proxy function")
{
    REQUIRE_EQ(proxy(test::enum_t::A), test::enum_t::A);
}


// underlying

tmp<name T> cmp_fn underlying_cast(enum_proxy_c<T> proxy) noex -> name T::underlying_t { ret proxy; }

COND_TMP_UNARY((ENV_STD::is_enum_v < T > || is_enum_proxy_g<T>))
typ(underlying_gt) = decltype(underlying_cast(declvalr<T>()));

ENV_TEST_CASE("underlying")
{
    REQUIRE_EQT(underlying_gt < test::enum_t >, test::enum_underlying_t);
    REQUIRE_EQT(underlying_gt < test::enum_proxy_t >, test::enum_underlying_t);
}


#endif // ENV_ENUM_PROXY_HPP
