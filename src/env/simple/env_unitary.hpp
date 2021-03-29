#ifndef ENV_UNITARY_HPP
#define ENV_UNITARY_HPP


tmp<name T>
strct unitary_gt
{
    typ(value_t) = T;

    DECL((value_t), value);
    GETTER(get, _get_value());
    GETTER(op *, _get_value());
    CMP_GETTER(op->, ENV_STD::addressof(_get_value()));


    EXPR_TMP
    ((name THead, name... TRest),
     value_t
             {
                 ENV_STD::forward<THead>(declvalr<THead>()),
                 ENV_STD::forward<TRest>(declvalr<TRest>())...
             })
    con cmp inl unitary_gt(THead&& head, TRest&& ... rest)
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

    DEFAULT_LIFE(unitary_gt, CMP);
};

ENV_TEST_CASE("unitary")
{
    REQUIRES(size_of_g<unitary_gt<int>> == size_of_g<int>);
    REQUIRES(ENV_STD::is_trivial_v < unitary_gt<int>>);
    REQUIRES(ENV_STD::is_standard_layout_v < unitary_gt<int>>);

    unitary_gt<int> i{1};
    REQUIRE(*i == 1);
    unitary_gt<unitary_gt<int>> ui{1};
    REQUIRE(ui->get() == 1);
}


#endif // ENV_UNITARY_HPP
