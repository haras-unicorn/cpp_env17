#ifndef ENV_ALIGNMENT_HPP
#define ENV_ALIGNMENT_HPP


// align_t

WHOLE_UL(align, algn, ENV_STD::size_t);

tmp<name T> cmp_obj align_t align_of_g{alignof(T)};


// max align

strct max_align_t
{
    alignas(next_pow2(align_of_g<ENV_STD::max_align_t>)) byte_t
            data[next_pow2(align_of_g<ENV_STD::max_align_t>)];
};

let_cmp max_align{align_of_g<max_align_t>};

ENV_TEST_CASE("alignment")
{
    REQUIRES(is_pow2(max_align));
}


// atomic alignment
// GCC doesn't have this on my machine
// stolen from: https://en.cppreference.com/w/cpp/thread/hardware_destructive_interference_size#ifdef __cpp_lib_hardware_interference_size

#ifdef __cpp_lib_hardware_interference_size

cmp_obj align_t atomic_align{ENV_STD::hardware_destructive_interference_size};

cmp_obj align_t pack_align{ENV_STD::hardware_constructive_interference_size};

#else

cmp_obj align_t atomic_align{2 * max_align};

cmp_obj align_t pack_align{2 * max_align};

#endif

ENV_TEST_CASE("atomic alignment")
{
    REQUIRES(is_pow2(atomic_align));
    REQUIRES(is_pow2(pack_align));
}


// aligned

// I'm actually pleased with the performance of this thing
// I didn't expect that it would be just marginally slower than (or equal to)
// using a type like int on -O3

tmp<size_t size = single, align_t align = max_align>
cls aligned_nnt
{
private:
    alignas(align) ENV_STD::byte val[size]{ENV_STD::byte{0}};


public:
    imp cmp inl aligned_nnt() noex = default;

    COND_TMP_UNARY
    (size >= size_of_g < T > && ENV_STD::is_trivially_copyable_v < T >)
    inl aligned_nnt(const T& other) noex
    {
        ENV_STD::memcpy(val, &other, size);
        *this = other;
    }

    COND_TMP_UNARY
    (size >= size_of_g < T > && ENV_STD::is_trivially_copyable_v < T >)
    callb inl op=(const T& other) noex
    {
        ENV_STD::memcpy(val, &other, size);
        return *this;
    }


    COND_TMP_UNARY
    ((size >= size_of_g < T > ) &&
     (ENV_STD::is_trivially_constructible_v < T > ) &&
     (ENV_STD::is_trivially_copyable_v < T > ))
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
        nonce(u44);
        obj const aligned_nnt<4_s, 8_algn> u48 = uint32_t{48};
        nonce(u48);
        obj const aligned_nnt<4_s, 2_algn> u42 = uint32_t{42};
        nonce(u42);
        obj const aligned_nnt<8_s, 4_algn> u84 = uint32_t{84};
        nonce(u84);
        obj const aligned_nnt<8_s, 8_algn> u88 = uint32_t{88};
        nonce(u88);
        obj const aligned_nnt<8_s, 2_algn> u82 = uint32_t{82};
        nonce(u82);

        REQUIRE(uint32_t{u44} == 44);
        REQUIRE(uint32_t{u48} == 48);
        REQUIRE(uint32_t{u42} == 42);
        REQUIRE(uint32_t{u84} == 84);
        REQUIRE(uint32_t{u88} == 88);
        REQUIRE(uint32_t{u82} == 82);
    }
}


ENV_BENCH_BEGIN


ENV_BENCH(aligned_storage_heap)
{
    ENV_BLOOP
    {
        state.PauseTiming();
        ENV_STD::vector<aligned_nnt < 4_s, 4_algn>>
        vec{ };
        vec.reserve(state.range(0));
        benchmark::DoNotOptimize(vec.data());
        state.ResumeTiming();

        ENV_STD::fill(vec.begin(), vec.end(), 1);
        ENV_STD::transform(vec.begin(), vec.end(), vec.begin(),
                           [](auto s) { return uint32_t{s} + 3; });
        let res = ENV_STD::all_of(vec.begin(), vec.end(),
                                  [](auto s) { ret uint32_t{s} == 4; });

        state.PauseTiming();
        benchmark::DoNotOptimize(&res);
        benchmark::ClobberMemory();
        state.ResumeTiming();
    }
}

ENV_TIME(aligned_storage_heap, ->Range(1<<20, 1<<30))


ENV_BENCH(aligned_int_heap)
{
    ENV_BLOOP
    {
        state.PauseTiming();
        ENV_STD::vector<int> vec{ };
        vec.reserve(state.range(0));
        benchmark::DoNotOptimize(vec.data());
        state.ResumeTiming();

        ENV_STD::fill(vec.begin(), vec.end(), 1);
        ENV_STD::transform(vec.begin(), vec.end(), vec.begin(),
                           [](auto i) { ret i + 3; });
        let res = ENV_STD::all_of(vec.begin(), vec.end(),
                                  [](auto i) { ret i == 4; });

        state.PauseTiming();
        benchmark::DoNotOptimize(&res);
        benchmark::ClobberMemory();
        state.ResumeTiming();
    }
}

ENV_TIME(aligned_int_heap, ->Range(1<<20, 1<<30))


ENV_BENCH_END


#endif // ENV_ALIGNMENT_HPP
