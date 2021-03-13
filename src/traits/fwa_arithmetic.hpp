#ifndef FWA_CORE_MATH_TRAITS_HPP
#define FWA_CORE_MATH_TRAITS_HPP


FWA_NAMESPACE_DETAIL_BEGIN

COND_CHECK_UNARY(is_arithmetic_enum, FWA_STD::is_convertible_v < FWA_STD::underlying_type_t < T >, int >);

FWA_NAMESPACE_DETAIL_END

COND_CHECK_UNARY
(
        is_arithmetic,
        (
                FWA_STD::is_arithmetic_v < T > ||
                FWA_STD::is_floating_point_v < T > ||
                detail::is_arithmetic_enum_g<T>
        )
);

COND_CONCEPT(arithmetic, (is_arithmetic_g<C>));


COND_CHECK_UNARY(is_floating, (FWA_STD::is_floating_point_v < T > ));

COND_CONCEPT(floating, (is_floating_g<C>));


COND_CHECK_UNARY(is_whole, (!is_floating_g<T>));

COND_CONCEPT(whole, (is_whole_g<C>));


FWA_NAMESPACE_DETAIL_BEGIN

COND_CHECK_UNARY(is_signed_enum, FWA_STD::is_signed_v < FWA_STD::underlying_type_t < T > >);

FWA_NAMESPACE_DETAIL_END

COND_CHECK_UNARY(is_signed, (FWA_STD::is_signed_v < T > || detail::is_signed_enum_g<T>));

COND_CONCEPT(signed, (is_signed_g<C>));


FWA_NAMESPACE_DETAIL_BEGIN

COND_CHECK_UNARY(is_unsigned_enum, FWA_STD::is_unsigned_v < FWA_STD::underlying_type_t < T > >);

FWA_NAMESPACE_DETAIL_END

COND_CHECK_UNARY(is_unsigned, (FWA_STD::is_unsigned_v < T > || detail::is_unsigned_enum_g<T>));

COND_CONCEPT(unsigned, (is_unsigned_g<C>));


FWA_NAMESPACE_TEST_BEGIN

enum unique_size_t : FWA_STD::size_t { };

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("math traits")
{
    SUBCASE("arithmetic")
    {
        REQUIRES(is_arithmetic_g<int>);
        REQUIRES(is_arithmetic_g<float>);
        REQUIRES(is_arithmetic_g<test::unique_size_t>);
        REQUIRES_FALSE(is_arithmetic_g<void>);
    }

    SUBCASE("floating")
    {
        REQUIRES(is_floating_g<float>);
        REQUIRES_FALSE(is_floating_g<int>);
        REQUIRES_FALSE(is_floating_g<test::unique_size_t>);
    }

    SUBCASE("whole")
    {
        REQUIRES(is_whole_g<int>);
        REQUIRES(is_whole_g<test::unique_size_t>);
        REQUIRES_FALSE(is_whole_g<float>);
    }

    SUBCASE("signed")
    {
        REQUIRES(is_signed_g<int>);
        REQUIRES_FALSE(is_signed_g<unsigned>);
        REQUIRES_FALSE(is_signed_g<test::unique_size_t>);
    }

    SUBCASE("unsigned")
    {
        REQUIRES(is_unsigned_g<unsigned>);
        REQUIRES(is_unsigned_g<test::unique_size_t>);
        REQUIRES_FALSE(is_unsigned_g<int>);
    }
}


// whole

template<bool Signed, FWA_STD::size_t Size>
strct whole_nns;

let_cmp without_sign{false};

let_cmp with_sign{true};

// unsigned

template<>
strct whole_nns<false, 1> : type_gt<uint8_t> { };

template<>
strct whole_nns<false, 2> : type_gt<uint16_t> { };

template<>
strct whole_nns<false, 4> : type_gt<uint32_t> { };

template<>
strct whole_nns<false, 8> : type_gt<uint64_t> { };

// signed

template<>
strct whole_nns<true, 1> : type_gt<int8_t> { };

template<>
strct whole_nns<true, 2> : type_gt<int16_t> { };

template<>
strct whole_nns<true, 4> : type_gt<int32_t> { };

template<>
strct whole_nns<true, 8> : type_gt<int64_t> { };

template<bool Signed, FWA_STD::size_t Size>
typ(whole_nnt) = name whole_nns<Signed, Size>::type;

FWA_CORE_TEST_CASE("number traits")
{
    REQUIRE_EQT(whole_nnt<without_sign, 1>, uint8_t);
    REQUIRE_EQT(whole_nnt<without_sign, 8>, uint64_t);
    REQUIRE_EQT(whole_nnt<with_sign, 1>, int8_t);
    REQUIRE_EQT(whole_nnt<with_sign, 8>, int64_t);
}


#endif // FWA_CORE_MATH_TRAITS_HPP
