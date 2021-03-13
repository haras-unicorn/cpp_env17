#ifndef ENV_CALLABLE_HPP
#define ENV_CALLABLE_HPP

// TODO: other call types

FWA_NAMESPACE_DETAIL_BEGIN

// declaration

tmp<name TCallable, name TInheritor = no_inheritor_s, name = success_t>
    strct callable_gs
{
    cmp_obj static bool is_callable{false};

    cmp_obj static bool is_callback{false};

    cmp_obj static bool is_consumer{false};

    cmp_obj static bool is_noex{false};
    cmp_obj static bool is_member{false};
};

// non overloaded base

tmp<name TReturn, name TInheritor>
    strct callable_gs<TReturn(), TInheritor>
{
private:
    DECL_SELF(callable_gs);

public:
    cmp_obj static bool is_callable{true};

    typ(return_t) = TReturn;
    cmp_obj static bool is_callback{FWA_STD::is_same_v<TReturn, void>};

    cmp_obj static bool is_consumer{false};

    typ(function_t) = TReturn();
    typ(function_ptr_t) = function_t *;

    cmp_obj static bool is_noex{false};
    cmp_obj static bool is_member{false};

    tmp<name = success_t> cmp_obj static bool is_supported{!_self_t::is_noex && !_self_t::is_member};
};

tmp<name TReturn, name TInheritor>
    strct callable_gs<TReturn() noex, TInheritor> : public callable_gs<
                                                        TReturn(),
                                                        FWA_CORE::self_ggt<callable_gs<TReturn() noex, TInheritor>, TInheritor>>
{
    cmp_obj static bool is_noex{true};
};

#define DEFINE_CALLABLE_STRUCTS(_call)                                                      \
    tmp<name TReturn> strct callable_gs<TReturn _call()> : public callable_gs<TReturn()>{}; \
    tmp<name TReturn> strct callable_gs<TReturn _call() noex> : public callable_gs<TReturn() noex> {}

// argument

tmp<name TReturn, name TInheritor, name... TArguments>
    strct callable_gs<TReturn(TArguments...), TInheritor> : public callable_gs<
                                                                TReturn(),
                                                                FWA_CORE::self_ggt<callable_gs<TReturn(TArguments...), TInheritor>, TInheritor>>
{
    cmp_obj static bool is_consumer{true};

    typ(argument_tuple_t) = FWA_STD::tuple<TArguments...>;
    cmp_obj static FWA_STD::size_t argument_count = FWA_STD::tuple_size_v<argument_tuple_t>;
    tmp<FWA_STD::size_t ArgumentIndex> typ(argument_at_nt) = FWA_STD::tuple_element_t<ArgumentIndex, argument_tuple_t>;

    typ(function_t) = TReturn(TArguments...);
    typ(function_ptr_t) = function_t *;
};

tmp<name TReturn, name TInheritor, name... TArguments>
    strct callable_gs<TReturn(TArguments...) noex, TInheritor> : public callable_gs<
                                                                     TReturn(TArguments...),
                                                                     FWA_CORE::self_ggt<callable_gs<TReturn(TArguments...) noex, TInheritor>, TInheritor>>
{
    cmp_obj static bool is_noex{true};
};

#define DEFINE_ARGUMENT_CALLABLE_STRUCTS(_call)                                                                \
    tmp<name TReturn, name... TArguments>                                                                      \
        strct callable_gs<TReturn _call(TArguments...)> : public callable_gs<TReturn(TArguments...)>{};        \
                                                                                                               \
    tmp<name TReturn, name... TArguments>                                                                      \
        strct callable_gs<TReturn _call(TArguments...) noex> : public callable_gs<TReturn(TArguments...) noex> \
    {                                                                                                          \
    }

    // ptr

#define DEFINE_PTR_CALLABLE_STRUCTS(_call)                                                                         \
    tmp<name TReturn, name... TArguments>                                                                          \
        strct callable_gs<TReturn _call (*)(TArguments...)> : public callable_gs<TReturn(TArguments...)>{};        \
                                                                                                                   \
    tmp<name TReturn, name... TArguments>                                                                          \
        strct callable_gs<TReturn _call (*)(TArguments...) noex> : public callable_gs<TReturn(TArguments...) noex> \
    {                                                                                                              \
    }

// these are needed here

EXPR_CHECK_UNARY(is_non_overloaded_functor, &T::op());

// qualified

tmp<name TQ, name TInheritor>
    strct callable_gs<
        TQ, TInheritor,
        FWA_CORE::success_vt<
            COND_TYPE(
                FWA_CORE::is_qualified_g<TQ> &&
                    callable_gs<FWA_CORE::unqualified_gt<TQ>>::is_callable &&
                !has_call_operator_g<FWA_CORE::unqualified_gt<TQ>>)>> : public callable_gs<FWA_CORE::unqualified_gt<TQ>, FWA_CORE::self_ggt<callable_gs<TQ, TInheritor>, TInheritor>>{};

// member ptr

tmp<name TMember, name THolder, name TInheritor>
    strct callable_gs<
        TMember THolder::*, TInheritor,
        FWA_CORE::success_vt<COND_TYPE(callable_gs<TMember>::is_callable)>> : public callable_gs<TMember, FWA_CORE::self_ggt<callable_gs<TMember THolder::*, TInheritor>, TInheritor>>
{
    cmp_obj static bool is_member{true};

    typ(holder_t) = THolder;
    typ(qualified_t) = THolder;
};

#define DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, _qualifier)                                                                                                                    \
    tmp<name TInheritor, name THolder, name TReturn, name... TArguments>                                                                                                                  \
        strct callable_gs<TReturn _call (THolder::*)(TArguments...) SPREAD(_qualifier), TInheritor> : public callable_gs<                                                                 \
                                                                                                          TReturn (THolder::*)(TArguments...),                                            \
                                                                                                          FWA_CORE::self_ggt<                                                             \
                                                                                                              callable_gs<                                                                \
                                                                                                                  TReturn _call (THolder::*)(TArguments...) SPREAD(_qualifier),           \
                                                                                                                  TInheritor>,                                                            \
                                                                                                              TInheritor>>                                                                \
    {                                                                                                                                                                                     \
        cmp_obj static bool is_noex{false};                                                                                                                                               \
        cmp_obj static bool is_member{true};                                                                                                                                              \
                                                                                                                                                                                          \
        typ(holder_t) = THolder;                                                                                                                                                          \
        typ(qualified_t) = THolder SPREAD(_qualifier);                                                                                                                                    \
    };                                                                                                                                                                                    \
                                                                                                                                                                                          \
    tmp<name TInheritor, name THolder, name TReturn, name... TArguments>                                                                                                                  \
        strct callable_gs<TReturn _call (THolder::*)(TArguments...) SPREAD(_qualifier) noex, TInheritor> : public callable_gs<                                                            \
                                                                                                               TReturn (THolder::*)(TArguments...) noex,                                  \
                                                                                                               FWA_CORE::self_ggt<                                                        \
                                                                                                                   callable_gs<                                                           \
                                                                                                                       TReturn _call (THolder::*)(TArguments...) SPREAD(_qualifier) noex, \
                                                                                                                       TInheritor>,                                                       \
                                                                                                                   TInheritor>>                                                           \
    {                                                                                                                                                                                     \
        cmp_obj static bool is_noex{true};                                                                                                                                                \
        cmp_obj static bool is_member{true};                                                                                                                                              \
                                                                                                                                                                                          \
        typ(holder_t) = THolder;                                                                                                                                                          \
        typ(qualified_t) = THolder SPREAD(_qualifier);                                                                                                                                    \
    }

#define DEFINE_QUALIFIED_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call)        \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (&));                \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (&&));               \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (const));            \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (const &));          \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (const &&));         \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (volatile));         \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (volatile &));       \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (volatile &&));      \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (const volatile));   \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (const volatile &)); \
    DEFINE_MEMBER_FUNCTION_PTR_CALLABLE_STRUCTS(_call, (const volatile &&))

// functors

tmp<name TFunctor, name TInheritor>
    strct callable_gs<
        TFunctor, TInheritor,
        FWA_CORE::success_vt<
            decl(&FWA_CORE::unqualified_gt<TFunctor>::op()),
            COND_TYPE(FWA_CORE::is_call_qualified_for_g<
                      TFunctor,
                      name callable_gs<decl(&FWA_CORE::unqualified_gt<TFunctor>::op())>::qualified_t>)>> : public callable_gs<decl(&FWA_CORE::unqualified_gt<TFunctor>::op()),
                                                                                                                              FWA_CORE::self_ggt<callable_gs<TFunctor, TInheritor>, TInheritor>>
{
    cmp_obj static bool is_member{false};
};

// overloaded functors

// NOTE: it is impossible to check if the holder is qualified for its templated call operator because
// we can't get to the signature of an overloaded function and, therefore,
// we can't get to the qualifiers of the function to check against the holder.
// TODO: fix by adding a templates to members

tmp<name TFunctor>
    strct callable_gs<
        TFunctor, no_inheritor_s,
        FWA_CORE::success_vt<
            COND_TYPE(
                FWA_CORE::has_call_operator_g<TFunctor> &&
                !FWA_CORE::detail::is_non_overloaded_functor_g<TFunctor>)>>
{
    EXPR_CLASS_CHECK_VARIADIC(is_callable, declval<TFunctor>()(declval<TVar>()...));
    cmp_obj static bool is_callable{true};

    cmp_obj static bool is_member{false};

    tmp<name = success_t> cmp_obj static bool is_supported{false};
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

FWA_NAMESPACE_DETAIL_END

// individual traits

COND_CHECK_UNARY(is_callable, detail::callable_gs<T>::is_callable);

COND_CONCEPT(callable, (is_callable_g<C>));

tmp<name T> typ(callable_gs) = detail::callable_gs<callable_r<T>>;

COND_CHECK_UNARY(is_callback, detail::callable_gs<T>::is_callback);

COND_CONCEPT(callback, is_callback_g<C>);

tmp<name T> typ(return_gt) = name detail::callable_gs<T>::return_t;

COND_CHECK_UNARY(is_consumer, detail::callable_gs<T>::is_consumer);

COND_CONCEPT(consumer, is_consumer_g<C>);

tmp<name T> typ(argument_tuple_gt) = name detail::callable_gs<T>::argument_tuple_t;

tmp<name T> cmp_obj FWA_STD::size_t argument_count_g = detail::callable_gs<T>::argument_count;

tmp<name T, FWA_STD::size_t Index> typ(argument_at_gnt) = name detail::callable_gs<T>::tmp argument_at_nt<Index>;

tmp<name T> typ(function_gt) = name detail::callable_gs<T>::function_t;

tmp<name T> typ(function_ptr_gt) = name detail::callable_gs<T>::function_ptr_t;

COND_CHECK_UNARY(is_callable_member, detail::callable_gs<T>::is_member);

COND_CONCEPT(callable_member, (is_callable_member_g<C>));

tmp<name T> typ(callable_holder_gt) = name detail::callable_gs<T>::holder_t;

tmp<name T> typ(callable_qualified_gt) = name detail::callable_gs<T>::qualified_t;

COND_CHECK_UNARY(is_noex_callable, detail::callable_gs<T>::is_noex);

COND_CONCEPT(noex_callable, is_noex_callable_g<C>);

// Uhh, yeah, so for some reason MSVC has an internal error if you don't do it like this
// TODO: make small repro somehow

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name T> cmp_obj bool is_supported_callable_g{detail::callable_gs<T>::tmp is_supported<>};

FWA_NAMESPACE_DETAIL_END

COND_CHECK_UNARY(is_supported_callable, detail::is_supported_callable_g<T>);

COND_CONCEPT(supported_callable, detail::is_supported_callable_g<C>);

// tests

FWA_NAMESPACE_TEST_BEGIN

strct templated_callable_t
{
    [[maybe_unused]] const FWA_STD::function<void()> lambda{[] {}};
    [[maybe_unused]] const FWA_STD::function<void()> *lambda_ptr{&lambda};
    tmp<name = success_t> callb op()() {}
};

FWA_NAMESPACE_TEST_END

ENV_TEST_CASE("callable traits")
{
    strct test_t{
        [[maybe_unused]] void overloaded(){}

        [[maybe_unused]] void overloaded(int){}

        [[maybe_unused]] static void static_except(){}

        [[maybe_unused]] virtual int int_unqualified_except_int(int){ret 1;
}

[[maybe_unused]] char char_unqualified_noex() noex { ret 2; }

[[maybe_unused]] void const_except_double_float(double, float) const {}

[[maybe_unused]] void lvalue_except_char(char) & {}

[[maybe_unused]] void volatile_rvalue_noex() volatile &&noex {}

[[maybe_unused]] float op()(char, int) { ret 20; }
}
;

SUBCASE("check")
{
    REQUIRES(is_callable_g<void()>);
    REQUIRES(is_callable_g<void (*)()>);
    REQUIRES(is_callable_g<void (*)() noex>);

    REQUIRES(is_callable_g<test_t &>);           // call operator is not const qualified
    REQUIRES_FALSE(is_callable_g<const test_t>); // call operator is not const qualified
    REQUIRES(is_callable_g<test::templated_callable_t>);
    REQUIRES(is_callable_g<FWA_STD::function<void()>>);
    REQUIRES(is_callable_g<decl(&FWA_STD::function<void()>::op())>);
    REQUIRES(is_callable_g<unqualified_gt<const FWA_STD::function<void()>>>);
    REQUIRES(is_callable_g<decl(&unqualified_gt<const FWA_STD::function<void()>>::op())>);
    REQUIRES(is_callable_g<const FWA_STD::function<void()>>);                // call operator is const qualified
    REQUIRES_FALSE(is_callable_g<const volatile FWA_STD::function<void()>>); // call operator is const qualified

    REQUIRES(is_callable_g<decl(&test_t::op())>);
    REQUIRES(is_callable_g<decl(&test_t::int_unqualified_except_int)>);
    REQUIRE_EQT(
        unqualified_gt<member_type_gt<decl(&test::templated_callable_t::lambda)>>,
        FWA_STD::function<void()>);
    REQUIRES(is_callable_g<decl(&test::templated_callable_t::lambda)>);
    REQUIRES_FALSE(is_callable_g<decl(&test::templated_callable_t::lambda_ptr)>);

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
    REQUIRES(argument_count_g<decl(&test_t::const_except_double_float)> == 2);
    REQUIRE_EQT(argument_tuple_gt<decl(&test_t::int_unqualified_except_int)>, FWA_STD::tuple<int>);
    REQUIRE_EQT(argument_tuple_gt<decl(&test_t::const_except_double_float)>, FWA_STD::tuple<double, float>);
}
SUBCASE("function")
{
    REQUIRE_EQT(function_gt<decl(&test_t::int_unqualified_except_int)>, int(int));
    REQUIRE_EQT(function_ptr_gt<decl(&test_t::const_except_double_float)>, void (*)(double, float));
    REQUIRE_EQT(function_ptr_gt<decl(&test_t::char_unqualified_noex)>, char (*)());
}
SUBCASE("noex")
{
    REQUIRES(is_noex_callable_g<decl(&test_t::char_unqualified_noex)>);
    REQUIRES(is_noex_callable_g<decl(&test_t::volatile_rvalue_noex)>);
    REQUIRES_FALSE(is_noex_callable_g<test_t>);
}
SUBCASE("member")
{
    REQUIRES(is_callable_member_g<decl(&test_t::char_unqualified_noex)>);
    REQUIRES_FALSE(is_callable_member_g<decl(&test_t::static_except)>);
}
SUBCASE("supported")
{
    REQUIRES_FALSE(is_supported_callable_g<decl(&test_t::char_unqualified_noex)>);
    REQUIRES(is_supported_callable_g<decl(&test_t::static_except)>);
}
SUBCASE("holder")
{
    REQUIRE_EQT(callable_holder_gt<decl(&test_t::char_unqualified_noex)>, test_t);
}
SUBCASE("qualifiers")
{
    REQUIRES(is_volatile_g<callable_qualified_gt<decl(&test_t::volatile_rvalue_noex)>>);
    REQUIRES(is_rvalue_ref_g<callable_qualified_gt<decl(&test_t::volatile_rvalue_noex)>>);
}
SUBCASE("templated")
{
    REQUIRE_EQT(return_gt<decl(&test::templated_callable_t::op() < void >)>, void);
    FWA_STD::invoke(&test::templated_callable_t::op() < void >, test::templated_callable_t{});
}
}

#endif // ENV_CALLABLE_HPP
