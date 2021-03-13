#ifndef FWA_CORE_BASIC_STANDARD_REQUIREMENTS_HPP
#define FWA_CORE_BASIC_STANDARD_REQUIREMENTS_HPP


// basic

EXPR_CHECK_UNARY(is_std_default_constructible, T{ }, T());

FWA_CORE_TEST_CASE("default constructible")
{
    STD_REQUIRE(is_std_default_constructible_g<int>);
    STD_REQUIRE_FALSE(is_std_default_constructible_g<void>);
}

EXPR_CHECK_UNARY(is_std_copy_constructible, declvall<T>() = declvall<const T>(), T(declvall<const T>()));

FWA_CORE_TEST_CASE("copy constructible")
{
    STD_REQUIRE(is_std_copy_constructible_g < int >);
    STD_REQUIRE_FALSE(is_std_copy_constructible_g < void >);
}

EXPR_CHECK_UNARY(is_std_move_constructible, declvall<T>() = declvalr<T>(), T(declvalr<T>()));

FWA_CORE_TEST_CASE("move constructible")
{
    STD_REQUIRE(is_std_move_constructible_g < int >);
    STD_REQUIRE_FALSE(is_std_move_constructible_g < void >);
}

COND_CHECK_UNARY(is_std_copy_assignable, FWA_STD::is_same_v < decl(declvall<T>() = declvall<const T>()), T &>);

FWA_CORE_TEST_CASE("copy assignable")
{
    STD_REQUIRE(is_std_copy_assignable_g < int >);
    STD_REQUIRE(is_std_copy_constructible_g < FWA_STD::pair < int, int > >);
    STD_REQUIRE_FALSE(is_std_copy_constructible_g < void >);
}

COND_CHECK_UNARY(is_std_move_assignable, FWA_STD::is_same_v < decl(declvall<T>() = declvalr<T>()), T &>);

FWA_CORE_TEST_CASE("move assignable")
{
    STD_REQUIRE(is_std_move_assignable_g < int >);
    STD_REQUIRE(is_std_move_assignable_g < FWA_STD::pair < int, int > >);
    STD_REQUIRE_FALSE(is_std_move_constructible_g < void >);
}

// this also has requirement of "All resources owned by u are reclaimed, no exceptions are thrown.",
// but there is no way to check for that.
// I could check for noexcept, but that doesn't really mean the function doesn't throw.
// MSVC permits destructor on void...
EXPR_CHECK_UNARY(is_std_destructible, COND_EXPR(!FWA_STD::is_same_v < T, void > ), declval<T>().~T());

FWA_CORE_TEST_CASE("destructible")
{
    STD_REQUIRE(is_std_destructible_g < int >);
    STD_REQUIRE_FALSE(is_std_destructible_g < void >);
}


// type properties - it would be very hard or impossible to check for these without the standard type traits.

// eligible for memcpy
COND_CHECK_UNARY(is_std_trivially_copyable, FWA_STD::is_trivially_copyable_v < T >);

FWA_CORE_TEST_CASE("trivially copyable")
{
    STD_REQUIRE(is_std_trivially_copyable_g < int >);
    STD_REQUIRE_FALSE(is_std_trivially_copyable_g < void >);
}

// trivially copyable with a trivial default constructor
COND_CHECK_UNARY(is_std_trivial, FWA_STD::is_trivial_v < T >);

FWA_CORE_TEST_CASE("trivial")
{
    STD_REQUIRE(is_std_trivial_g < int >);
    STD_REQUIRE_FALSE(is_std_trivial_g < void >);
}

// usable in other programming languages
COND_CHECK_UNARY(is_std_standard_layout, FWA_STD::is_standard_layout_v < T >);

FWA_CORE_TEST_CASE("standard layout")
{
    STD_REQUIRE(is_std_standard_layout_g < int >);
    STD_REQUIRE_FALSE(is_std_standard_layout_g < void >);
}

// a C struct
COND_CHECK_UNARY(is_std_pod, FWA_STD::is_pod_v < T >);

FWA_CORE_TEST_CASE("pod")
{
    STD_REQUIRE(is_std_pod_g < int >);
    STD_REQUIRE_FALSE(is_std_pod_g < void >);
}


// library-wide - defined here because these are really common

// notes: https://en.cppreference.com/w/cpp/named_req/EqualityComparable
COND_CHECK_UNARY
(is_std_equality_comparable, FWA_STD::is_convertible_v < decl(declval<const T>() == declval<const T>()), bool >);

FWA_CORE_TEST_CASE("equality comparable")
{
    STD_REQUIRE(is_std_equality_comparable_g < int >);
    STD_REQUIRE_FALSE(is_std_equality_comparable_g < void >);
}

// notes: https://en.cppreference.com/w/cpp/named_req/LessThanComparable
COND_CHECK_UNARY
(is_std_less_than_comparable, FWA_STD::is_convertible_v < decl(declval<const T>() < declval<const T>()), bool >);

FWA_CORE_TEST_CASE("less than comparable")
{
    STD_REQUIRE(is_std_less_than_comparable_g < int >);
    STD_REQUIRE_FALSE(is_std_less_than_comparable_g < void >);
}

// notes: https://en.cppreference.com/w/cpp/named_req/Swappable
EXPR_CHECK_UNARY
(is_std_swappable, FWA_STD::swap(declvall<T>(), declvall<T>()));

FWA_CORE_TEST_CASE("swappable")
{
    STD_REQUIRE(is_std_swappable_g < int >);
    STD_REQUIRE_FALSE(is_std_swappable_g < void >);
}


#endif // FWA_CORE_BASIC_STANDARD_REQUIREMENTS_HPP
