#ifndef ENV_OTHER_REQUIREMENTS_HPP
#define ENV_OTHER_REQUIREMENTS_HPP


// it can be used as an argument to std::valarray
// no operations on it can throw
COND_CHECK_UNARY(
        is_std_numeric_type,
        neg_vt<ENV_STD::is_const<T>, ENV_STD::is_volatile<T>>,
        ENV_STD::is_object<T>,

        is_std_default_constructible_gs<T>,

        is_std_copy_constructible_gs<T>,
        is_std_copy_assignable_gs<T>,

        is_std_destructible_gs<T>);


// notes: https://en.cppreference.com/w/cpp/named_req/LiteralType
// This is deprecated in c++17 and removed in c++20:
// https://en.cppreference.com/w/cpp/types/is_std_literal_type
// what matters is that these types can be returned from constexpr functions
ENV_DETAIL_BEGIN

tmp<name T> cmp_fn get_literal() noex->T;

ENV_DETAIL_END

EXPR_CHECK_UNARY(is_std_literal_type, detail::get_literal<T>());

ENV_TEST_CASE("std literal")
{
    STD_REQUIRE(is_std_literal_type_g<int>);
}


// notes: https://en.cppreference.com/w/cpp/named_req/BitmaskType
// types that can represent a set of constant values
// (std::bitset, integers, enums...)
// for c++17 the operations have to be inline and constexpr
EXPR_CHECK_UNARY(
        is_std_bitmask_type,
        ~declval<T>(),

        declval<T>() & declval<T>(),
        declval<T>() | declval<T>(),
        declval<T>() ^ declval<T>(),

        declvall<T>() &= declval<T>(),
        declvall<T>() |= declval<T>(),
        declvall<T>() ^= declval<T>());


#endif // ENV_OTHER_REQUIREMENTS_HPP
