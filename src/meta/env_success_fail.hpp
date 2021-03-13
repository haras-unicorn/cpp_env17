#ifndef ENV_SUCCESS_FAIL_HPP
#define ENV_SUCCESS_FAIL_HPP

// tags

enm success_t : bool {}
cmp_obj_p success{true};

enm fail_t : bool {}
cmp_obj_p fail{false};

// check

tmp<name T> cmp_obj bool is_success_g{ENV_STD::is_same_v<T, success_t>};

#if ENV_CPP >= 17

tmp<deduc Expr> cmp_obj bool is_success_n{ENV_STD::is_same_v<ENV_STD::remove_const_t<decl(Expr)>, success_t>};

#else // ENV_CPP >= 17

tmp<nullptr_t Expr> cmp_obj bool is_success_n{true};

#endif // ENV_CPP >= 17

tmp<name T> cmp_obj bool is_fail_g{ENV_STD::is_same_v<T, fail_t>};

#if ENV_CPP >= 17

tmp<deduc Expr> cmp_obj bool is_fail_n{ENV_STD::is_same_v<ENV_STD::remove_const_t<decl(Expr)>, fail_t>};

#else // ENV_CPP >= 17

tmp<nullptr_t Expr> cmp_obj bool is_fail_n{true};

#endif // ENV_CPP >= 17

ENV_TEST_CASE("check success/fail")
{
    SUBCASE("success")
    {
        REQUIRES(is_success_g<success_t>);
        REQUIRES_FALSE(is_success_g<int>);

        REQUIRES(is_success_n<success>);

#if ENV_CPP >= 17
        REQUIRES_FALSE(is_success_n<1>);
#endif // ENV_CPP >= 17
    }

    SUBCASE("fail")
    {
        REQUIRES(is_fail_g<fail_t>);
        REQUIRES_FALSE(is_fail_g<int>);

        REQUIRES(is_fail_n<fail>);

#if ENV_CPP >= 17
        REQUIRES_FALSE(is_fail_n<1>);
#endif // ENV_CPP >= 17
    }
}

// make

// Explanation: https://stackoverflow.com/questions/66261570/c-sfinae-not-failing

ENV_NAMESPACE_DETAIL_BEGIN

tmp<name... T> strct make_success_vt : value_gnt<success_t, success_t{true}>{};

tmp<name... T> strct make_fail_vt : value_gnt<fail_t, fail_t{false}>{};

ENV_NAMESPACE_DETAIL_END

tmp<name... T> typ(success_vt) = name detail::make_success_vt<T...>::value_type;

tmp<name... T> let_cmp success_v = detail::make_success_vt<T...>::value;

tmp<name... T> typ(fail_vt) = name detail::make_fail_vt<T...>::value_type;

tmp<name... T> let_cmp fail_v = detail::make_fail_vt<T...>::value;

ENV_TEST_CASE("make success/fail")
{
    SUBCASE("success")
    {
        REQUIRE_EQT(success_vt<int, float, double>, success_t);
        REQUIRES(success_v<int, float, double> == success);
    }
    SUBCASE("fail")
    {
        REQUIRE_EQT(fail_vt<int, float, double>, fail_t);
        REQUIRES(fail_v<int, float, double> == fail);
    }
}

#endif // ENV_SUCCESS_FAIL_HPP
