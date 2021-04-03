#ifndef ENV_VARIADIC_HPP
#define ENV_VARIADIC_HPP


template<name... T>
typ(variadic_vt) = ENV_HANA::tuple<T...>;

template<name... T>
typ(var_vt) = variadic_vt<T...>;

let_cmp var = ENV_HANA::make_tuple;


template<template<name...> class TKind, name TVar>
strct apply_kgs;

template<template<name...> class TKind, name... TVar>
strct apply_kgs<TKind, var_vt<TVar...>>
{
    typ(type) = TKind<TVar...>;
};

template<template<name...> class TKind, name TVar>
typ(apply_kgt) = name apply_kgs<TKind, TVar>::type;


ENV_TEST_CASE("variadic")
{
    REQUIRES(var_vt<int, int>{}.size() == 2);
    REQUIRE_EQT(var_vt<int, double, float>{}[2_c], float);
    REQUIRE_EQT(var_vt<int, double, float>{}[3_c], fail_t);
    REQUIRE_EQT(var_vt<int, double, float>::last_t, float);
    REQUIRE_EQT(var_vt<int>::last_t, int);
    REQUIRE_EQT(var_vt<>::last_t, fail_t);
    REQUIRE_EQT(var_vt<>::at_nt<534>, fail_t);
}


#endif // ENV_VARIADIC_HPP
