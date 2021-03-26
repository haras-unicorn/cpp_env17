#ifndef ENV_EXCEPTION_HPP
#define ENV_EXCEPTION_HPP


// debug/production

#ifdef NDEBUG
#define DEBUG_FLAG ENV::falsy
#else
#define DEBUG_FLAG ENV::truthy
#endif

cmp_obj flag_t is_debug{DEBUG_FLAG};

cmp_obj flag_t is_production{!is_debug};

ENV_TEST_CASE("debug / production")
{
    REQUIRES(is_debug != is_production);
}


// literals

WHOLE_L(error_code, err, int);

TEXT_L(message, msg, const char *, l_t);

TEXT_L(description, dsc, const char *, l_t);


// policies

ENV_DETAIL_BEGIN

strct exception_policy_s { };

ENV_DETAIL_END

strct ignore_exceptions_s : public detail::exception_policy_s { };

strct throw_on_exception_s : public detail::exception_policy_s { };

strct exit_on_exception_s : public detail::exception_policy_s { };

strct check_exceptions_s : public detail::exception_policy_s { };

strct adjust_on_exception_s : public detail::exception_policy_s { };

typ(ignore_ex_t) = ignore_exceptions_s;

typ(throw_ex_t) = throw_on_exception_s;

typ(exit_ex_t) = exit_on_exception_s;

typ(check_ex_t) = check_exceptions_s;

typ(adjust_ex_t) = adjust_on_exception_s;

cmp_obj ignore_ex_t ignore_ex{ };

cmp_obj throw_ex_t throw_ex{ };

cmp_obj exit_ex_t exit_ex{ };

cmp_obj check_ex_t check_ex{ };

cmp_obj adjust_ex_t adjust_ex{ };


// policy concepts

COND_CHECK_UNARY
(
        is_exception_policy,
        (ENV_STD::is_base_of_v < detail::exception_policy_s, T >)
);

COND_CONCEPT(exception_policy, (is_exception_policy_g<C>));

COND_CHECK_UNARY
(
        is_unignorable_policy,
        (is_exception_policy_g<T>) &&
        (!ENV_STD::is_same_v < T, ignore_exceptions_s >)
);

COND_CONCEPT(unignorable_policy, (is_unignorable_policy_g<C>));

ENV_TEST_CASE("exception policies")
{
    REQUIRES(is_exception_policy_g < unqualified_gt<decl(ignore_ex)>>);
    REQUIRES(is_exception_policy_g < unqualified_gt<decl(throw_ex)>>);
    REQUIRES(is_exception_policy_g < unqualified_gt<decl(exit_ex)>>);
    REQUIRES(is_exception_policy_g < unqualified_gt<decl(check_ex)>>);
    REQUIRES(is_exception_policy_g < unqualified_gt<decl(adjust_ex)>>);

    REQUIRES_FALSE(is_unignorable_policy_g < unqualified_gt<decl(ignore_ex)>>);
    REQUIRES(is_unignorable_policy_g < unqualified_gt<decl(throw_ex)>>);
    REQUIRES(is_unignorable_policy_g < unqualified_gt<decl(exit_ex)>>);
    REQUIRES(is_unignorable_policy_g < unqualified_gt<decl(check_ex)>>);
    REQUIRES(is_unignorable_policy_g < unqualified_gt<decl(adjust_ex)>>);
}


// default policies

typ(d_exception_policy_s) =
ENV_STD::conditional_t<
        is_debug, throw_on_exception_s,
        ignore_exceptions_s>;

cmp_obj d_exception_policy_s d_exception_policy{ };

typ(d_unignorable_policy_s) =
ENV_STD::conditional_t<
        is_debug, throw_on_exception_s,
        adjust_on_exception_s>;

cmp_obj d_unignorable_policy_s d_unignorable_policy{ };

ENV_TEST_CASE("default exception policies")
{
    REQUIRES(is_exception_policy_g <
             unqualified_gt<decl(d_exception_policy)>>);
    REQUIRES(is_unignorable_policy_g <
             unqualified_gt<decl(d_unignorable_policy)>>);
}


// status

struct [[OBJECT_ATTRIBUTES]] exception_status_s
{
    let_cmp static state_message{"Invalid state."_msg};
    let_cmp static state_code{10_err};

    let_cmp static argument_message{"Invalid arguments."_msg};
    let_cmp static argument_code{11_err};

    let_cmp static default_message{""_msg};
    let_cmp static default_code{-1_err};
};

ENV_TEST_CASE("exception status")
{
    REQUIRE_EQT(unqualified_gt<decltype(exception_status_s::state_message)>,
                message_t);
    REQUIRE_EQT(unqualified_gt<decltype(exception_status_s::argument_message)>,
                message_t);
    REQUIRE_EQT(unqualified_gt<decltype(exception_status_s::default_message)>,
                message_t);

    REQUIRE_EQT(unqualified_gt<decltype(exception_status_s::state_code)>,
                error_code_t);
    REQUIRE_EQT(unqualified_gt<decltype(exception_status_s::argument_code)>,
                error_code_t);
    REQUIRE_EQT(unqualified_gt<decltype(exception_status_s::default_code)>,
                error_code_t);
}

// exceptions

struct invalid_state_t : public ENV_STD::logic_error
{
    using logic_error::logic_error;
};

struct invalid_argument_t : public ENV_STD::logic_error
{
    using logic_error::logic_error;
};


// handling

ENV_CLANG_SUPPRESS_PUSH("ConstantConditionsOC")
ENV_CLANG_SUPPRESS_PUSH("UnreachableCode")

callb exit(message_t message)
{
    CMP_ON((is_debug), __debugbreak(););

    if (message == exception_status_s::state_message)
        ENV_STD::exit(exception_status_s::state_code);
    if (message == exception_status_s::argument_message)
        ENV_STD::exit(exception_status_s::argument_code);

    ENV_STD::exit(exception_status_s::default_code);
}

ENV_CLANG_SUPPRESS_POP
ENV_CLANG_SUPPRESS_POP

COND_TMP_UNARY((ENV_STD::is_base_of_v < ENV_STD::exception, T >))
callb except(message_t message) { throw T{message.data()}; }


// TODO: tests for unignorable stuff

// args

#define EXCEPTION_POLICY_TYPE_NAME TExcPol

#define EXCEPTION_POLICY_TEMPLATE_ARGUMENT                                                     \
        name EXCEPTION_POLICY_TYPE_NAME = ENV::d_exception_policy_s,                           \
        ENV::is_exception_policy_r<EXCEPTION_POLICY_TYPE_NAME> =                               \
        ENV::requirement

#define UNIGNORABLE_POLICY_TEMPLATE_ARGUMENT                                                   \
        name EXCEPTION_POLICY_TYPE_NAME = ENV::d_unignorable_policy_s,                         \
        ENV::is_unignorable_policy_r<EXCEPTION_POLICY_TYPE_NAME> =                             \
        ENV::requirement

#define EXCEPTION_POLICY_ARGUMENT_NAME exc_pol

#define EXCEPTION_POLICY_RUNTIME_ARGUMENT                                                      \
        nonced EXCEPTION_POLICY_TYPE_NAME EXCEPTION_POLICY_ARGUMENT_NAME =                     \
        EXCEPTION_POLICY_TYPE_NAME{}

// shorthands

#define exc_name EXCEPTION_POLICY_TEMPLATE_ARGUMENT
#define unignore_name UNIGNORABLE_POLICY_TEMPLATE_ARGUMENT
#define exc_arg EXCEPTION_POLICY_RUNTIME_ARGUMENT

// noex

#define UNQUALIFIED_POLICY_TYPE \
        ENV::unqualified_gt<EXCEPTION_POLICY_TYPE_NAME>

#define IS_NO_THROW_EXCEPTION_POLICY \
        !ENV_STD::is_same_v<         \
                UNQUALIFIED_POLICY_TYPE, \
                ENV::throw_on_exception_s>

#define NOEX_IF_NO_THROW_EXCEPTION_POLICY \
        noex(IS_NO_THROW_EXCEPTION_POLICY)

// shorthand

#define is_noex_pol IS_NO_THROW_EXCEPTION_POLICY
#define noex_pol NOEX_IF_NO_THROW_EXCEPTION_POLICY


// when

#define SHOULD_IGNORE_EXCEPTION \
        ENV_STD::is_same_v<UNQUALIFIED_POLICY_TYPE, ENV::ignore_exceptions_s>

#define SHOULD_EXIT_ON_EXCEPTION \
        ENV_STD::is_same_v<UNQUALIFIED_POLICY_TYPE, ENV::exit_on_exception_s>

#define SHOULD_THROW_ON_EXCEPTION \
        ENV_STD::is_same_v<UNQUALIFIED_POLICY_TYPE, ENV::throw_on_exception_s>

#define SHOULD_CHECK_EXCEPTIONS \
        ENV_STD::is_same_v<UNQUALIFIED_POLICY_TYPE, ENV::check_exceptions_s>

#define SHOULD_ADJUST_ON_EXCEPTION \
        ENV_STD::is_same_v<UNQUALIFIED_POLICY_TYPE, ENV::adjust_on_exception_s>

// state

#define WHEN_STATE_ELABORATE(_check, _exit, _throw, _return)                          \
        CMP_ON((SHOULD_EXIT_ON_EXCEPTION), ON(_check, SPREAD(_exit)););               \
        CMP_ON((SHOULD_THROW_ON_EXCEPTION), ON(_check, SPREAD(_throw)););             \
        CMP_ON((SHOULD_CHECK_EXCEPTIONS), ON(_check, SPREAD(_return)););              \
        CMP_ON((SHOULD_ADJUST_ON_EXCEPTION), ON(_check, SPREAD(_return));)

#define WHEN_STATE_SIMPLE(_check, _exception, _message, _return)                 \
        WHEN_STATE_ELABORATE(                                                    \
                _check,                                                          \
                (ENV::exit(_message);),                                          \
                (ENV::except<SPREAD(_exception)>(_message);),                    \
                _return)

#define WHEN_STATE(_check, _return)                     \
        WHEN_STATE_SIMPLE(                              \
                _check,                                 \
                (ENV::invalid_state_t),                 \
                ENV::exception_status_s::state_message, \
                _return)

// arg

#define WHEN_ARG_ELABORATE(_check, _exit, _throw, _return, _adjust)                   \
        CMP_ON((SHOULD_EXIT_ON_EXCEPTION), ON(_check, SPREAD(_exit)););               \
        CMP_ON((SHOULD_THROW_ON_EXCEPTION), ON(_check, SPREAD(_throw)););             \
        CMP_ON((SHOULD_CHECK_EXCEPTIONS), ON(_check, SPREAD(_return)););              \
        CMP_ON((SHOULD_ADJUST_ON_EXCEPTION), ON(_check, SPREAD(_adjust));)

#define WHEN_ARG_SIMPLE(_check, _exception, _message, _return, _adjust)     \
        WHEN_ARG_ELABORATE(                                                 \
                _check,                                                     \
                (ENV::exit(_message);),                                     \
                (ENV::except<SPREAD(_exception)>(_message);),               \
                _return,                                                    \
                _adjust)

#define WHEN_ARG(_check, _return, _adjust)                  \
        WHEN_ARG_SIMPLE(                                    \
                _check,                                     \
                (ENV::invalid_argument_t),                  \
                ENV::exception_status_s::argument_message,  \
                _return,                                    \
                _adjust)


ENV_TEST_BEGIN

struct uint_ptr_proxy_t
{
    DECL_THIS(uint_ptr_proxy_t);
    DEF((int * ), ptr, (nil));

    con uint_ptr_proxy_t(_ptr_t ptr) : _ptr{ptr} { }

    DEFAULT_LIFE(uint_ptr_proxy_t, CMP);

    CONST_GETTER_FML((), (int), op *, (_get()));

    CONST_GETTER_FML((), (int), get, (_get()));

    tmp<exc_name>
    callb inl multiply_positive(int to_add, exc_arg) noex_pol
    {
        WHEN_STATE((_get_ptr() == nullptr), (/* should ret */ ret;));
        WHEN_ARG((to_add < 0), (/* should ret: */ ret;),
                 (/* should adjust: */ to_add = 0;));

        _get() *= to_add;
    }

protected:
    CONST_GETTER_FML((), (int & ), _get, (*_get_ptr();));
};

ENV_TEST_END

ENV_TEST_CASE("exception policies")
{
    SUBCASE("invalid state")
    {
        test::uint_ptr_proxy_t proxy{ };

        SUBCASE("throw")
        {
            REQUIRE_FALSE(noex(proxy.multiply_positive(1, throw_ex)));
            REQUIRE_THROWS(proxy.multiply_positive(1, throw_ex));
        }

        SUBCASE("check")
        {
            REQUIRE(noex(proxy.multiply_positive(1, check_ex)));
            proxy.multiply_positive(1, check_ex);
        }

        SUBCASE("adjust")
        {
            REQUIRE(noex(proxy.multiply_positive(1, adjust_ex)));
            proxy.multiply_positive(1, adjust_ex);
        }
    }

    SUBCASE("invalid arguments")
    {
        int target{2};
        test::uint_ptr_proxy_t proxy{&target};

        SUBCASE("ignore")
        {
            REQUIRE(noex(proxy.multiply_positive(1, ignore_ex)));
            proxy.multiply_positive(-1, ignore_ex);
            REQUIRE_EQ(*proxy, -2);
        }

        SUBCASE("check")
        {
            REQUIRE(noex(proxy.multiply_positive(1, check_ex)));
            proxy.multiply_positive(-12, check_ex);

            // TODO: investigate how and why....
            REQUIRE_EQ(*proxy, 2);
        }

        SUBCASE("throw")
        {
            REQUIRE_FALSE(noex(proxy.multiply_positive(1, throw_ex)));
            REQUIRE_THROWS(proxy.multiply_positive(-10, throw_ex));
        }

        SUBCASE("adjust")
        {
            REQUIRE(noex(proxy.multiply_positive(1, adjust_ex)));
            proxy.multiply_positive(-2, adjust_ex);
            REQUIRE_EQ(*proxy, 0);
        }
    }
}


#endif // ENV_EXCEPTION_HPP
