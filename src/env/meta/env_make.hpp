#ifndef ENV_MAKE_HPP
#define ENV_MAKE_HPP


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


#endif // ENV_MAKE_HPP
