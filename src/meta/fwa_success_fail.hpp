#ifndef FWA_CORE_SUCCESS_FAIL_HPP
#define FWA_CORE_SUCCESS_FAIL_HPP


// tags

enm success_t : bool { } cmp_obj_p success{true};

enm fail_t : bool { } cmp_obj_p fail{false};


// check

tmp<name T> cmp_obj bool is_success_g{FWA_STD::is_same_v<T, success_t>};

#if FWA_CPP >= 17

tmp<deduc Expr> cmp_obj bool is_success_n{FWA_STD::is_same_v<decl(Expr), success_t>};

#else // FWA_CPP >= 17

tmp<nullptr_t Expr> cmp_obj bool is_success_n{true};

#endif // FWA_CPP >= 17

tmp<name T> cmp_obj bool is_fail_g{FWA_STD::is_same_v<T, fail_t>};

#if FWA_CPP >= 17

tmp<deduc Expr> cmp_obj bool is_fail_n{FWA_STD::is_same_v<decl(Expr), fail_t>};

#else // FWA_CPP >= 17

tmp<nullptr_t Expr> cmp_obj bool is_fail_n{true};

#endif // FWA_CPP >= 17

FWA_CORE_TEST_CASE("check success/fail")
{
    SUBCASE("success")
    {
        REQUIRES(is_success_g<success_t>);
        REQUIRES_FALSE(is_success_g<int>);

        REQUIRES(is_success_n<success>);

#if FWA_CPP >= 17
        REQUIRES_FALSE(is_success_n<1>);
#endif // FWA_CPP >= 17
    }

    SUBCASE("fail")
    {
        REQUIRES(is_fail_g<fail_t>);
        REQUIRES_FALSE(is_fail_g<int>);

        REQUIRES(is_fail_n<fail>);

#if FWA_CPP >= 17
        REQUIRES_FALSE(is_fail_n<1>);
#endif // FWA_CPP >= 17
    }
}


// make

// Explanation: https://stackoverflow.com/questions/66261570/c-sfinae-not-failing

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name... T>
strct make_success_vt : value_gnt<success_t, success_t{true}> { };

tmp<name... T>
strct make_fail_vt : value_gnt<fail_t, fail_t{false}> { };

FWA_NAMESPACE_DETAIL_END

tmp<name... T> typ(success_vt) = name detail::make_success_vt<T...>::value_type;

tmp<name... T> let_cmp success_v = detail::make_success_vt<T...>::value;

tmp<name... T> typ(fail_vt) = name detail::make_fail_vt<T...>::value_type;

tmp<name... T> let_cmp fail_v = detail::make_fail_vt<T...>::value;

FWA_CORE_TEST_CASE ("make success/fail")
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


#endif // FWA_CORE_SUCCESS_FAIL_HPP
