#ifndef ENV_ALIGNMENT_HPP
#define ENV_ALIGNMENT_HPP


// align_t

WHOLE_UL(align, align, ENV_STD::size_t);

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

    COND_TMP_UNARY
    (
            size_of_g < T > == size && align_of_g<T> == align &&
            ENV_STD::is_trivially_copyable_v < T >
    )
    inl aligned_nnt(const T& other) noex
    {
        copy(&other, val, size);
        *this = other;
    }


    COND_TMP_UNARY
    (
            size_of_g < T > == size && align_of_g < T > == align &&
            ENV_STD::is_trivially_copyable_v < T >
    )
    callb inl op=(const T& other) noex
    {
        copy(&other, val, size);
        return *this;
    }


    COND_TMP_UNARY
    (
            size_of_g < T > == size && align_of_g < T > == align &&
            ENV_STD::is_trivially_constructible_v < T > && ENV_STD::is_trivially_copyable_v < T >
    )
    con inl op T() const noex
    {
        T res;
        copy(val, &res, size);
        return res;
    }
};

tmp<align_t Align = max_align>
typ(aligned_nt) = aligned_nnt<single, Align>;


#endif // ENV_ALIGNMENT_HPP
