#ifndef ENV_BASIC_STANDARD_REQUIREMENTS_HPP
#define ENV_BASIC_STANDARD_REQUIREMENTS_HPP


// basic

EXPR_CHECK_UNARY(
        is_std_default_constructible,
        T{},
        T());

ENV_TEST_CASE("default constructible")
{
    STD_REQUIRE(is_std_default_constructible_g<int>);
    STD_REQUIRE_FALSE(is_std_default_constructible_g<void>);
}

EXPR_CHECK_UNARY(
        is_std_copy_constructible,
        declvall<T>() = declvall<const T>(),
        T(declvall<const T>()));

ENV_TEST_CASE("copy constructible")
{
    STD_REQUIRE(is_std_copy_constructible_g<int>);
    STD_REQUIRE_FALSE(is_std_copy_constructible_g<void>);
}

EXPR_CHECK_UNARY(
        is_std_move_constructible,
        declvall<T>() = declvalr<T>(),
        T(declvalr<T>()));

ENV_TEST_CASE("move constructible")
{
    STD_REQUIRE(is_std_move_constructible_g<int>);
    STD_REQUIRE_FALSE(is_std_move_constructible_g<void>);
}

COND_CHECK_UNARY(
        is_std_copy_assignable,
        same_vt<decl(declvall<T>() = declvall<const T>()), T&>);

ENV_TEST_CASE("copy assignable")
{
    STD_REQUIRE(is_std_copy_assignable_g<int>);
    STD_REQUIRE(is_std_copy_constructible_g<ENV_STD::pair<int, int>>);
    STD_REQUIRE_FALSE(is_std_copy_constructible_g<void>);
}

COND_CHECK_UNARY(
        is_std_move_assignable,
        same_vt<decl(declvall<T>() = declvalr<T>()), T&>);

ENV_TEST_CASE("move assignable")
{
    STD_REQUIRE(is_std_move_assignable_g<int>);
    STD_REQUIRE(is_std_move_assignable_g<ENV_STD::pair<int, int>>);
    STD_REQUIRE_FALSE(is_std_move_constructible_g<void>);
}

// this also has requirement of "All resources owned by u are reclaimed,
// no exceptions are thrown.",
// but there is no way to check for that.
// I could check for noexcept, but that doesn't really mean
// the function doesn't throw.
// Destructor on void is permitted in MSVC? *sigh*

COND_CHECK_UNARY(
        is_std_destructible,
        ENV_STD::is_destructible<T>);

ENV_TEST_CASE("destructible")
{
    STD_REQUIRE(is_std_destructible_g<int>);
    STD_REQUIRE_FALSE(is_std_destructible_g<void>);
}


// type properties - it would be very hard or impossible to check for these
// without the standard type traits.

// eligible for memcpy
COND_CHECK_UNARY(
        is_std_trivially_copyable,
        ENV_STD::is_trivially_copyable<T>);

ENV_TEST_CASE("trivially copyable")
{
    STD_REQUIRE(is_std_trivially_copyable_g<int>);
    STD_REQUIRE_FALSE(is_std_trivially_copyable_g<void>);
}

// trivially copyable with a trivial default constructor
COND_CHECK_UNARY(
        is_std_trivial,
        ENV_STD::is_trivial<T>);

ENV_TEST_CASE("trivial")
{
    STD_REQUIRE(is_std_trivial_g<int>);
    STD_REQUIRE_FALSE(is_std_trivial_g<void>);
}

// usable in other programming languages
COND_CHECK_UNARY(
        is_std_standard_layout,
        ENV_STD::is_standard_layout<T>);

ENV_TEST_CASE("standard layout")
{
    STD_REQUIRE(is_std_standard_layout_g<int>);
    STD_REQUIRE_FALSE(is_std_standard_layout_g<void>);
}

// a C struct - trivial and standard layout
COND_CHECK_UNARY(
        is_std_pod,
        ENV_STD::is_pod<T>);

ENV_TEST_CASE("pod")
{
    STD_REQUIRE(is_std_pod_g<int>);
    STD_REQUIRE_FALSE(is_std_pod_g<void>);
}


// library-wide - defined here because these are really common

// notes: https://en.cppreference.com/w/cpp/named_req/EqualityComparable
COND_CHECK_UNARY(
        is_std_equality_comparable,
        ENV_STD::is_convertible<
                decl(declval<const T>() == declval<const T>()),
                bool>);

ENV_TEST_CASE("equality comparable")
{
    STD_REQUIRE(is_std_equality_comparable_g<int>);
    STD_REQUIRE_FALSE(is_std_equality_comparable_g<void>);
}

// notes: https://en.cppreference.com/w/cpp/named_req/LessThanComparable
COND_CHECK_UNARY(
        is_std_less_than_comparable,
        ENV_STD::is_convertible<
                decl(declval<const T>() < declval<const T>()),
                bool>);

ENV_TEST_CASE("less than comparable")
{
    STD_REQUIRE(is_std_less_than_comparable_g<int>);
    STD_REQUIRE_FALSE(is_std_less_than_comparable_g<void>);
}

// notes: https://en.cppreference.com/w/cpp/named_req/Swappable
EXPR_CHECK_UNARY(
        is_std_swappable,
        ENV_STD::swap(declvall<T>(), declvall<T>()));

ENV_TEST_CASE("swappable")
{
    STD_REQUIRE(is_std_swappable_g<int>);
    STD_REQUIRE_FALSE(is_std_swappable_g<void>);
}


#endif // ENV_BASIC_STANDARD_REQUIREMENTS_HPP
