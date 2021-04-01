#ifndef ENV_ATOMIC_HPP
#define ENV_ATOMIC_HPP


ENV_DETAIL_BEGIN

template<name T>
strct atomic_gs
{
    typ(is_atomic_t) = false_t;
    typ(is_atomic_wrapper_t) = false_t;
    typ(is_lock_free_t) = false_t;
};

template<>
strct atomic_gs<ENV_STD::atomic_flag>
{
    typ(atomic_t) = ENV_STD::atomic_flag;
    typ(value_t) = bool;

    typ(is_atomic_t) = true_t;
    typ(is_atomic_wrapper_t) = false_t;
    typ(is_lock_free_t) = true_t;
};

template<name T>
strct atomic_gs<ENV_STD::atomic<T>>
{
    typ(atomic_t) = ENV_STD::atomic<T>;
    typ(value_t) = T;

    typ(is_atomic_t) = true_t;
    typ(is_atomic_wrapper_t) = true_t;
    typ(is_lock_free_t) = bool_nt<atomic_t::is_always_lock_free>;
};


tmp<name T> typ(atomic_value_gt) = name atomic_gs<T>::value_t;

ENV_DETAIL_END


COND_CHECK_UNARY(
        is_atomic,
        name detail::atomic_gs<T>::is_atomic_t);

COND_CONCEPT(atomic, is_atomic_gs<C>);


COND_CHECK_UNARY(
        is_atomic_wrapper,
        name detail::atomic_gs<T>::is_atomic_wrapper_t);

COND_CONCEPT(atomic_wrapper, is_atomic_wrapper_gs<C>);


COND_CHECK_UNARY(
        is_lock_free,
        name detail::atomic_gs<T>::is_lock_free_t);

COND_CONCEPT(lock_free, is_lock_free_gs<C>);


tmp<name T> typ(atomic_value_gt) = detail::atomic_value_gt<atomic_r<T>>;


ENV_TEST_CASE("atomic")
{
    REQUIRES(is_atomic_g<ENV_STD::atomic<int>>);
    REQUIRES(is_atomic_g<ENV_STD::atomic_flag>);
    REQUIRES_FALSE(is_atomic_g<int>);

    REQUIRES(is_atomic_wrapper_g<ENV_STD::atomic<int>>);
    REQUIRES_FALSE(is_atomic_wrapper_g<ENV_STD::atomic_flag>);

    REQUIRE_EQT(atomic_value_gt<ENV_STD::atomic<int>>, int);
    REQUIRE_EQT(atomic_value_gt<ENV_STD::atomic_flag>, bool);
}


#endif // ENV_ATOMIC_HPP
