#ifndef ENV_MUTABLE_HPP
#define ENV_MUTABLE_HPP


tmp<name T>
strct mutable_gt
{
    typ(value_t) = T;

private:
    mutable value_t _value;

protected:
    cmp_fn _get_value() const noex -> value_t& { ret _value; }

public:
    cmp_fn get() const noex -> value_t& { ret _get_value(); }

    cmp_fn operator*() const noex -> value_t& { ret _get_value(); }

    cmp_fn operator->() const noex { ret ENV_STD::addressof(_get_value()); };


    EXPR_TMP
    ((name THead, name... TRest),
     value_t
             {
                 ENV_STD::forward<THead>(declvalr<THead>()),
                 ENV_STD::forward<TRest>(declvalr<TRest>())...
             })
    con cmp inl mutable_gt(THead&& head, TRest&& ... rest)
    noexpr(
            value_t
            {
                ENV_STD::forward<THead>(declvalr<THead>()),
                ENV_STD::forward<TRest>(declvalr<TRest>())...
            }):
            _value
                    {
                            ENV_STD::forward<THead>(head),
                            ENV_STD::forward<TRest>(rest)...
                    } { }

    DEFAULT_LIFE(mutable_gt, CMP);
};

ENV_TEST_CASE("mutable")
{
    REQUIRES(size_of_g<mutable_gt<int>> == size_of_g<int>);
    REQUIRES(ENV_STD::is_trivial_v < mutable_gt<int>>);
    REQUIRES(ENV_STD::is_standard_layout_v < mutable_gt<int>>);

    const mutable_gt<int> t{1};
    t.get() = 2;
}


#endif // ENV_MUTABLE_HPP
