#ifndef ENV_SPINTEX_HPP
#define ENV_SPINTEX_HPP


EXPR_TMP_UNARY(declvall<T>()(declvalr<index_t>()))
strct spintex_gt final
{
private:
    DECL_THIS(spintex_gt);

    NIL((T), backoff);
    atomic_flag_t _flag = ATOMIC_FLAG_INIT;


public:
    con cmp inl spintex_gt() noex = default;
    dest inl ~spintex_gt() noex = default;

    spintex_gt(const spintex_gt&) = delete;
    spintex_gt(spintex_gt&&) = delete;
    spintex_gt& op=(const spintex_gt&) = delete;
    spintex_gt& op=(const spintex_gt&) volatile = delete;
    spintex_gt& op=(spintex_gt&&) = delete;
    spintex_gt& op=(spintex_gt&&) volatile = delete;


    cmp_fn lock() noex
    {
        for (auto i = first ;
             _flag.test_and_set(ENV_STD::memory_order_acquire) ;
             i = index_t{i + 1})
            _get_backoff()(i);
    }

    cmp_fn unlock() noex
    {
        _flag.clear(ENV_STD::memory_order_release);
    }
};


#endif // ENV_SPINTEX_HPP
