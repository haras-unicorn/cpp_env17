#ifndef ENV_CALLABLE_HPP
#define ENV_CALLABLE_HPP


ENV_DETAIL_BEGIN

// declaration

template<name TCallable, inheritor_name, name = req_t>
strct callable_gs
{
    typ(is_callable_t) = false_t;

    typ(is_callback_t) = false_t;

    typ(is_consumer_t) = false_t;

    typ(is_noex_t) = false_t;
    typ(is_member_t) = false_t;
};


// non overloaded base

template<name TReturn, inheritor_extract>
strct callable_gs<TReturn(), inheritor>
{
private:
    DECL_SELF(callable_gs);

public:
    typ(is_callable_t) = true_t;

    typ(return_t) = TReturn;
    typ(is_callback_t) = same_vt<TReturn, void>;

    typ(is_consumer_t) = false_t;
    typ(argument_tuple_t) = ENV_HANA::tuple<>;
    typ(argument_count_t) = value_gnt<ENV_STD::size_t, 0>;

    typ(function_t) = TReturn();
    typ(function_ptr_t) = function_t*;

    typ(is_noex_t) = false_t;
    typ(is_member_t) = false_t;

    template<name T>
    cmp_obj static bool is_supported_g{
            !_self_t::is_noex_t::value || !_self_t::is_member_t::value};
};

template<name TReturn, inheritor_extract>
strct callable_gs<TReturn() noex, inheritor> :
    public callable_gs<
            TReturn(),
            ENV::self_ggt<
                    callable_gs<TReturn() noex, inheritor>,
                    inheritor>>
{
    typ(is_noex_t) = true_t;
};

#define DEFINE_CALLABLE_STRUCTS(_call)        \
    template<name TReturn>                    \
    strct callable_gs<TReturn _call()> :      \
        public callable_gs<TReturn()>{};      \
    template<name TReturn>                    \
    strct callable_gs<TReturn _call() noex> : \
        public callable_gs<TReturn() noex> { }


// argument

template<name TReturn, name... TArguments, inheritor_extract>
strct callable_gs<TReturn(TArguments...), inheritor> :
    public callable_gs<
            TReturn(),
            ENV::self_ggt<
                    callable_gs<TReturn(TArguments...), inheritor>,
                    inheritor>>
{
    typ(is_consumer_t) = true_t;


    typ(argument_tuple_t) = ENV_HANA::tuple<TArguments...>;
    typ(argument_count_t) = value_gnt<ENV_STD::size_t, sizeof...(TArguments)>;

    template<ENV_STD::size_t Index>
    typ(argument_at_nt) = name ENV::var_vt<TArguments...>::tmp at_nt<Index>;


    typ(function_t) = TReturn(TArguments...);
    typ(function_ptr_t) = function_t*;
};

template<name TReturn, name... TArguments, inheritor_extract>
strct callable_gs<TReturn(TArguments...) noex, inheritor> :
    public callable_gs<
            TReturn(TArguments...),
            ENV::self_ggt<
                    callable_gs<TReturn(TArguments...) noex, inheritor>,
                    inheritor>>
{
    typ(is_noex_t) = true_t;
};

#define DEFINE_ARGUMENT_CALLABLE_STRUCTS(_call)            \
    template<name TReturn, name... TArguments>             \
    strct callable_gs<TReturn _call(TArguments...)> :      \
        public callable_gs<TReturn(TArguments...)>{};      \
                                                           \
    template<name TReturn, name... TArguments>             \
    strct callable_gs<TReturn _call(TArguments...) noex> : \
        public callable_gs<TReturn(TArguments...) noex> { }


    // ptr

#define DEFINE_PTR_CALLABLE_STRUCTS(_call)                     \
    template<name TReturn, name... TArguments>                 \
    strct callable_gs<TReturn _call (*)(TArguments...)> :      \
        public callable_gs<TReturn(TArguments...)>{};          \
                                                               \
    template<name TReturn, name... TArguments>                 \
    strct callable_gs<TReturn _call (*)(TArguments...) noex> : \
        public callable_gs<TReturn(TArguments...) noex> { }


// this are needed here

EXPR_CHECK_UNARY(is_non_overloaded_functor, &T::op());


// member ptr

template<name TMember, name THolder, inheritor_extract>
strct callable_gs<
        TMember THolder::*, inheritor,
        COND_TYPE(name callable_gs<TMember>::is_callable_t)> :
    public callable_gs<
            TMember,
            ENV::self_ggt<
                    callable_gs<TMember THolder::*, inheritor>,
                    inheritor>>
{
    typ(is_member_t) = true_t;

    typ(holder_t) = THolder;
    typ(qualified_t) = THolder;
};

#define DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, _qualifier)                     \
    template<name THolder, name TReturn, name... TArguments, inheritor_extract>            \
    strct callable_gs<                                                                     \
            TReturn _call (THolder::*)(TArguments...) _qualifier,                          \
            inheritor> :                                                                   \
        public callable_gs<                                                                \
                TReturn(TArguments...),                                                    \
                ENV::self_ggt<                                                             \
                        callable_gs<                                                       \
                                TReturn _call (THolder::*)(TArguments...) _qualifier,      \
                                inheritor>,                                                \
                        inheritor>>                                                        \
    {                                                                                      \
        typ(is_noex_t) = false_t;                                                          \
        typ(is_member_t) = true_t;                                                         \
                                                                                           \
        typ(holder_t) = THolder;                                                           \
        typ(qualified_t) = THolder _qualifier;                                             \
    };                                                                                     \
                                                                                           \
    template<name THolder, name TReturn, name... TArguments, inheritor_extract>            \
    strct callable_gs<                                                                     \
            TReturn _call (THolder::*)(TArguments...) _qualifier noex,                     \
            inheritor> :                                                                   \
        public callable_gs<                                                                \
                TReturn(TArguments...) noex,                                               \
                ENV::self_ggt<                                                             \
                        callable_gs<                                                       \
                                TReturn _call (THolder::*)(TArguments...) _qualifier noex, \
                                inheritor>,                                                \
                        inheritor>>                                                        \
    {                                                                                      \
        typ(is_noex_t) = true_t;                                                           \
        typ(is_member_t) = true_t;                                                         \
                                                                                           \
        typ(holder_t) = THolder;                                                           \
        typ(qualified_t) = THolder _qualifier;                                             \
    }

#define DEFINE_QUALIFIED_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call)     \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, &);               \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, &&);              \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, const);           \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, const&);          \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, const&&);         \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, volatile);        \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, volatile&);       \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, volatile&&);      \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, const volatile);  \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, const volatile&); \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, const volatile&&)


// functors

template<name TFunctor, inheritor_extract>
strct callable_gs<
        TFunctor, inheritor,
        COND_TYPE(
                is_call_qualified_for_gs<
                        TFunctor,
                        name callable_gs<
                                decl(&ENV::unqualified_gt<
                                        TFunctor>::op())>::qualified_t>)> :
    public callable_gs<
            decl(&ENV::unqualified_gt<TFunctor>::op()),
            ENV::self_ggt<
                    callable_gs<TFunctor, inheritor>,
                    inheritor>>
{
    typ(is_member_t) = false_t;
};


// overloaded functors

// NOTE: it is impossible to check if the holder is qualified for
// its templated call operator because
// we can't get to the signature of an overloaded function and, therefore,
// we can't get to the qualifiers of the function to check against the holder.

template<name TFunctor>
strct callable_gs<
        TFunctor, no_inheritor_s,
        COND_TYPE(
                con_vt<has_call_operator_gs<TFunctor>,
                       neg_vt<is_non_overloaded_functor_gs<TFunctor>>>)>
{
    EXPR_CLASS_CHECK_VARIADIC(
            is_callable,
            declval<TFunctor>()(declval<TVar>()...));

    typ(is_callable_t) = true_t;

    typ(is_member_t) = false_t;

    template<name T>
    typ(is_supported_t) = first_gvt<false_t, T>;
};


// call types

// TODO: other call types

// DEFINE_CALLABLE_STRUCTS(SKIP); // defined
// DEFINE_ARGUMENT_CALLABLE_STRUCTS(SKIP); // defined
DEFINE_PTR_CALLABLE_STRUCTS(SKIP);
DEFINE_QUALIFIED_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(SKIP);

#undef DEFINE_QUALIFIED_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS
#undef DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS

#undef DEFINE_PTR_CALLABLE_STRUCTS

#undef DEFINE_ARGUMENT_CALLABLE_STRUCTS

#undef DEFINE_CALLABLE_STRUCTS

ENV_DETAIL_END


// individual traits

COND_CHECK_UNARY(is_callable, name detail::callable_gs<T>::is_callable_t);

COND_CONCEPT(callable, is_callable_gs<C>);

tmp<name T> typ(callable_gs) = detail::callable_gs<callable_r<T>>;


COND_CHECK_UNARY(is_callback, name detail::callable_gs<T>::is_callback_t);

COND_CONCEPT(callback, is_callback_gs<C>);

tmp<name T> typ(return_gt) = name detail::callable_gs<T>::return_t;


COND_CHECK_UNARY(is_consumer, name detail::callable_gs<T>::is_consumer_t);

COND_CONCEPT(consumer, is_consumer_gs<C>);

template<name T>
typ(argument_tuple_gt) = name detail::callable_gs<T>::argument_tuple_t;

template<name T>
typ(argument_count_gt) = name detail::callable_gs<T>::argument_count_t;

template<name T, ENV_STD::size_t Index>
typ(argument_at_gnt) = name detail::callable_gs<T>::tmp argument_at_nt<Index>;


tmp<name T> typ(function_gt) = name detail::callable_gs<T>::function_t;

tmp<name T> typ(function_ptr_gt) = name detail::callable_gs<T>::function_ptr_t;


COND_CHECK_UNARY(is_callable_member, name detail::callable_gs<T>::is_member_t);

COND_CONCEPT(callable_member, is_callable_member_gs<C>);

template<name T>
typ(callable_holder_gt) = name detail::callable_gs<T>::holder_t;

template<name T>
typ(callable_qualified_gt) = name detail::callable_gs<T>::qualified_t;


COND_CHECK_UNARY(is_noex_callable, name detail::callable_gs<T>::is_noex_t);

COND_CONCEPT(noex_callable, is_noex_callable_gs<C>);


// Uhh, yeah, so for some reason MSVC has an internal error
// if you don't do it like this

ENV_DETAIL_BEGIN

template<name T>
typ(is_supported_callable_t) =
        bool_nt<detail::callable_gs<T>::tmp is_supported_g<void_t>>;

ENV_DETAIL_END

COND_CHECK_UNARY(is_supported_callable, name detail::is_supported_callable_t<T>);

COND_CONCEPT(supported_callable, is_supported_callable_gs<C>);


// tests

ENV_TEST_BEGIN

strct templated_callable_t
{
    const ENV_STD::function<void()>  lambda{[] {}};
    const ENV_STD::function<void()>* lambda_ptr{&lambda};

    template<name = req_t>
    callb op()() { }
};

ENV_TEST_END


ENV_TEST_CASE("callable traits")
{
    struct test_t
    {
        void overloaded() { }

        void overloaded(int) { }

        static void static_except() { }

        virtual int int_unqualified_except_int(int)
        {
            ret 1;
        }

        char char_unqualified_noex() noex
        {
            ret 2;
        }

        void const_except_double_float(double, float) const { }

        void lvalue_except_char(char) & { }

        void volatile_rvalue_noex() volatile&& noex { }

        float op()(char, int)
        {
            ret 20;
        }
    };

    SUBCASE("check")
    {
        REQUIRES(is_callable_g<void()>);
        REQUIRES(is_callable_g<void (*)()>);
        REQUIRES(is_callable_g<void (*)() noex>);

        REQUIRES(is_callable_g<test_t&>);
        REQUIRES_FALSE(is_callable_g<const test_t>);
        REQUIRES(is_callable_g<test::templated_callable_t>);
        REQUIRES(is_callable_g<ENV_STD::function<void()>>);
        REQUIRES(is_callable_g<decl(&ENV_STD::function<void()>::op())>);
        REQUIRES(is_callable_g<unqualified_gt<const ENV_STD::function<void()>>>);
        REQUIRES(is_callable_g<decl(&ENV_STD::function<void()>::op())>);
        REQUIRES(is_callable_g<const ENV_STD::function<void()>>);
        REQUIRES_FALSE(is_callable_g<const volatile ENV_STD::function<void()>>);

        REQUIRES(is_callable_g<decl(&test_t::op())>);
        REQUIRES(is_callable_g<decl(&test_t::int_unqualified_except_int)>);
        REQUIRES(is_callable_g<decl(&test::templated_callable_t::lambda)>);
        REQUIRES_FALSE(is_callable_g<
                       decl(&test::templated_callable_t::lambda_ptr)>);

        REQUIRES_FALSE(is_callable_g<int>);
        REQUIRES_FALSE(is_callable_g<void>);
    }
    SUBCASE("ret")
    {
        REQUIRES(is_callback_g<void()>);
        REQUIRES(is_callback_g<void(int)>);
        REQUIRE_EQT(return_gt<decl(&test_t::int_unqualified_except_int)>, int);
        REQUIRE_EQT(return_gt<decl(&test_t::lvalue_except_char)>, void);
        REQUIRE_EQT(return_gt<test_t>, float);
    }
    SUBCASE("arguments")
    {
        REQUIRES(is_consumer_g<void(int)>);
        REQUIRES(is_consumer_g<int(int)>);
        REQUIRES(argument_count_gt<
                         decl(&test_t::const_except_double_float)>::value ==
                 2);
        REQUIRES(argument_count_gt<
                         decl(&test_t::op())>::value ==
                 2);
        REQUIRE_EQT(argument_tuple_gt<decl(&test_t::int_unqualified_except_int)>,
                    ENV_HANA::tuple<int>);
        REQUIRE_EQT(argument_tuple_gt<decl(&test_t::const_except_double_float)>,
                    ENV_HANA::tuple<double, float>);
    }
    SUBCASE("function")
    {
        REQUIRE_EQT(function_gt<decl(&test_t::int_unqualified_except_int)>,
                    int(int));
        REQUIRE_EQT(function_ptr_gt<decl(&test_t::const_except_double_float)>,
                    void (*)(double, float));
        REQUIRE_EQT(function_ptr_gt<decl(&test_t::char_unqualified_noex)>,
                    char (*)());
    }
    SUBCASE("noex")
    {
        REQUIRES(is_noex_callable_g<decl(&test_t::char_unqualified_noex)>);
        REQUIRES(is_noex_callable_g<decl(&test_t::volatile_rvalue_noex)>);
        REQUIRES_FALSE(is_noex_callable_g<test_t>);
        REQUIRES_FALSE(is_noex_callable_g<decl(&test_t::op())>);
    }
    SUBCASE("member")
    {
        REQUIRES(is_callable_member_g<decl(&test_t::char_unqualified_noex)>);
        REQUIRES_FALSE(is_callable_member_g<decl(&test_t::static_except)>);
    }
    SUBCASE("supported")
    {
        REQUIRES_FALSE(is_supported_callable_g<
                       decl(&test_t::char_unqualified_noex)>);
        REQUIRES(is_supported_callable_g<
                 decl(&test_t::static_except)>);
    }
    SUBCASE("holder")
    {
        REQUIRE_EQT(
                callable_holder_gt<decl(&test_t::char_unqualified_noex)>,
                test_t);
    }
    SUBCASE("qualifiers")
    {
        REQUIRES(is_volatile_g<
                 callable_qualified_gt<decl(&test_t::volatile_rvalue_noex)>>);
        REQUIRES(is_rvalue_ref_g<
                 callable_qualified_gt<decl(&test_t::volatile_rvalue_noex)>>);
    }
    SUBCASE("templated")
    {
        REQUIRE_EQT(
                return_gt<decl(&test::templated_callable_t::op() < void >)>,
                void);
        ENV_STD::invoke(
                &test::templated_callable_t::op() < void >,
                test::templated_callable_t{});
    }
}


#endif // ENV_CALLABLE_HPP
