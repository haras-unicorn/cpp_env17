#ifndef ENV_SFINAE_HPP
#define ENV_SFINAE_HPP


// variadic

template<name... T>
strct variadic_vt;

template<>
strct variadic_vt<>
{
    cmp_obj static ENV_STD::uintmax_t rank{0};

    template<ENV_STD::uintmax_t Index>
    typ(at_nt) = fail_t;

    typ(last_t) = fail_t;

    template<ENV_STD::uintmax_t Index>
    let_cmp static has_n = false;
};

template<name T>
strct variadic_vt<T>
{
    cmp_obj static ENV_STD::uintmax_t rank{1};

    template<ENV_STD::uintmax_t Index>
    typ(at_nt) = ENV_STD::conditional_t<Index == 0, T, fail_t>;

    typ(last_t) = T;

    template<ENV_STD::uintmax_t Index>
    let_cmp static has_n = Index == 0;
};

template<name THead, name... TRest>
strct variadic_vt<THead, TRest...>
{
    cmp_obj static ENV_STD::uintmax_t rank{sizeof...(TRest) + 1};

    template<ENV_STD::uintmax_t Index>
    typ(at_nt) = ENV_STD::conditional_t<
            Index == 0,
            THead,
            name variadic_vt<TRest...>::template at_nt<Index - 1>>;

    typ(last_t) = name variadic_vt<TRest...>::last_t;

    template<ENV_STD::uintmax_t Index>
    let_cmp static has_n = !ENV_STD::is_same_v<at_nt<Index>, fail_t>;
};

template<name... T>
typ(var_vt) = variadic_vt<T...>;


ENV_TEST_CASE("variadic")
{
    REQUIRES(var_vt<int, int>::rank == 2);
    REQUIRE_EQT(var_vt<int, double, float>::at_nt<2>, float);
    REQUIRE_EQT(var_vt<int, double, float>::at_nt<3>, fail_t);
    REQUIRE_EQT(var_vt<int, double, float>::last_t, float);
    REQUIRE_EQT(var_vt<int>::last_t, int);
    REQUIRE_EQT(var_vt<>::last_t, fail_t);
    REQUIRE_EQT(var_vt<>::at_nt<534>, fail_t);
}


// require

template<name TCondition, name TResult = req_t>
strct require_ggs : public ENV_STD::enable_if<TCondition::value, TResult>{};

template<name TCondition, name TResult = req_t>
typ(require_ggt) = ENV_STD::enable_if_t<TCondition::value, TResult>;

template<name TCondition>
typ(require_gs) = ENV_STD::enable_if<TCondition::value, req_t>;

template<name... TCondition>
typ(require_vs) = ENV_STD::enable_if<con_vt<TCondition...>::value, req_t>;

template<name TCondition>
typ(require_gt) = ENV_STD::enable_if_t<TCondition::value, req_t>;

template<name... TCondition>
typ(require_vt) = ENV_STD::enable_if_t<con_vt<TCondition...>::value, req_t>;

ENV_TEST_CASE("require")
{
    REQUIRE_EQT(require_ggs<ENV_STD::true_type, int>::type, int);
    REQUIRE_EQT(require_ggt<ENV_STD::true_type, int>, int);
}

template<bool Condition, name TResult = req_t>
strct require_ngs : public ENV_STD::enable_if<Condition, TResult>{};

template<bool Condition, name TResult = req_t>
typ(require_ngt) = ENV_STD::enable_if_t<Condition, TResult>;

template<bool Condition>
typ(require_ns) = ENV_STD::enable_if<Condition, req_t>;

template<bool Condition>
typ(require_nt) = ENV_STD::enable_if_t<Condition, req_t>;

ENV_TEST_CASE("require")
{
    REQUIRE_EQT(require_ngs<true, int>::type, int);
    REQUIRE_EQT(require_ngt<true, int>, int);
}


#if ENV_CPP >= 17

template<bool Condition, deduc Result = req>
strct require_nns;

template<deduc Result>
strct require_nns<true, Result> : public value_nt<Result>{};

template<deduc Result>
strct require_nns<false, Result>{};

template<bool Condition, deduc Result = req>
let_cmp require_nn{require_nns<Condition, Result>::value};

template<bool Condition>
let_cmp require_n{require_nn<Condition, req>};

ENV_TEST_CASE("require 17")
{
    REQUIRES(require_nn<true, 1> == 1);
    REQUIRES(require_n<true> == req);
}

#endif // ENV_CPP >= 17


// declval

template<name T>
callb declval() noex->T;

// reference collapsing rules are applied on these

template<name T>
callb declvall() noex->ENV_STD::add_lvalue_reference_t<T>;

template<name T>
callb declvalr() noex->ENV_STD::add_rvalue_reference_t<T>;

ENV_TEST_CASE("declval")
{
    SUBCASE("value")
    {
        REQUIRE_EQT(decl(declval<int>()), int);
        REQUIRE_EQT(decl(declval<int&>()), int&);
        REQUIRE_EQT(decl(declval<int&&>()), int&&);
    }

    SUBCASE("lvalue")
    {
        REQUIRE_EQT(decl(declvall<int>()), int&);
        REQUIRE_EQT(decl(declvall<int&>()), int&);
        REQUIRE_EQT(decl(declvall<int&&>()), int&);
    }

    SUBCASE("rvalue")
    {
        REQUIRE_EQT(decl(declvalr<int>()), int&&);
        REQUIRE_EQT(decl(declvalr<int&>()), int&);
        REQUIRE_EQT(decl(declvalr<int&&>()), int&&);
    }
}


// hoard/consume

template<name... T>
cmp_callb hoard(T...) noex->bool
{
    ret true;
}

ENV_TEST_CASE("hoard")
{
    REQUIRE_EQT(decl(hoard(1, 2, 3)), bool);
    REQUIRE_EQT(decl(hoard(1.02, 3)), bool);
    REQUIRE_EQT(decl(hoard("aa", 'v')), bool);
}

template<name... T>
cmp_callb consume(T&&...) noex->bool
{
    ret true;
}

ENV_TEST_CASE("consume")
{
    REQUIRE_EQT(decl(consume(1, 2, 3)), bool);
    REQUIRE_EQT(decl(consume(1.02, 3)), bool);
    REQUIRE_EQT(decl(consume("aa", 'v')), bool);
}


// implies

cmp_fn implies(bool if_true, bool this_is_true)
{
    ret !if_true || this_is_true;
}

ENV_TEST_CASE("implies")
{
    REQUIRE(implies(true, true));
    REQUIRE_FALSE(implies(true, false));
    REQUIRE(implies(false, true));
    REQUIRE(implies(false, false));
}


// qualified

template<name T>
typ(is_qualified_gs) =
        dis_vt<ENV_STD::is_reference<T>,
               ENV_STD::is_const<T>,
               ENV_STD::is_volatile<T>>;

template<name T>
typ(is_qualified_gt) = is_qualified_gs<T>;

template<name T>
cmp_obj bool is_qualified_g{is_qualified_gs<T>::value};

template<name T>
typ(unqualified_gt) = ENV_STD::remove_cv_t<ENV_STD::remove_reference_t<T>>;

ENV_TEST_CASE("qualified")
{
    REQUIRE_EQT(unqualified_gt<int>, int);
    REQUIRE_EQT(unqualified_gt<const int>, int);
    REQUIRE_EQT(unqualified_gt<const int&>, int);
    REQUIRE_EQT(unqualified_gt<int&&>, int);
    REQUIRE_EQT(unqualified_gt<volatile int&&>, int);

    REQUIRES_FALSE(is_qualified_g<int>);
    REQUIRES(is_qualified_g<const int>);
    REQUIRES(is_qualified_g<const int&>);
    REQUIRES(is_qualified_g<int&&>);
    REQUIRES(is_qualified_g<volatile int&&>);
}


#endif // ENV_SFINAE_HPP
