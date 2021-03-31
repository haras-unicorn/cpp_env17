#ifndef ENV_COMPARE_HPP
#define ENV_COMPARE_HPP


// std compare

tmp<name T> typ(compare_gt) = ENV_STD::less<T>;


// compare

tmp<name TLhs, name TRhs>
cmp_fn compare(
        const ENV::std_comparable_c<TLhs>& lhs,
        const ENV::std_comparable_c<TRhs>& rhs) noex
{
    ret lhs < rhs;
}

COND_TMP_BINARY
((ENV::are_comparable_g < TLhs, TRhs >) &&
 (!ENV::are_std_comparable_g < TLhs, TRhs >))
cmp_fn compare(const TLhs& lhs, const TRhs rhs) noex
{
    ret lhs.compare(rhs);
}

ENV_TEST_CASE("compare")
{
    REQUIRE(compare(1, 2));
    REQUIRE_FALSE(compare(2, 1));
}


// comparer

strct comparer_t
{
    tmp<name TLhs, name TRhs>
    cmp_fn op()(
            const comparable_with_c <TLhs, TRhs>& lhs,
            const TRhs& rhs) const noex
    {
        ret compare(lhs, rhs);
    }
}
inl cmp comparer{ };

ENV_TEST_CASE("comparer")
{
    REQUIRE(comparer(1, 2));
    REQUIRE_FALSE(comparer(2, 1));
}


#endif // ENV_COMPARE_HPP
