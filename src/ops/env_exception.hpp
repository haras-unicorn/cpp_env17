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

// literals

WHOLE_L(error_code, err, int);

TEXT_L(message, msg, const char *, l_t);

TEXT_L(description, dsc, const char *, l_t);

// policies

ENV_NAMESPACE_DETAIL_BEGIN

strct exception_policy_s{};

ENV_NAMESPACE_DETAIL_END

strct ignore_exceptions_s : public detail::exception_policy_s{};

strct exit_on_exception_s : public detail::exception_policy_s{};

strct throw_on_exception_s : public detail::exception_policy_s{};

strct check_exceptions_s : public detail::exception_policy_s{};

strct adjust_on_exception_s : public detail::exception_policy_s{};

cmp_obj ignore_exceptions_s ignore_ex{};

cmp_obj throw_on_exception_s throw_ex{};

cmp_obj exit_on_exception_s exit_ex{};

cmp_obj check_exceptions_s check_ex{};

cmp_obj adjust_on_exception_s adjust_ex{};

// policy concepts

COND_CHECK_UNARY(is_exception_policy, (ENV_STD::is_base_of_v<detail::exception_policy_s, T>));

COND_CONCEPT(exception_policy, (is_exception_policy_g<C>));

COND_CHECK_UNARY(is_unignorable_policy, (is_exception_policy_g<T> && !ENV_STD::is_same_v<T, ignore_exceptions_s>));

COND_CONCEPT(unignorable_policy, (is_unignorable_policy_g<C>));

// default policies

typ(d_exception_policy_s) = ENV_STD::conditional_t<is_debug, throw_on_exception_s, ignore_exceptions_s>;

cmp_obj d_exception_policy_s d_exception_policy{};

typ(d_unignorable_policy_s) = ENV_STD::conditional_t<is_debug, throw_on_exception_s, adjust_on_exception_s>;

cmp_obj d_unignorable_policy_s d_unignorable_policy{};

// status

struct OBJECT_ATTRIBUTES exception_status_s
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
    REQUIRE_EQT(unqualified_gt<decltype(exception_status_s::state_message)>, message_t);
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
    CMP_ON((is_debug)__debugbreak(););

    if (message == exception_status_s::state_message)
        ENV_STD::exit(exception_status_s::state_code);
    if (message == exception_status_s::argument_message)
        ENV_STD::exit(exception_status_s::argument_code);

    ENV_STD::exit(exception_status_s::default_code);
}

ENV_CLANG_SUPPRESS_POP
ENV_CLANG_SUPPRESS_POP

COND_TMP_UNARY((ENV_STD::is_base_of_v<ENV_STD::exception, T>))
callb except(message_t message) { throw T{message.data()}; }

// TODO: tests for unignorable stuff

// args

#define EXCEPTION_POLICY_TYPE_NAME TExcPol

#define EXCEPTION_POLICY_TEMPLATE_ARGUMENT name EXCEPTION_POLICY_TYPE_NAME = ENV::d_exception_policy_s

#define UNIGNORABLE_POLICY_TEMPLATE_ARGUMENT name EXCEPTION_POLICY_TYPE_NAME = ENV::d_unignorable_policy_s

#define EXCEPTION_POLICY_ARGUMENT_NAME exc_pol

#define EXCEPTION_POLICY_RUNTIME_ARGUMENT                                                            \
    [[maybe_unused]] exception_policy_c<EXCEPTION_POLICY_TYPE_NAME> EXCEPTION_POLICY_ARGUMENT_NAME = \
        EXCEPTION_POLICY_TYPE_NAME {}

#define UNIGNORABLE_POLICY_RUNTIME_ARGUMENT                                                            \
    [[maybe_unused]] unignorable_policy_c<EXCEPTION_POLICY_TYPE_NAME> EXCEPTION_POLICY_ARGUMENT_NAME = \
        EXCEPTION_POLICY_TYPE_NAME {}

// shorthands

#define exc_name EXCEPTION_POLICY_TEMPLATE_ARGUMENT
#define exc_arg EXCEPTION_POLICY_RUNTIME_ARGUMENT
#define unignore UNIGNORABLE_POLICY_RUNTIME_ARGUMENT

// noex

#define IS_NO_THROW_EXCEPTION_POLICY !ENV_STD::is_same_v<EXCEPTION_POLICY_TYPE_NAME, ENV::throw_on_exception_s>
#define NOEX_IF_NO_THROW_EXCEPTION_POLICY noex(IS_NO_THROW_EXCEPTION_POLICY)

// shorthand

#define is_noex_pol IS_NO_THROW_EXCEPTION_POLICY
#define noex_pol NOEX_IF_NO_THROW_EXCEPTION_POLICY

// when

// state

#define SHOULD_IGNORE_EXCEPTION ENV_STD::is_same_v<EXCEPTION_POLICY_TYPE_NAME, ENV::ignore_exceptions_s>
#define SHOULD_EXIT_ON_EXCEPTION ENV_STD::is_same_v<EXCEPTION_POLICY_TYPE_NAME, ENV::exit_on_exception_s>
#define SHOULD_THROW_ON_EXCEPTION ENV_STD::is_same_v<EXCEPTION_POLICY_TYPE_NAME, ENV::throw_on_exception_s>
#define SHOULD_CHECK_EXCEPTIONS ENV_STD::is_same_v<EXCEPTION_POLICY_TYPE_NAME, ENV::check_exceptions_s>
#define SHOULD_ADJUST_ON_EXCEPTION ENV_STD::is_same_v<EXCEPTION_POLICY_TYPE_NAME, ENV::adjust_on_exception_s>

#define WHEN_STATE_ELABORATE(_state_check, _state_exit, _state_throw, _return) \
    CMP_ON((!SHOULD_IGNORE_EXCEPTION),                                         \
           ON(_state_check,                                                    \
              {                                                                \
                  CMP_ON((SHOULD_EXIT_ON_EXCEPTION), SPREAD(_state_exit););    \
                  CMP_ON((SHOULD_THROW_ON_EXCEPTION), SPREAD(_state_throw););  \
                  CMP_ON((SHOULD_CHECK_EXCEPTIONS), SPREAD(_return););         \
                  CMP_ON((SHOULD_ADJUST_ON_EXCEPTION), SPREAD(_return););      \
              });)

#define WHEN_STATE_SIMPLE(_state_check, _state_exception, _state_message, _return) \
    WHEN_STATE_ELABORATE(                                                          \
        _state_check,                                                              \
        (ENV::exit(_state_message)),                                               \
        (ENV::except<SPREAD(_state_exception)>(_state_message)),                   \
        _return)

#define WHEN_STATE(_state_check, _return)       \
    WHEN_STATE_SIMPLE(                          \
        _state_check,                           \
        (ENV::invalid_state_t),                 \
        ENV::exception_status_s::state_message, \
        _return)

// arg

#define WHEN_ARG_ELABORATE(_argument_check, _argument_exit, _argument_throw, _return, _adjust) \
    CMP_ON((!SHOULD_IGNORE_EXCEPTION),                                                         \
           ON(_argument_check,                                                                 \
              {                                                                                \
                  CMP_ON((SHOULD_EXIT_ON_EXCEPTION), SPREAD(_argument_exit););                 \
                  CMP_ON((SHOULD_THROW_ON_EXCEPTION), SPREAD(_argument_throw););               \
                  CMP_ON((SHOULD_CHECK_EXCEPTIONS), SPREAD(_return););                         \
                  CMP_ON((SHOULD_ADJUST_ON_EXCEPTION), SPREAD(_adjust););                      \
              });)

#define WHEN_ARG_SIMPLE(                                               \
    _argument_check, _argument_exception, _argument_message,           \
    _return, _adjust)                                                  \
    WHEN_ARG_ELABORATE(                                                \
        _argument_check,                                               \
        (ENV::exit(_argument_message)),                                \
        (ENV::except<SPREAD(_argument_exception)>(_argument_message)), \
        _return,                                                       \
        _adjust)

#define WHEN_ARG(_argument_check, _return, _adjust) \
    WHEN_ARG_SIMPLE(                                \
        _argument_check,                            \
        (ENV::invalid_argument_t),                  \
        ENV::exception_status_s::argument_message,  \
        _return,                                    \
        _adjust)

ENV_NAMESPACE_TEST_BEGIN

struct uint_ptr_proxy_t
{
    DECL_THIS(uint_ptr_proxy_t);
    DEF((int *), ptr, (nil));

    con uint_ptr_proxy_t(_ptr_t ptr) : _ptr{ptr} {}

    DEFAULT_LIFE(uint_ptr_proxy_t, CMP);

    CONST_GETTER_FML((), (auto), op *, (_get();));

    CONST_GETTER_FML((), (auto), get, (_get();));

    tmp<exc_name>
        callb inl multiply_positive(int to_add, exc_arg) noex_pol
    {
        WHEN_STATE((_get_ptr() == nullptr), (/* should ret */ ret;));
        WHEN_ARG((to_add < 0), (/* should ret: */ ret;), (/* should adjust: */ to_add = 0;));

        _get() *= to_add;
    }

protected:
    CONST_GETTER_FML((), (int &), _get, (*_get_ptr();));
};

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("exception policies")
{
    SUBCASE("invalid state")
    {
        test::uint_ptr_proxy_t proxy{};

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
