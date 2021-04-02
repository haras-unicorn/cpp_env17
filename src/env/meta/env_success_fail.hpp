#ifndef ENV_SUCCESS_FAIL_HPP
#define ENV_SUCCESS_FAIL_HPP


// tags

enm       succ_t : bool { }
cmp_obj_p succ{true};

enm       fail_t : bool { }
cmp_obj_p fail{false};


typ(req_t) = succ_t;
let_cmp req{succ_t{true}};


// check

template<name T>
cmp_obj bool is_succ_g{ENV_STD::is_same_v<T, succ_t>};

#if ENV_CPP >= 17

template<deduc Expr>
cmp_obj bool is_succ_n{
        ENV_STD::is_same_v<ENV_STD::remove_const_t<decl(Expr)>, succ_t>};

#else // ENV_CPP >= 17

tmp<nullptr_t Expr> cmp_obj bool is_succ_n{true};

#endif // ENV_CPP >= 17


template<name T>
cmp_obj bool is_fail_g{
        ENV_STD::is_same_v<T, fail_t>};

#if ENV_CPP >= 17

template<deduc Expr>
cmp_obj bool is_fail_n{
        ENV_STD::is_same_v<ENV_STD::remove_const_t<decl(Expr)>, fail_t>};

#else // ENV_CPP >= 17

tmp<fail_t Expr> cmp_obj bool is_fail_n{true};

#endif // ENV_CPP >= 17


template<name T>
cmp_obj bool is_req_g{
        ENV_STD::is_same_v<T, req_t>};

#if ENV_CPP >= 17

template<deduc Expr>
cmp_obj bool is_req_n{
        ENV_STD::is_same_v<
                ENV_STD::remove_const_t<decl(Expr)>,
                req_t>};

#else // ENV_CPP >= 17

tmp<nullptr_t Expr> cmp_obj bool is_req_n{true};

#endif // ENV_CPP >= 17


ENV_TEST_CASE("check succ/fail")
{
    SUBCASE("succ")
    {
        REQUIRES(is_succ_g<succ_t>);
        REQUIRES_FALSE(is_succ_g<int>);

        REQUIRES(is_succ_n<succ>);

#if ENV_CPP >= 17
        REQUIRES_FALSE(is_succ_n<fail>);
#endif // ENV_CPP >= 17
    }

    SUBCASE("fail")
    {
        REQUIRES(is_fail_g<fail_t>);
        REQUIRES_FALSE(is_fail_g<int>);

        REQUIRES(is_fail_n<fail>);

#if ENV_CPP >= 17
        REQUIRES_FALSE(is_fail_n<succ>);
#endif // ENV_CPP >= 17
    }

    SUBCASE("req")
    {
        REQUIRES(is_req_g<req_t>);
        REQUIRES_FALSE(is_req_g<int>);

        REQUIRES(is_req_n<req>);
        REQUIRES(is_req_n<succ>);

#if ENV_CPP >= 17
        REQUIRES_FALSE(is_req_n<fail>);
#endif // ENV_CPP >= 17
    }
}


// make

// Explanation: https://stackoverflow.com/questions/66261570/c-sfinae-not-failing

ENV_DETAIL_BEGIN

template<name... T>
strct make_succ_vt : value_gnt<succ_t, succ_t{true}>{};

template<name... T>
strct make_true_vt : value_gnt<bool, true>{};


template<name... T>
strct make_fail_vt : value_gnt<fail_t, fail_t{false}>{};

template<name... T>
strct make_false_vt : value_gnt<bool, false>{};


template<name... T>
strct make_req_vt : value_gnt<req_t, req_t{true}>{};


template<name TRes, TRes Res, name... T>
strct make_gnvt : value_gnt<TRes, Res>{};

template<name TRes, name... T>
strct make_vt : type_gt<TRes>{};


ENV_DETAIL_END


template<name... T>
typ(succ_vt) = name detail::make_succ_vt<T...>::value_type;

template<name... T>
cmp_obj succ_t succ_v = detail::make_succ_vt<T...>::value;

template<name... T>
typ(true_vt) = name detail::make_true_vt<T...>::type;

template<name... T>
cmp_obj bool true_v = detail::make_true_vt<T...>::value;


template<name... T>
typ(fail_vt) = name detail::make_fail_vt<T...>::value_type;

template<name... T>
cmp_obj fail_t fail_v = detail::make_fail_vt<T...>::value;

template<name... T>
typ(false_vt) = name detail::make_false_vt<T...>::type;

template<name... T>
cmp_obj bool false_v = detail::make_false_vt<T...>::value;


template<name... T>
typ(req_vt) = name detail::make_req_vt<T...>::value_type;

template<name... T>
cmp_obj req_t req_v = detail::make_req_vt<T...>::value;


template<name TRes, TRes Res, name... T>
typ(make_gnvt) = name detail::make_gnvt<TRes, Res, T...>::type;

template<name TRes, name... T>
typ(make_vt) = name detail::make_vt<TRes, T...>::type;


ENV_TEST_CASE("make")
{
    SUBCASE("succ")
    {
        REQUIRE_EQT(succ_vt<int, float, double>, succ_t);
        REQUIRES(succ_v<int, float, double> == succ);
        REQUIRES(true_v<int, float, double>);
    }
    SUBCASE("fail")
    {
        REQUIRE_EQT(fail_vt<int, float, double>, fail_t);
        REQUIRES(fail_v<int, float, double> == fail);
        REQUIRES_FALSE(false_v<int, float, double>);
    }
    SUBCASE("make")
    {
        REQUIRE_EQT(make_gnvt<int, 1, float, double, fail_t>::value_type, int);
    }
    SUBCASE("req")
    {
        REQUIRE_EQT(req_vt<int, float, double>, req_t);
        REQUIRES(req_v<int, float, double> == req);
    }
}


#endif // ENV_SUCCESS_FAIL_HPP
