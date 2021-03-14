#ifndef ENV_TRANSPORT_HPP
#define ENV_TRANSPORT_HPP


// copy

COND_TMP_BINARY
(
        (ENV_STD::is_same_v < ENV_STD::remove_cv_t < TLhs > , ENV_STD::remove_volatile_t < TRhs >>) &&
        (ENV_STD::is_trivially_copyable_v < TRhs > ) && (ENV_STD::is_trivially_copyable_v < TLhs > )
)
callb inl copy(TLhs* from, TRhs* to, size_t amount = single) noex
{
    ret rcast<TRhs*>(ENV_STD::memcpy(to, from, amount * sizeof(TLhs)));
}

ENV_TEST_CASE("copy")
{
    int a{1}, b{2};
    nonce(a);
    nonce(b);
    REQUIRE_EQ(*copy(&a, &b), 1);
}


// copy_overlapped

COND_TMP_BINARY
(
        (ENV_STD::is_same_v < ENV_STD::remove_cv_t < TLhs > , ENV_STD::remove_volatile_t < TRhs >>) &&
        (ENV_STD::is_trivially_copyable_v < TRhs > ) && (ENV_STD::is_trivially_copyable_v < TLhs > )
)
callb inl copy_overlapped(TLhs* from, TRhs* to, size_t amount = single) noex
{
    ret rcast<TRhs*>(ENV_STD::memmove(to, from, amount * sizeof(TLhs)));
}

ENV_TEST_CASE("move")
{
    int a[]{1, 2, 3};
    nonce(a);
    REQUIRE_EQ(*copy_overlapped(&a[0], &a[1], 2_s), 1);
}


#endif // ENV_TRANSPORT_HPP