#ifndef ENV_PTR_HPP
#define ENV_PTR_HPP

// ranked

tmp<rank_t Rank, name TValue>
    strct ptr_ngs
{
    cmp_obj static rank_t rank{Rank};
    typ(value_t) = TValue;

    typ(ptr_t) = name ptr_ngs<rank_t{rank - 1}, value_t *>::ptr_t;
};

tmp<name TValue>
    strct ptr_ngs<rank_zero, TValue>
{
    cmp_obj static rank_t rank{0};
    typ(value_t) = TValue;

    typ(ptr_t) = value_t;
};

tmp<rank_t Rank, name TValue>
    typ(ptr_ncgs) = ptr_ngs<Rank, add_const_gt<TValue>>;

tmp<rank_t Rank, name TValue>
    typ(ptr_nmgs) = ptr_ngs<Rank, remove_const_gt<TValue>>;

tmp<rank_t Rank, name TValue>
    typ(ptr_ngt) = name ptr_ngs<Rank, TValue>::ptr_t;

tmp<rank_t Rank, name TValue>
    typ(ptr_ncgt) = ptr_ngt<Rank, add_const_gt<TValue>>;

tmp<rank_t Rank, name TValue>
    typ(ptr_nmgt) = ptr_ngt<Rank, remove_const_gt<TValue>>;

ENV_TEST_CASE("ranked ptr")
{
    obj int a = 1;
    obj ptr_ncgt<rank_one, int> b = &a;
    REQUIRE_EQ(*b, a);

    obj ptr_ncgt<rank_two, int> c = &b;
    REQUIRE_EQ(**c, a);
}

// not ranked

tmp<name TValue>
    typ(ptr_gt) = ptr_ngt<rank_one, TValue>;

tmp<name TValue>
    typ(ptr_cgt) = ptr_gt<add_const_gt<TValue>>;

tmp<name TValue>
    typ(ptr_mgt) = ptr_gt<remove_const_gt<TValue>>;

ENV_TEST_CASE("unranked ptr")
{
    obj int a = 1;
    obj ptr_cgt<int> b = &a;
    REQUIRE_EQ(*b, a);

    obj ptr_gt<decltype(b)> c = &b;
    REQUIRE_EQ(**c, a);
}

// hash

ENV_NAMESPACE_STD_BEGIN

// https://stackoverflow.com/questions/20953390/what-is-the-fastest-hash-function-for-pointers
tmp<name TValue>
    strct hash<TValue *> // NOLINT(bugprone-forward-declaration-namespace)
{
private:
    typ(value_t) = ENV_STD::remove_cv_t<TValue>;

    typ(subject_t) = const value_t *;

    cmp_fn static log2(size_t value) noex->size_t { ret value == 1 ? 1 : (1 + log2(value / 2)); }

    let_cmp static _shift = static_cast<size_t>(log2(1 + sizeof(value_t)));

public:
    cmp_fn op()(subject_t subject) const noex
    {
        ret static_cast<size_t>(reinterpret_cast<uintptr_t>(subject)) >> _shift;
    }
};

ENV_NAMESPACE_STD_END

ENV_TEST_CASE("ptr hash")
{
    let a = 1;
    obj ptr_cgt<int> b = &a;
    obj ptr_ncgt<rank_two, int> c = &b;

    REQUIRE_NE(ENV_STD::hash<ptr_cgt<int>>{}(b), ENV_STD::hash<ptr_ncgt<rank_two, int>>{}(c));
}

// ptr size/align

let_cmp ptr_size{sizeof(void *)};

let_cmp ptr_align{alignof(void *)};

ENV_TEST_CASE("ptr size")
{
    REQUIRES(is_pow2(ptr_size));
    REQUIRES(is_pow2(ptr_align));
}

#endif // ENV_PTR_HPP
