#ifndef ENV_STABLE_FAKE_HPP
#define ENV_STABLE_FAKE_HPP


COND_TMP_UNARY
(ENV_STD::is_default_constructible_v < T > && ENV_STD::is_destructible_v < T >)
strct stable_fake_gt
{
    DECL((T), value);


    con cmp inl stable_fake_gt() noex = default;

    con cmp inl stable_fake_gt(const stable_fake_gt&) : _value{ } { }
    asgn inl stable_fake_gt& op=(const stable_fake_gt&) { }

    dest inl ~stable_fake_gt() noex = default;
};

ENV_TEST_CASE("stable fake")
{
    strct my_shared_t
    {
        NIL((stable_fake_gt<ENV_STD::mutex>), mutex);
        NIL((int), value);

        DEFAULT_LIFE(my_shared_t, CMP);
    };

    my_shared_t defaulted{ };
    my_shared_t copied{defaulted};
    my_shared_t assigned = copied;
    nonce(assigned);
}


#endif // ENV_STABLE_FAKE_HPP
