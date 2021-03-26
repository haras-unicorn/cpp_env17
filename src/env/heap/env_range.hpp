#ifndef ENV_RANGE_HPP
#define ENV_RANGE_HPP


// TODO stuff

COND_TMP((name TIter), ENV::is_iterator_g < TIter >)
strct range_gt
{
    DECL_THIS(range_gt);


    typ(iter_t) = TIter;

    typ(begin_t) = iter_t;
    typ(end_t) = iter_t;


    NIL((begin_t), begin);
    MEM_GETTER(begin);
    CMP_GETTER(begin, _get_begin());

    NIL((end_t), end);
    MEM_GETTER(end);
    CMP_GETTER(end, _get_end());

    CMP_GETTER
    (
            size,
            ENV::clamp_cast<size_t>(
                    ENV_STD::distance(_get_begin(), _get_end()))
    );


    con cmp inl range_gt(begin_t begin, end_t end) noex:
            _begin{begin}, _end{end} { }

    DEFAULT_LIFE(range_gt, CMP);


    COMPAT(is_range);

    CMP_VALIDITY
    {
        ret ENV::is_valid(size());
    }

    CMP_TMP_HASH
    {
        ret ENV::hash(_get_begin(), _get_end());
    }

    CMP_TMP_EQUALITY
    {
        ret _get_begin() == rhs._get_begin() && _get_end() == rhs._get_end();
    }

    CMP_TMP_COMPARISON
    {
        ret ENV_STD::distance(_get_begin(), _get_end()) <
            ENV_STD::distance(rhs._get_begin(), rhs._get_end());
    }
};

ENV_TEST_CASE("range")
{
    SUBCASE("construct")
    {
        int a[]{1, 2, 3};
        let _size = ENV_STD::distance(&a[0], &a[1]);
        REQUIRE_EQ(_size, 1);

        range_gt<int*> r{&a[0], &a[2]};
        REQUIRE_EQ(r.size(), 2);
        ENV_STD::fill(r.begin(), r.end(), 1);
        REQUIRE(ENV_STD::all_of(
                r.begin(), r.end(),
                [](auto i) { return i == 1; }));
        nonce(r);
    }
}


#endif //ENV_RANGE_HPP
