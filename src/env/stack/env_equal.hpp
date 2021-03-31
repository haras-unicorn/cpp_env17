#ifndef ENV_EQUAL_HPP
#define ENV_EQUAL_HPP


// std equal

tmp<name T> typ(equal_gt) = ENV_STD::equal_to<T>;


// equal

tmp<name TLhs, name TRhs>
cmp_fn equal(
        const ENV::std_equatable_c<TLhs>& lhs,
        const ENV::std_equatable_c<TRhs>& rhs) noex
{
    ret lhs == rhs;
}

COND_TMP_BINARY
((ENV::are_equatable_g < TLhs, TRhs >) &&
 (!ENV::are_std_equatable_g < TLhs, TRhs >))
cmp_fn equal(const TLhs& lhs, const TRhs rhs) noex
{
    ret lhs.equal(rhs);
}

ENV_TEST_CASE("equal")
{
    REQUIRE(equal(1, 1));
    REQUIRE_FALSE(equal(1, 2));
}


// equater

strct equater_t
{
    tmp<name TLhs, name TRhs>
    cmp_fn op()(
            const equatable_with_c <TLhs, TRhs>& lhs,
            const TRhs& rhs) const noex
    {
        ret equal(lhs, rhs);
    }
}
inl cmp equater{ };

ENV_TEST_CASE("equater")
{
    REQUIRE(equater(1, 1));
    REQUIRE_FALSE(equater(1, 2));
}


#endif // ENV_EQUAL_HPP
