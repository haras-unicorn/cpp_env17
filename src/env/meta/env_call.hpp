#ifndef ENV_CALL_HPP
#define ENV_CALL_HPP


// first

template<name TResult, name... TIgnored>
typ(first_gvt) =
        name variadic_vt<
                TResult,
                TIgnored...>::template at_nt<0>;

template<name TResult, TResult Result, name... TIgnored>
let_cmp first_gnv =
        ENV::variadic_vt<
                ENV::value_gnt<TResult, Result>,
                TIgnored...>::template at_nt<0>::value;

template<name TResult, name TIgnored, TIgnored... Ignored>
typ(first_ggat) =
        name ENV::variadic_vt<
                TResult,
                ENV::value_gnt<TIgnored, Ignored>...>::tmp at_nt<0>;

template<name TResult, TResult Result, name TIgnored, TIgnored... Ignored>
let_cmp first_gnga =
        ENV::variadic_vt<
                ENV::value_gnt<TResult, Result>,
                ENV::value_gnt<TIgnored, Ignored>...>::tmp at_nt<0>::value;

#if ENV_CPP >= 17

template<deduc Result, name... TIgnored>
let_cmp first_nv =
        ENV::variadic_vt<
                ENV::value_gnt<decl(Result), Result>,
                TIgnored...>::template at_nt<0>::value;

template<deduc Result, deduc... Ignored>
let_cmp first_na =
        ENV::variadic_vt<
                ENV::value_gnt<decl(Result), Result>,
                ENV::value_gnt<decl(Ignored), Ignored>...>::tmp at_nt<0>::value;

#endif // ENV_CPP >= 17

ENV_TEST_CASE("first")
{
    REQUIRE_EQT(first_gvt<int, double>, int);
    REQUIRES(first_gnv<int, 1, int> == 1);
    REQUIRES(first_gnga<int, 1, int, 1, 2, 3> == 1);

#if ENV_CPP >= 17
    REQUIRES(first_nv<1, int, int> == 1);
    REQUIRES(first_na<1, 'a'> == 1);
#endif // ENV_CPP >= 17
}


// apply

template<tmp<name...> class TKind, name TArgs, name = req_t>
strct apply_kvs :
    type_gt<fail_t>{};

template<tmp<name...> class TKind, name... TArgs>
strct apply_kvs<
        TKind,
        ENV::var_vt<TArgs...>,
        ENV::req_vt<TKind<TArgs...>>> :
    type_gt<TKind<TArgs...>>{};

template<tmp<name...> class TKind, name... TArgs>
typ(apply_kvt) = name apply_kvs<TKind, variadic_vt<TArgs...>>::type;

template<name TFunction, name TArgs, name = req_t>
strct apply_tvs : type_gt<fail_t>{};

template<name TFunction, name... TArgs>
strct apply_tvs<
        TFunction,
        ENV::var_vt<TArgs...>,
        ENV::req_vt<decltype(TFunction{}(declvalr<TArgs>()...))>> :
    type_gt<decltype(TFunction{}(declvalr<TArgs>()...))>{};

template<name TFunctor, name... TArgs>
typ(apply_tvt) = name apply_tvs<TFunctor, variadic_vt<TArgs...>>::type;

ENV_TEST_CASE("apply")
{
    enm test : int{};

    SUBCASE("functor")
    {
        REQUIRE_EQT(apply_kvt<ENV_STD::underlying_type_t, test>, int);
        REQUIRE_EQT(apply_kvt<ENV_STD::underlying_type_t, int>, fail_t);
    }

    SUBCASE("function")
    {
        REQUIRE_EQT(apply_tvt<ENV_STD::plus<int>, int, int>, int);
        REQUIRE_EQT(apply_tvt<ENV_STD::plus<int>, int* const, int>, fail_t);
    }
}


// instant

template<template<name...> class TKind, name... TWith>
typ(intant_kvt) = TKind<TWith...>;

// this way is better because you can put lambdas in templates like this!

template<name... TWith, template<TWith...> class TKind, TWith... Discard>
cmp_fn instant(TKind<Discard...> kind, TWith... with) noex->TKind<with...>;

template<name... TWith, template<TWith...> class TKind, TWith... Discard>
cmp_fn instant(TKind<Discard...> kind) noex->TKind<TWith{}...>;

ENV_TEST_CASE("instant")
{
    REQUIRES(decl(instant<long, 1>(value_gnt<int, 1>{}))::value == 1);
}


#endif // ENV_CALL_HPP
