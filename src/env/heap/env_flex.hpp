#ifndef ENV_FLEX_HPP
#define ENV_FLEX_HPP


// TODO stuff

COND_TMP((name TIter), ENV::is_iterator_g < TIter >)
strct flex_gt
{
    DECL_THIS(flex_gt);


    typ(iter_t) = TIter;

    typ(range_t) = range_gt<iter_t>;
    typ(begin_t) = name range_t::begin_t;
    typ(end_t) = name range_t::end_t;

    typ(last_t) = iter_t;


    NIL((range_gt < iter_t > ), range);
    MEM_GETTER(range);
    GETTER(get_begin, _get_range().get_begin());
    CMP_GETTER(begin, get_begin());
    GETTER(get_end, _get_range().get_end());
    CMP_GETTER(end, get_end());

    NIL((iter_t), last);
    MEM_GETTER(last);
    CMP_GETTER(last, get_last());

    CMP_GETTER(size, _get_range().size());
    CMP_GETTER(capacity, ENV::clamp_cast<size_t>(ENV_STD::distance(get_begin(), get_last())));


    con cmp inl flex_gt(range_t range, last_t last) noex:
            _range{range}, _last{last} { }

    con cmp inl flex_gt(begin_t begin, end_t end, last_t last) noex:
            _range{begin, end}, _last{last} { }

    DEFAULT_LIFE(flex_gt, CMP);


    COMPAT(is_flex);
    CMP_VALIDITY { ret ENV::is_valid(_get_range()); }
    CMP_TMP_HASH { ret ENV::hash(_get_range(), _get_last()); }
    CMP_TMP_EQUALITY { ret _get_range() == rhs._get_range() && _get_last() == rhs._get_last(); }
    CMP_TMP_COMPARISON { ret _get_range() < rhs._get_range(); }
};

ENV_TEST_CASE("flex")
{
    SUBCASE("construct")
    {
        int a[]{1, 2, 3};
        range_gt<int*> r{&a[0], &a[1]};

        flex_gt<int*> f_r{r, &a[2]};
        nonce(f_r);
        flex_gt<int*> f{&a[0], &a[1], &a[2]};
        nonce(f);
    }
}


#endif // ENV_FLEX_HPP
