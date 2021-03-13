#ifndef ENV_LIBRARY_WIDE_STANDARD_REQUIREMENTS_HPP
#define ENV_LIBRARY_WIDE_STANDARD_REQUIREMENTS_HPP

COND_CHECK_UNARY(
    is_std_value_swappable,
    is_std_legacy_iterator_g<T> &&

        is_std_swappable_g<decl(*declval<T>())>);

ENV_TEST_CASE("value swappable")
{
    STD_REQUIRE(is_std_value_swappable_g<int *>);
    STD_REQUIRE_FALSE(is_std_value_swappable_g<test::forward_iterator_t>);
}

// notes: https://en.cppreference.com/w/cpp/named_req/NullablePointer
COND_CHECK_UNARY(
    is_std_nullable_pointer,
    is_std_equality_comparable_g<T> &&

            /* this should also produce a null value */
            is_std_default_constructible_g<T> &&

                is_std_copy_constructible_g<T> &&
                    is_std_copy_assignable_g<T> &&
                        is_std_destructible_g<T> &&

                            /* weirdly, there is a difference between the requirement of constructing an lvalue or an rvalue which
         * is impossible to check here */
                            EXPR_COND(T{nullptr}, declvall<T>() = nullptr) &&

        /* there are requirements on the effects of these */
        /* this one is just weird */
        ENV_STD::is_convertible_v<decl(declval<T>() != declval<T>()), bool> &&
        ENV_STD::is_convertible_v<decl(declval<T>() == nullptr), bool> &&
        ENV_STD::is_convertible_v<decl(nullptr == declval<T>()), bool> &&
        ENV_STD::is_convertible_v<decl(declval<T>() != nullptr), bool> &&
        ENV_STD::is_convertible_v<decl(nullptr != declval<T>()), bool>);

ENV_TEST_CASE("nullable pointer")
{
    STD_REQUIRE(is_std_nullable_pointer_g<int *>);
    STD_REQUIRE_FALSE(is_std_nullable_pointer_g<void>);
}

// notes: https://en.cppreference.com/w/cpp/named_req/FunctionObject
// this is very hard to get right, but I hope it is good enough
COND_CHECK_UNARY(
    is_std_function_object,
    ENV_STD::is_object_v<T> &&
            is_callable_g<T> &&
        !is_callable_member_g<T>);

ENV_TEST_CASE("function object")
{
    STD_REQUIRE(is_std_function_object_g<void (*)()>);
    STD_REQUIRE_FALSE(is_std_function_object_g<void()>);
}

// I already wrote this without even realizing it lol
COND_CHECK_UNARY(is_std_callable, is_callable_g<T>);

ENV_TEST_CASE("callable")
{
    STD_REQUIRE(is_std_callable_g<void()>);
    STD_REQUIRE_FALSE(is_std_callable_g<void>);
}

// notes: https://en.cppreference.com/w/cpp/named_req/Hash
COND_CHECK_UNARY(
    is_std_hash,
    is_std_function_object_g<T> &&
            is_std_copy_constructible_g<T> &&
                is_std_destructible_g<T> &&

                    /* I can't check anything else, other than if it's callable, because in order to get the return type,
         * I have to know the key type because it could be overloaded which is impossible  */
                    /* there are some more requirements other than the return type being a std::size_t */
                    is_callable_g<T> &&
        !is_callable_member_g<T>);

ENV_TEST_CASE("hash")
{
    STD_REQUIRE(is_std_hash_g<ENV_STD::hash<int>>);
    STD_REQUIRE_FALSE(is_std_hash_g<int>);
}

// Predicate, BinaryPredicate and Compare are impossible to check and the reason is the same as it is with hash

// notes: https://en.cppreference.com/w/cpp/named_req/Predicate
COND_CHECK_UNARY(
    is_std_predicate,
    is_callable_g<T> && !is_callable_member_g<T>);

ENV_TEST_CASE("predicate")
{
    STD_REQUIRE(is_std_predicate_g<bool(int)>);
    STD_REQUIRE_FALSE(is_std_predicate_g<void>);
}

// notes: https://en.cppreference.com/w/cpp/named_req/BinaryPredicate
COND_CHECK_UNARY(
    is_std_binary_predicate,
    is_callable_g<T> && !is_callable_member_g<T>);

ENV_TEST_CASE("binary predicate")
{
    STD_REQUIRE(is_std_binary_predicate_g<bool(int)>);
    STD_REQUIRE_FALSE(is_std_binary_predicate_g<void>);
}

// notes: https://en.cppreference.com/w/cpp/named_req/Compare
COND_CHECK_UNARY(
    is_std_compare,
    is_callable_g<T> && !is_callable_member_g<T>);

ENV_TEST_CASE("compare")
{
    STD_REQUIRE(is_std_compare_g<bool(int)>);
    STD_REQUIRE_FALSE(is_std_compare_g<void>);
}

#endif // ENV_LIBRARY_WIDE_STANDARD_REQUIREMENTS_HPP
