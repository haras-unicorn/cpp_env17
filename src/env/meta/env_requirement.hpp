#ifndef ENV_REQUIREMENT_HPP
#define ENV_REQUIREMENT_HPP


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


#endif // ENV_REQUIREMENT_HPP
