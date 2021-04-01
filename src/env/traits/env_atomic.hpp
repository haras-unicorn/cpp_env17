#ifndef ENV_ATOMIC_HPP
#define ENV_ATOMIC_HPP


ENV_DETAIL_BEGIN

tmp<name T>
strct atomic_gs
{
    cmp_obj static bool is_atomic{false};
    cmp_obj static bool is_atomic_wrapper{false};
    cmp_obj static bool is_lock_free{false};
};

tmp<>
strct atomic_gs<ENV_STD::atomic_flag>
{
    typ(atomic_t) = ENV_STD::atomic_flag;
    typ(value_t) = bool;

    cmp_obj static bool is_atomic{true};
    cmp_obj static bool is_atomic_wrapper{false};
    cmp_obj static bool is_lock_free{true};
};

tmp<name T>
strct atomic_gs<ENV_STD::atomic<T>>
{
    typ(atomic_t) = ENV_STD::atomic<T>;
    typ(value_t) = T;

    cmp_obj static bool is_atomic{true};
    cmp_obj static bool is_atomic_wrapper{true};
    cmp_obj static bool is_lock_free{atomic_t::is_always_lock_free};
};


tmp<name T> typ(atomic_value_gt) = name atomic_gs<T>::value_t;

ENV_DETAIL_END


COND_CHECK_UNARY(is_atomic, detail::atomic_gs<T>::is_atomic);

COND_CONCEPT(atomic, is_atomic_g<C>);


COND_CHECK_UNARY(is_atomic_wrapper, detail::atomic_gs<T>::is_atomic_wrapper);

COND_CONCEPT(atomic_wrapper, is_atomic_wrapper_g<C>);


COND_CHECK_UNARY(is_lock_free, detail::atomic_gs<T>::is_lock_free);

COND_CONCEPT(lock_free, is_lock_free_g<C>);


tmp<name T> typ(atomic_value_gt) = detail::atomic_value_gt<atomic_r<T>>;


ENV_TEST_CASE("atomic")
{
    REQUIRES(is_atomic_g<ENV_STD::atomic < int>>);
    REQUIRES(is_atomic_g<ENV_STD::atomic_flag>);
    REQUIRES_FALSE(is_atomic_g<int>);

    REQUIRES(is_atomic_wrapper_g<ENV_STD::atomic < int>>);
    REQUIRES_FALSE(is_atomic_wrapper_g<ENV_STD::atomic_flag>);

    REQUIRE_EQT(atomic_value_gt<ENV_STD::atomic < int>>, int);
    REQUIRE_EQT(atomic_value_gt<ENV_STD::atomic_flag>, bool);
}


#endif // ENV_ATOMIC_HPP
