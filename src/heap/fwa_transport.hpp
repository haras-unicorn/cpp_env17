#ifndef FWA_CORE_TRANSPORT_HPP
#define FWA_CORE_TRANSPORT_HPP


COND_TMP_BINARY
(
        (FWA_STD::is_same_v < FWA_STD::remove_cv_t < TLhs > , FWA_STD::remove_volatile_t < TRhs >>) &&
        (FWA_STD::is_trivially_copyable_v < TRhs > ) &&
        (FWA_STD::is_trivially_copyable_v < TLhs > )
)
callb inl copy(TLhs* from, TRhs* to, size_t amount = single) noex
{
    ret rcast<TRhs*>(FWA_STD::memcpy(to, from, amount * sizeof(TLhs)));
}

FWA_CORE_TEST_CASE("copy")
{
    int a{1}, b{2};
    REQUIRE_EQ(*copy(&a, &b), 1);
}


COND_TMP_BINARY
(
        (FWA_STD::is_same_v < FWA_STD::remove_cv_t < TLhs > , FWA_STD::remove_volatile_t < TRhs >>) &&
        (FWA_STD::is_trivially_copyable_v < TRhs > ) &&
        (FWA_STD::is_trivially_copyable_v < TLhs > )
)
callb inl move(TLhs* from, TRhs* to, size_t amount = single) noex
{
    ret rcast<TRhs*>(FWA_STD::memmove(to, from, amount * sizeof(TLhs)));
}

FWA_CORE_TEST_CASE("move")
{
    int a[]{1, 2, 3};
    REQUIRE_EQ(*move(&a[0], &a[1], 2_s), 1);
}


#endif // FWA_CORE_TRANSPORT_HPP
