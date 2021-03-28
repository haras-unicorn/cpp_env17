#ifndef ENV_UNITARY_HPP
#define ENV_UNITARY_HPP


COND_TMP_UNARY(ENV::is_stable_g < T >)
strct unitary_gt
{
    typ(value_t) = T;

    value_t value;
    GETTER(get, value);
    GETTER(op *, value);
    CMP_GETTER(op->, ENV_STD::addressof(value));

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
