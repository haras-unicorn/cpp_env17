#ifndef ENV_BEHAVIOUR_HPP
#define ENV_BEHAVIOUR_HPP


// hash

// std_hashable

// std_hashable is weird like this because the STL tries to get a reference to void if
// we try to instantiate ENV_STD::hash with it, so we have to check that first.
// I don't know if this is UB, though. It seems a bit sketchy.

EXPR_CHECK_UNARY(
        is_std_hashable,
        TYPE_EXPR(T&),
        COND_EXPR(same_vt<
                  decl(ENV_STD::hash<T>{}(declval<T>())),
                  ENV_STD::size_t>));

COND_CONCEPT(std_hashable, is_std_hashable_gs<C>);

// hashable

COND_CHECK_UNARY(
        has_hash,
        same_vt<decl(declval<T>().hash()), ENV_STD::size_t>);

COND_CHECK_UNARY(is_hashable, dis_vt<has_hash_gs<T>, is_std_hashable_gs<T>>);

COND_CONCEPT(hashable, is_hashable_gs<C>);


ENV_TEST_CASE("hashable")
{
    REQUIRES(is_std_hashable_g<int>);
    REQUIRES_FALSE(is_std_hashable_g<void>);
}


// equal

// std_equatable

EXPR_CHECK_UNARY(
        is_std_equatable,
        declvalr<T>() == declvalr<T>(),
        declvalr<T>() != declvalr<T>());

EXPR_CHECK_BINARY(
        are_std_equatable,
        declvalr<TLhs>() == declvalr<TRhs>(),
        declvalr<TLhs>() != declvalr<TRhs>(),

        declvalr<TRhs>() == declvalr<TLhs>(),
        declvalr<TRhs>() != declvalr<TLhs>());

COND_CONCEPT(std_equatable, is_std_equatable_gs<C>);

COND_CONCEPT_UNARY(std_equatable_with, are_std_equatable_gs<C, T>);


// equatable

COND_CHECK_UNARY(
        has_equal,
        same_vt<
                decl(declval<T>().equal(declval<T>())),
                bool>);

COND_CHECK_BINARY(
        have_equal,
        same_vt<
                decl(declval<TLhs>().equal(declval<TRhs>())),
                bool>);

COND_CHECK_UNARY(is_equatable, dis_vt<is_std_equatable_gs<T>, has_equal_gs<T>>);

COND_CHECK_BINARY(
        are_equatable,
        dis_vt<are_std_equatable_gs<TLhs, TRhs>, have_equal_gs<TLhs, TRhs>>);

COND_CONCEPT(equatable, is_equatable_gs<C>);

COND_CONCEPT_UNARY(equatable_with, are_equatable_gs<C, T>);


ENV_TEST_CASE("equatable")
{
    REQUIRES(is_equatable_g<int>);
    REQUIRES_FALSE(is_equatable_g<void>);

    REQUIRES(are_equatable_g<int, double>);
    REQUIRES_FALSE(are_equatable_g<void, int>);
}


// compare

// std_comparable

EXPR_CHECK_UNARY(
        is_std_comparable,
        declvalr<T>() < declvalr<T>(),
        declvalr<T>() > declvalr<T>(),
        declvalr<T>() <= declvalr<T>(),
        declvalr<T>() >= declvalr<T>());

EXPR_CHECK_BINARY(
        are_std_comparable,
        declvalr<TLhs>() < declvalr<TRhs>(),
        declvalr<TLhs>() > declvalr<TRhs>(),
        declvalr<TLhs>() <= declvalr<TRhs>(),
        declvalr<TLhs>() >= declvalr<TRhs>(),

        declvalr<TRhs>() < declvalr<TLhs>(),
        declvalr<TRhs>() > declvalr<TLhs>(),
        declvalr<TRhs>() <= declvalr<TLhs>(),
        declvalr<TRhs>() >= declvalr<TLhs>());

COND_CONCEPT(std_comparable, is_std_comparable_gs<C>);

COND_CONCEPT_UNARY(std_comparable_with, are_std_comparable_gs<C, T>);


// comparable

COND_CHECK_UNARY(
        has_compare,
        same_vt<
                decl(declval<T>().compare(declval<T>())),
                bool>);

COND_CHECK_BINARY(
        have_compare,
        same_vt<
                decl(declval<TLhs>().compare(declval<TRhs>())),
                bool>);

COND_CHECK_UNARY(
        is_comparable,
        dis_vt<is_std_comparable_gs<T>, has_compare_gs<T>>);

COND_CHECK_BINARY(
        are_comparable,
        dis_vt<are_std_comparable_gs<TLhs, TRhs>, have_compare_gs<TLhs, TRhs>>);

COND_CONCEPT(comparable, is_comparable_gs<C>);

COND_CONCEPT_UNARY(comparable_with, are_comparable_gs<C, T>);


ENV_TEST_CASE("comparable")
{
    REQUIRES(is_comparable_g<int>);
    REQUIRES_FALSE(is_comparable_g<void>);

    REQUIRES(are_comparable_g<int, double>);
    REQUIRES_FALSE(are_comparable_g<void, int>);
}


// key

COND_CHECK_UNARY(
        is_key,
        is_hashable_gs<T>,
        is_equatable_gs<T>,
        is_comparable_gs<T>);

COND_CONCEPT(key, is_key_gs<C>);


// advance

// std advanceable

EXPR_CHECK_UNARY(is_std_advanceable, ++declvall<T>(), declvall<T>()++);

COND_CONCEPT(std_advanceable, is_std_advanceable_gs<C>);


// advanceable

COND_CHECK_UNARY(
        has_advance,
        same_vt<decl(declvall<T>().advance()), T&>);

COND_CHECK_UNARY(
        is_advanceable,
        dis_vt<has_advance_gs<T>, is_std_advanceable_gs<T>>);

COND_CONCEPT(advanceable, is_advanceable_gs<C>);


ENV_TEST_CASE("advanceable")
{
    REQUIRES(is_advanceable_g<int>);
    REQUIRES(is_advanceable_g<int*>);
    REQUIRES(is_advanceable_g<const int*>);
    REQUIRES_FALSE(has_advance_g<const int* const>);
    REQUIRES_FALSE(is_std_advanceable_g<const int* const>);
    REQUIRES_FALSE(is_advanceable_g<const int* const>);
    REQUIRES_FALSE(is_advanceable_g<int* const>);
}


// range

EXPR_CHECK_UNARY(is_range, declval<T>().begin(), declval<T>().end());

ENV_TEST_CASE("range")
{
    REQUIRES(is_range_g<ENV_STD::vector<int>>);
    REQUIRES_FALSE(is_range_g<int>);
}


#endif // ENV_BEHAVIOUR_HPP
