#ifndef ENV_CALL_HPP
#define ENV_CALL_HPP

// first

FWA_CLANG_SUPPRESS_PUSH("OCUnusedTemplateParameterInspection")

tmp<name TResult, name... TIgnored>
    typ(first_gvt) = TResult;

tmp<name TResult, TResult Result, name... TIgnored>
    let_cmp first_gnv = Result;

tmp<name TResult, name TIgnored, TIgnored... Ignored>
    typ(first_ggat) = TResult;

tmp<name TResult, TResult Result, name TIgnored, TIgnored... Ignored>
    let_cmp first_gnga = Result;

#if FWA_CPP >= 17

tmp<deduc Result, name... TIgnored>
    let_cmp first_nv = Result;

tmp<deduc Result, deduc... Ignored>
    let_cmp first_na = Result;

#endif // FWA_CPP >= 17

FWA_CLANG_SUPPRESS_POP

ENV_TEST_CASE("first")
{
    REQUIRE_EQT(first_gvt<int, double>, int);
    REQUIRE_EQ(first_gnv<int, 1, int>, 1);
}

// apply

tmp<tmp<name...> class TKind, name TArgs, name = success_t>
    strct apply_kvs : type_gt<fail_t>{};

tmp<tmp<name...> class TKind, name... TArgs>
    strct apply_kvs<TKind, variadic_vt<TArgs...>, success_vt<TKind<TArgs...>>> : type_gt<TKind<TArgs...>>{};

tmp<tmp<name...> class TKind, name... TArgs>
    typ(apply_kvt) = name apply_kvs<TKind, variadic_vt<TArgs...>>::type;

tmp<name TFunctor, name TArgs, name = success_t>
    strct apply_tvs : type_gt<fail_t>{};

tmp<name TFunctor, name... TArgs>
    strct apply_tvs<TFunctor, variadic_vt<TArgs...>, success_vt<decltype(TFunctor{}(declvalr<TArgs>()...))>> : type_gt<decltype(TFunctor{}(declvalr<TArgs>()...))>{};

tmp<name TFunctor, name... TArgs>
    typ(apply_tvt) = name apply_tvs<TFunctor, variadic_vt<TArgs...>>::type;

ENV_TEST_CASE("apply")
{
    enm test : int{};

    SUBCASE("type")
    {
        REQUIRE_EQT(apply_kvt<FWA_STD::underlying_type_t, test>, int);
        REQUIRE_EQT(apply_kvt<FWA_STD::underlying_type_t, int>, fail_t);
    }

    SUBCASE("functor")
    {
        REQUIRE_EQT(apply_tvt<FWA_STD::plus<int>, int, int>, int);
        REQUIRE_EQT(apply_tvt<FWA_STD::plus<int>, FWA_STD::string_view, int>, fail_t);
    }
}

#endif // ENV_CALL_HPP
