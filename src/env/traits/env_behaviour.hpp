#ifndef ENV_BEHAVIOUR_HPP
#define ENV_BEHAVIOUR_HPP


// hash

// std hashable

// has_hash is weird like this because the STL tries to get a reference to void if
// we try to instantiate ENV_STD::hash with it, so we have to check that first.
// I don't know if this is UB, though. It seems a bit sketchy.

EXPR_CHECK_UNARY(is_std_hashable, declval<T&>(), ENV_STD::hash < T > { }(declvalr<T>()));

COND_CONCEPT(std_hashable, is_std_hashable_g<C>);

ENV_TEST_CASE("std hashable")
{
    REQUIRES(is_std_hashable_g<int>);
    REQUIRES_FALSE(is_std_hashable_g<void>);
}


// hashable

FUNCTION_DETECTOR(hash);

EXPR_CHECK_UNARY(is_hashable, has_hash_g<T> || is_std_hashable_g<T>);

COND_CONCEPT(hashable, is_hashable_g<C>);


// equatable

EXPR_CHECK_UNARY
(
        is_equatable,
        nonce(declvalr<T>() == declvalr<T>()),
        nonce(declvalr<T>() != declvalr<T>())
);

EXPR_CHECK_BINARY
(
        are_equatable,
        nonce(declvalr<TLhs>() == declvalr<TRhs>()),
        nonce(declvalr<TLhs>() != declvalr<TRhs>()),

        nonce(declvalr<TRhs>() == declvalr<TLhs>()),
        nonce(declvalr<TRhs>() != declvalr<TLhs>())
);

COND_CONCEPT(equatable, (is_equatable_g<C>));


// comparable

EXPR_CHECK_UNARY
(
        is_comparable,
        nonce(declvalr<T>() < declvalr<T>()),
        nonce(declvalr<T>() > declvalr<T>()),
        nonce(declvalr<T>() <= declvalr<T>()),
        nonce(declvalr<T>() >= declvalr<T>())
);

EXPR_CHECK_BINARY
(
        are_comparable,
        nonce(declvalr<TLhs>() < declvalr<TRhs>()),
        nonce(declvalr<TLhs>() > declvalr<TRhs>()),
        nonce(declvalr<TLhs>() <= declvalr<TRhs>()),
        nonce(declvalr<TLhs>() >= declvalr<TRhs>()),

        nonce(declvalr<TRhs>() < declvalr<TLhs>()),
        nonce(declvalr<TRhs>() > declvalr<TLhs>()),
        nonce(declvalr<TRhs>() <= declvalr<TLhs>()),
        nonce(declvalr<TRhs>() >= declvalr<TLhs>())
);

COND_CONCEPT(comparable, (is_comparable_g<C>));


ENV_TEST_CASE("behaviour traits")
{
    SUBCASE("equatable")
    {
        REQUIRES(is_equatable_g<int>);
        REQUIRES_FALSE(is_equatable_g<void>);

        REQUIRES(are_equatable_g<int, double>);
        REQUIRES_FALSE(are_equatable_g<void, int>);
    }

    SUBCASE("comparable")
    {
        REQUIRES(is_comparable_g<int>);
        REQUIRES_FALSE(is_comparable_g<void>);

        REQUIRES(are_comparable_g<int, double>);
        REQUIRES_FALSE(are_comparable_g<void, int>);
    }
}


// key

COND_CHECK_UNARY(is_key, is_hashable_g<T> && is_equatable_g<T>);

COND_CONCEPT(key, is_key_g<C>);


// advanceable

EXPR_CHECK_UNARY(is_advanceable, ++declvall<T>(), declvall<T>()++);

ENV_TEST_CASE("advanceable")
{
    REQUIRES(is_advanceable_g < int >);
    REQUIRES(is_advanceable_g < int * >);
    REQUIRES(is_advanceable_g<const int*>);
    REQUIRES_FALSE(is_advanceable_g<const int* const>);
    REQUIRES_FALSE(is_advanceable_g < int * const>);
}


#endif // ENV_BEHAVIOUR_HPP
