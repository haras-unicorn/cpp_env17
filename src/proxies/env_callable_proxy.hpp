#ifndef ENV_CALLABLE_PROXY_HPP
#define ENV_CALLABLE_PROXY_HPP

// proxy

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name TFunction>
    cls callable_proxy_gt : private FWA_STD::function<TFunction>
{
    DECL_THIS(callable_proxy_gt);
    DECL_BASE(std_function, (FWA_STD::function<TFunction>));

    typ(function_t) = TFunction;
    typ(proxy_t) = callable_proxy_gt<function_t>;

    using _std_function_base_t::function;
    using _std_function_base_t::op = ;

    DEFAULT_LIFE(callable_proxy_gt, NOEX);

    DEF_AS_BASE(std_function);

    NOEX_VALIDITY { ret this->_as_std_function_base()->op bool(); };

    ELABORATE_ENABLE_IF_COMPAT(
        (
            (is_imp_convertible_tmp_g<T, callable_proxy_gt>)&&(FWA_STD::is_same_v<name T::function_t, function_t>)),
        (
            (is_imp_convertible_tmp_g<T, callable_proxy_gt>)&&(FWA_STD::is_same_v<name T::function_t, function_t>)),
        (FWA_CORE::first_gnv<bool, false, T>));

    DEF_TMP_ID;

    using _std_function_base_t::swap;
    using _std_function_base_t::op bool;

    using _std_function_base_t::op();

protected:
    // TODO: proper id operations by target pointer
    // I wanted to define the id operations by using the target, but
    // it is not possible to do it without RTTI by the ISO standard.
    // Maybe some day I write my own function type with type erasure like in the standard that
    // permits this kind of behaviour.

    /*
public:
    DEFINE_TEMPLATE_ID_OPERATIONS
    (
    (
            const auto target = _get_target();

            ret target != nil ? static_cast<id_t>(reinterpret_cast<FWA_STD::uintptr_t>(target)) : n_id;
    ),
    (
            const auto target = _get_target();

            ret target != nil ? static_cast<hash_t>(reinterpret_cast<FWA_STD::uintptr_t>(target)) : n_hash;
    ),
    (
            const auto target = _get_target();
            const auto rhs_target = rhs._get_target();

            ret (target == nil || rhs_target == nil) ? falsy : target == rhs_target;
    )
    );


protected:
    RETURN_ATTRIBUTES inl const void* _get_target() const noex
    { ret this->_Target(this->_Target_type()); }
*/
};

FWA_NAMESPACE_DETAIL_END

FWA_NAMESPACE_STD_BEGIN

tmp<name TValue>
    strct hash<FWA_CORE::detail::callable_proxy_gt<TValue>>{
        cmp_fn op()(const FWA_CORE::detail::callable_proxy_gt<TValue> &callable) const noex{ret callable.hash();
}
}
;

FWA_NAMESPACE_STD_END

COND_TMP_UNARY((is_supported_callable_g<T>))
typ(callable_proxy_gt) = detail::callable_proxy_gt<function_gt<T>>;

tmp<name T>
    fun inl proxy(callable_c<T> &&value) noex { ret callable_proxy_gt<T>{FWA_STD::forward<T>(value)}; }

ENV_TEST_CASE("callable proxy")
{
    callable_proxy_gt<int()> a{[]() { ret 1; }};

    strct test{static int f(){ret 2;
}
}
;
callable_proxy_gt<int()> b{&test::f};

SUBCASE("call")
{
    REQUIRE_EQ(a(), 1);
    REQUIRE_EQ(b(), 2);
    REQUIRE_EQ(proxy(
                   []() {
                       ret 1;
                   })(),
               1);
}

SUBCASE("hash")
{
    REQUIRES(is_hashable_g<decltype(a)>);

    // TODO: investigate why this sometimes fails and sometimes doesn't.
    // REQUIRE_NE(hash(a), hash(b));

    REQUIRE_EQ(hash(a), hasher(a));
}

SUBCASE("equality")
{
    REQUIRE_NE(a, b);
    REQUIRE_EQ(a, a);
}
}

// concept

ELABORATE_COND_CHECK(
    is_callable_proxy,
    (name TCallableProxy), (TCallableProxy),
    (name TCallable), (detail::callable_proxy_gt<TCallable>),
    (is_supported_callable_g<TCallable>));

COND_CONCEPT(callable_proxy, (is_callable_proxy_g<C>));

ENV_TEST_CASE("callable proxy concept")
{
    REQUIRES(is_callable_proxy_g<callable_proxy_gt<int()>>);
    REQUIRES_FALSE(is_callable_proxy_g<int>);
}

// callback

typ(callback_t) = callable_proxy_gt<void()>;

obj inl const callback_t n_callback{}, d_callback{[] {}}; // NOLINT(cert-err58-cpp)

ENV_TEST_CASE("callback")
{
    REQUIRE(n_callback.is_invalid());
    d_callback();
}

#endif // ENV_CALLABLE_PROXY_HPP
