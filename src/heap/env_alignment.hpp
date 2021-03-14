#ifndef ENV_ALIGNMENT_HPP
#define ENV_ALIGNMENT_HPP


// align_t

WHOLE_UL(align, algn, ENV_STD::size_t);

tmp<name T> cmp_obj align_t align_of_g{alignof(T)};


// max align

strct max_align_t
{
    alignas(next_pow2(align_of_g<ENV_STD::max_align_t>)) byte_t data[next_pow2(align_of_g<ENV_STD::max_align_t>)];
};

let_cmp max_align{align_of_g<max_align_t>};

ENV_TEST_CASE("alignment")
{
    REQUIRES(is_pow2(max_align));
}


// aligned

tmp<size_t size = single, align_t align = max_align>
cls aligned_nnt
{
private:
    alignas(align) ENV_STD::byte val[size]{ENV_STD::byte{0}};


public:
    imp cmp inl aligned_nnt() noex = default;

    COND_TMP_UNARY(size >= size_of_g < T > && ENV_STD::is_trivially_copyable_v < T >)
    inl aligned_nnt(const T& other) noex
    {
        ENV_STD::memcpy(val, &other, size);
        *this = other;
    }

    COND_TMP_UNARY(size >= size_of_g < T > && ENV_STD::is_trivially_copyable_v < T >)
    callb inl op=(const T& other) noex
    {
        ENV_STD::memcpy(val, &other, size);
        return *this;
    }


    COND_TMP_UNARY
    (size >= size_of_g < T > && ENV_STD::is_trivially_constructible_v < T > && ENV_STD::is_trivially_copyable_v < T >)
    con inl op T() const noex
    {
        T res;
        ENV_STD::memcpy(&res, val, sizeof(T));
        return res;
    }
};

tmp<align_t Align = max_align>
typ(aligned_nt) = aligned_nnt<single, Align>;


ENV_TEST_CASE("aligned")
{
    SUBCASE("size")
    {
        REQUIRES(size_of_g < aligned_nnt<8_s, 8_algn>> == 8);
        REQUIRES(size_of_g < aligned_nnt<8_s, 16_algn>> == 16);
        REQUIRES(size_of_g < aligned_nnt<8_s, 4_algn>> == 8);
        REQUIRES(size_of_g < aligned_nnt<8_s, 8_algn>> == 8);
        REQUIRES(size_of_g < aligned_nnt<16_s, 8_algn>> == 16);
        REQUIRES(size_of_g < aligned_nnt<4_s, 8_algn>> == 8);
    }

    SUBCASE("align")
    {
        REQUIRES(align_of_g<aligned_nnt<8_s, 8_algn>> == 8);
        REQUIRES(align_of_g<aligned_nnt<16_s, 8_algn>> == 8);
        REQUIRES(align_of_g<aligned_nnt<4_s, 8_algn>> == 8);
        REQUIRES(align_of_g<aligned_nnt<8_s, 8_algn>> == 8);
        REQUIRES(align_of_g<aligned_nnt<8_s, 16_algn>> == 16);
        REQUIRES(align_of_g<aligned_nnt<8_s, 4_algn>> == 4);
    }

    SUBCASE("construction")
    {
        REQUIRES(is_qualified_for_g < uint32_t, aligned_nnt<4_s, 4_algn>>);
        REQUIRES(is_qualified_for_g < uint32_t, aligned_nnt<4_s, 8_algn>>);
        REQUIRES(is_qualified_for_g < uint32_t, aligned_nnt<4_s, 2_algn>>);
        REQUIRES(is_qualified_for_g < uint32_t, aligned_nnt<4_s, 4_algn>>);
        REQUIRES(is_qualified_for_g < uint32_t, aligned_nnt<8_s, 4_algn>>);
        REQUIRES(!is_qualified_for_g < uint32_t, aligned_nnt<2_s, 4_algn>>);

        obj const aligned_nnt<4_s, 4_algn> u44 = uint32_t{44};
        obj const aligned_nnt<4_s, 8_algn> u48 = uint32_t{48};
        obj const aligned_nnt<4_s, 2_algn> u42 = uint32_t{42};
        obj const aligned_nnt<8_s, 4_algn> u84 = uint32_t{84};
        obj const aligned_nnt<8_s, 8_algn> u88 = uint32_t{88};
        obj const aligned_nnt<8_s, 2_algn> u82 = uint32_t{82};

        REQUIRE(uint32_t{u44} == 44);
        REQUIRE(uint32_t{u48} == 48);
        REQUIRE(uint32_t{u42} == 42);
        REQUIRE(uint32_t{u84} == 84);
        REQUIRE(uint32_t{u88} == 88);
        REQUIRE(uint32_t{u82} == 82);
    }
}


ENV_NAMESPACE_BENCH_BEGIN

ENV_BENCH(aligned_storage_heap)
{
    ENV_BLOOP
    {
    }
}

ENV_TIME(aligned_storage_heap)

ENV_NAMESPACE_BENCH_END


#endif // ENV_ALIGNMENT_HPP
