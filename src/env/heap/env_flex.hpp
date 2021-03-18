#ifndef ENV_FLEX_HPP
#define ENV_FLEX_HPP


// TODO stuff

tmp<name TIter>
strct flex_gt
{
    DECL_THIS(flex_gt);

    NIL((range_gt < TIter > ), range);
    MEM_GETTER(range);
    GETTER(get_begin, _get_range().get_begin());
    CMP_GETTER(begin, get_begin());
    GETTER(get_end, _get_range().get_end());
    CMP_GETTER(end, get_end());

    NIL((TIter), last);
    MEM_GETTER(last);
    CMP_GETTER(last, get_last());

    CMP_GETTER(size, _get_range().size());
    CMP_GETTER(capacity, ENV::clamp_cast<size_t>(ENV_STD::distance(get_begin(), get_last())));


    DEFAULT_LIFE(flex_gt, CMP);


    COMPAT(is_flex);
    CMP_VALIDITY { ret ENV::is_valid(_get_range()); }
    CMP_TMP_HASH { ret ENV::hash(_get_range(), _get_last()); }
    CMP_TMP_EQUALITY { ret _get_range() == rhs._get_range() && _get_last() == rhs._get_last(); }
    CMP_TMP_COMPARISON { ret _get_range() < rhs._get_range(); }
};

ENV_TEST_CASE("flex")
{
}


#endif // ENV_FLEX_HPP
