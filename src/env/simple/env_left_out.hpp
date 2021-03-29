#ifndef ENV_LEFT_OUT_HPP
#define ENV_LEFT_OUT_HPP


tmp<name T>
strct left_out_gt : public T
{
    DECL_BASE(left_out, (T));
    using _left_out_base_t::_left_out_base_t;

    con cmp inl left_out_gt() noex = default;

    con cmp inl left_out_gt(const left_out_gt&) : _left_out_base_t{ } { }
    con cmp inl left_out_gt(left_out_gt&&) : _left_out_base_t{ } { }
    asgn inl left_out_gt& op=(const left_out_gt&) { }
    asgn inl left_out_gt& op=(left_out_gt&&) { }

    dest inl ~left_out_gt() noex = default;
};

ENV_TEST_CASE("left out")
{
    strct my_shared_t
    {
        int value{ };
        left_out_gt<ENV_STD::mutex> mutex{ };

        DEFAULT_LIFE(my_shared_t, CMP);
    };
    REQUIRES(ENV_STD::is_standard_layout_v < my_shared_t >);

    my_shared_t defaulted{1};
    my_shared_t copied{defaulted};
    my_shared_t assigned = copied;
    REQUIRE_EQ(assigned.value, 1);
}


#endif // ENV_LEFT_OUT_HPP
