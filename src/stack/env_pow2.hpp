#ifndef ENV_POW2_HPP
#define ENV_POW2_HPP

// 0 is considered a power of 2 here because all of this is usually used for containers and
// sometimes you want an empty container.

#if FWA_CPP >= 17

COND_TMP((deduc Exp), (Exp >= 0 && Exp < 64))
cmp_obj uint64_t pow2{1 << Exp};

#else // FWA_CPP >= 17

COND_TMP((FWA_STD::size_t Exp), (Exp >= 0 && Exp < 64))
cmp_obj uint64_t pow2{1 << Exp};

#endif // FWA_CPP >= 17

ENV_TEST_CASE("make_pow2")
{
    REQUIRE_EQ(pow2<0>, 1);
    REQUIRE_EQ(pow2<1>, 2);
    REQUIRE_EQ(pow2<2>, 4);
    REQUIRE_EQ(pow2<3>, 8);
    REQUIRE_EQ(pow2<4>, 16);
    REQUIRE_EQ(pow2<5>, 32);
    REQUIRE_EQ(pow2<6>, 64);
    REQUIRE_EQ(pow2<7>, 128);
    REQUIRE_EQ(pow2<8>, 256);
    REQUIRE_EQ(pow2<9>, 512);
    REQUIRE_EQ(pow2<10>, 1024);
}

// is_pow2

tmp<name T>
    cmp_fn is_pow2(arithmetic_c<T> to_check) noex
{
    if_cmp(is_whole_g<T>) ret to_check >= 0 && !(to_check & (to_check - 1));
    else ret to_check >= 0 && !mod(to_check, 2);
}

ENV_TEST_CASE("is_pow2")
{
    REQUIRE(is_pow2(2));
    REQUIRE(is_pow2(0_n));
    REQUIRE_FALSE(is_pow2(3_s));

    REQUIRE(is_pow2(2_s));
    REQUIRE(is_pow2(0));
    REQUIRE_FALSE(is_pow2(3_w));

    REQUIRE_FALSE(is_pow2(-2_w));
    REQUIRE_FALSE(is_pow2(-3_w));

    REQUIRE(is_pow2(2.0));
    REQUIRE_FALSE(is_pow2(2.3));

    REQUIRE_FALSE(is_pow2(-2.0));
    REQUIRE_FALSE(is_pow2(-2.3));
}

// next_pow2

// from: bit twiddling hacks

// -- first because if you pass a power of two you will get the next one instead of the one you passed.
// >> and | the result so that it fills all bits on the right of the highest bit and
// then just ++ the result for the next highest bit - the next power of two.

FWA_NAMESPACE_DETAIL_BEGIN

cmp_fn next_pow2(uint8_t from) noex
{
    --from;
    from |= from >> 1;
    from |= from >> 2;
    from |= from >> 4;
    ++from;

    return from;
}

cmp_fn next_pow2(uint16_t from) noex
{
    --from;
    from |= from >> 1;
    from |= from >> 2;
    from |= from >> 4;
    from |= from >> 8;
    ++from;

    return from;
}

cmp_fn next_pow2(uint32_t from) noex
{
    --from;
    from |= from >> 1;
    from |= from >> 2;
    from |= from >> 4;
    from |= from >> 8;
    from |= from >> 16;
    ++from;

    return from;
}

cmp_fn next_pow2(uint64_t from) noex
{
    --from;
    from |= from >> 1;
    from |= from >> 2;
    from |= from >> 4;
    from |= from >> 8;
    from |= from >> 16;
    from |= from >> 32;
    ++from;

    return from;
}

FWA_NAMESPACE_DETAIL_END

tmp<name T>
    cmp_fn next_pow2(arithmetic_c<T> from) noex
{
    if_cmp(is_unsigned_g<T>) ret detail::next_pow2(from);
    else if_cmp(is_whole_g<T>) ret detail::next_pow2(scast<whole_nnt<false, sizeof(T)>>(ramp(from)));
    else ret detail::next_pow2(scast<uint64_t>(ramp(ceil(from))));
}

// I think up to 1024 is good enough.
// TODO - more signed tests

ENV_TEST_CASE("next_pow2")
{
    SUBCASE("unsigned")
    {
        SUBCASE("8 bit")
        {
            REQUIRE_EQ(next_pow2(1_u8), 1);
            REQUIRE_EQ(next_pow2(2_u8), 2);
            REQUIRE_EQ(next_pow2(3_u8), 4);
            REQUIRE_EQ(next_pow2(5_u8), 8);
            REQUIRE_EQ(next_pow2(9_u8), 16);
            REQUIRE_EQ(next_pow2(17_u8), 32);
            REQUIRE_EQ(next_pow2(33_u8), 64);
            REQUIRE_EQ(next_pow2(65_u8), 128);
        }
        SUBCASE("16 bit")
        {
            REQUIRE_EQ(next_pow2(1_u16), 1);
            REQUIRE_EQ(next_pow2(2_u16), 2);
            REQUIRE_EQ(next_pow2(3_u16), 4);
            REQUIRE_EQ(next_pow2(5_u16), 8);
            REQUIRE_EQ(next_pow2(9_u16), 16);
            REQUIRE_EQ(next_pow2(17_u16), 32);
            REQUIRE_EQ(next_pow2(33_u16), 64);
            REQUIRE_EQ(next_pow2(65_u16), 128);
            REQUIRE_EQ(next_pow2(129_u16), 256);
            REQUIRE_EQ(next_pow2(257_u16), 512);
            REQUIRE_EQ(next_pow2(513_u16), 1024);
        }
        SUBCASE("32 bit")
        {
            REQUIRE_EQ(next_pow2(1_u32), 1);
            REQUIRE_EQ(next_pow2(2_u32), 2);
            REQUIRE_EQ(next_pow2(3_u32), 4);
            REQUIRE_EQ(next_pow2(5_u32), 8);
            REQUIRE_EQ(next_pow2(9_u32), 16);
            REQUIRE_EQ(next_pow2(17_u32), 32);
            REQUIRE_EQ(next_pow2(33_u32), 64);
            REQUIRE_EQ(next_pow2(65_u32), 128);
            REQUIRE_EQ(next_pow2(129_u32), 256);
            REQUIRE_EQ(next_pow2(257_u32), 512);
            REQUIRE_EQ(next_pow2(513_u32), 1024);
        }
        SUBCASE("64 bit")
        {
            REQUIRE_EQ(next_pow2(1_u64), 1);
            REQUIRE_EQ(next_pow2(2_u64), 2);
            REQUIRE_EQ(next_pow2(3_u64), 4);
            REQUIRE_EQ(next_pow2(5_u64), 8);
            REQUIRE_EQ(next_pow2(9_u64), 16);
            REQUIRE_EQ(next_pow2(17_u64), 32);
            REQUIRE_EQ(next_pow2(33_u64), 64);
            REQUIRE_EQ(next_pow2(65_u64), 128);
            REQUIRE_EQ(next_pow2(129_u64), 256);
            REQUIRE_EQ(next_pow2(257_u64), 512);
            REQUIRE_EQ(next_pow2(513_u64), 1024);
        }
    }

    SUBCASE("signed")
    {
        REQUIRE_EQ(next_pow2(-3), 0);
        REQUIRE_EQ(next_pow2(3_w), 4);
        REQUIRE_EQ(next_pow2(3_s), 4);
    }

    SUBCASE("floating")
    {
        REQUIRE_EQ(next_pow2(-3.1), 0);
        REQUIRE_EQ(next_pow2(3.1), 4);
    }
}

// align_pow2 - ceiling

#if FWA_CPP >= 17

tmp<deduc Pow2, name T>
#else  // FWA_CPP >= 17

tmp<FWA_STD::size_t Pow2, name T>
#endif // FWA_CPP >= 17
    cmp_fn align_pow2(arithmetic_c<T> to_align) noex
{
    // it doesn't work in the template for some reason
    static_assert(is_pow2(Pow2));
    obj cmp uintmax_t _pow2 = Pow2;

    if_cmp(is_unsigned_g<T>)
    {
        let cmp low_mask = _pow2 - 1;
        let cmp high_mask = ~low_mask;

        return (to_align + low_mask) & high_mask;
    }
    else if_cmp(is_whole_g<T>) ret align_pow2<Pow2>(scast<whole_nnt<false, sizeof(T)>>(ramp(to_align)));
    else ret align_pow2<Pow2>(scast<uint64_t>(ramp(ceil(to_align))));
}

TEST_CASE("align_pow2")
{
    REQUIRE_EQ(align_pow2<2>(3_n), 4);
    REQUIRE_EQ(align_pow2<4>(3_s), 4);
    REQUIRE_EQ(align_pow2<4_s>(5_n), 8);

    REQUIRE_EQ(align_pow2<2>(3_w), 4);
    REQUIRE_EQ(align_pow2<4>(3_w), 4);
    REQUIRE_EQ(align_pow2<4>(-5_w), 0);

    REQUIRE_EQ(align_pow2<2>(3.1), 4);
    REQUIRE_EQ(align_pow2<4>(2.5), 4);
    REQUIRE_EQ(align_pow2<4>(-5.2), 0);
}

#endif // ENV_POW2_HPP
