#ifndef ENV_RANGE_HPP
#define ENV_RANGE_HPP


tmp<name TVal>
strct range_gt
{
    DECL_THIS(range_gt);


    NIL((TVal * ), begin);
    MEM_GETTER(begin);
    CMP_GETTER(begin, _get_begin());

    NIL((TVal * ), end);
    MEM_GETTER(end);
    CMP_GETTER(end, _get_end());

    CMP_GETTER(size, ENV_STD::distance(_get_begin(), _get_end()));


    DEFAULT_LIFE(range_gt, CMP);


    COMPAT(is_range);

    CMP_VALIDITY
    {
        ret _get_begin() != nil && _get_end() != nil;
    }

    CMP_TMP_HASH
    {
        ret hash(_get_begin(), _get_end());
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
}


#endif //ENV_RANGE_HPP
