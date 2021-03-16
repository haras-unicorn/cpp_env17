#ifndef ENV_TRANSPORT_HPP
#define ENV_TRANSPORT_HPP


// copy

COND_TMP((name TFrom, name TTo), ENV::is_writeable_to_g < ENV::subject_gt < TFrom >, TTo >)
callb inl copy(TFrom from, TTo to, size_t amount = single) noex
{
    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (ENV_STD::is_trivially_copyable_v<value_t>)
    {
        ret rcast<value_t*>(ENV_STD::memcpy(to, from, amount * sizeof(value_t)));
    }
    else
    {
        mut from_iter = from;
        let from_end = from + amount;

        mut to_iter = to;

        while (from_iter != from_end) *to_iter++ = *from_iter++;

        ret to;
    }
}

ENV_TEST_CASE("copy")
{
    const int a{1};
    int b{2};
    nonce(a);
    nonce(b);
    REQUIRE_EQ(*copy(&a, &b), 1);
}


// overlapping copy

COND_TMP((name TFrom, name TTo), ENV::is_writeable_to_g < ENV::subject_gt < TFrom >, TTo >)
callb inl ocopy(TFrom from, TTo to, size_t amount = single) noex
{
    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    ret
            CMP_TERN
            (
                    (ENV_STD::is_trivially_copyable_v < value_t > ),
                    (rcast<value_t*>(ENV_STD::memmove(to, from, amount * sizeof(value_t)))),
                    (copy(from, to, amount))
            );
}

ENV_TEST_CASE("overlapping copy")
{
    int a[]{1, 2, 3};
    nonce(a);
    REQUIRE_EQ(*ocopy(&a[0], &a[1], 2_s), 1);
}


// uninitialized copy

#ifdef ENV_STANDARD_REQUIREMENTS

COND_TMP_TERNARY(ENV::is_writeable_to_g < ENV::subject_gt < T1 >, T2 > && is_std_allocator_g < unqualified_gt<T3> >)
#else

COND_TMP_TERNARY(ENV::is_writeable_to_g < ENV::subject_gt < T1 >, T2 >)
#endif

callb inl ucopy(T1 from, T2 to, T3& alloc, size_t amount = single)
noexpr(ENV_STD::allocator_traits<T3>::construct(alloc, to, *from))
{
    typ(value_t) = T2;

    typ(alloc_t) = T3;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>::rebind_alloc<value_t>;


    mut from_iter = from;
    let from_end = from + amount;

    mut to_iter = to;

    while (from_iter != from_end) traits_t::construct(to_iter++, *from_iter++);

    ret to;
}



// move

COND_TMP_BINARY
(
        (ENV_STD::is_same_v < ENV_STD::remove_cv_t < TLhs > , ENV_STD::remove_volatile_t < TRhs >>) &&
        (ENV_STD::is_move_assignable_v < TRhs > )
)
callb inl move(TLhs* from, TRhs* to, size_t amount = single) noex
{
    typ(value_t) = ENV_STD::remove_cv_t<TRhs>;

    if_cmp (ENV_STD::is_trivially_copyable_v<value_t>)
    {
        ret rcast<value_t*>(ENV_STD::memcpy(to, from, amount * sizeof(value_t)));
    }
    else
    {
        mut from_iter = from;
        let from_end = from + amount;

        mut to_iter = to;

        while (from_iter != from_end) *to_iter++ = ENV_STD::move(*from_iter++);

        ret to;
    }
}

ENV_TEST_CASE("move")
{
    const int a{1};
    int b{2};
    nonce(a);
    nonce(b);
    REQUIRE_EQ(*move(&a, &b), 1);
}


// overlapping move

COND_TMP_BINARY
(
        (ENV_STD::is_same_v < ENV_STD::remove_cv_t < TLhs > , ENV_STD::remove_volatile_t < TRhs >>) &&
        (ENV_STD::is_move_assignable_v < TRhs > )
)
callb inl omove(TLhs* from, TRhs* to, size_t amount = single) noex
{
    typ(value_t) = ENV_STD::remove_cv_t<TRhs>;

    ret
            CMP_TERN
            (
                    (ENV_STD::is_trivially_copyable_v < value_t > ),
                    (rcast<value_t*>(ENV_STD::memmove(to, from, amount * sizeof(value_t)))),
                    (move(from, to, amount))
            );
}

ENV_TEST_CASE("overlapping move")
{
    int a[]{1, 2, 3};
    nonce(a);
    REQUIRE_EQ(*omove(&a[0], &a[1], 2_s), 1);
}


// swap

COND_TMP_BINARY
(
        (ENV_STD::is_same_v < ENV_STD::remove_cv_t < TLhs > , ENV_STD::remove_volatile_t < TRhs >>) &&
        (ENV_STD::is_swappable_v < TRhs > )
)
callb inl swap(TLhs* from, TRhs* to, size_t amount = single) noex
{
    typ(value_t) = ENV_STD::remove_cv_t<TRhs>;

    mut from_iter = from;
    let from_end = from + amount;

    mut to_iter = to;

    while (from_iter != from_end) ENV_STD::swap(*to_iter++, *from_iter++);

    ret to;
}

ENV_TEST_CASE("move")
{
    const int a{1};
    int b{2};
    nonce(a);
    nonce(b);
    REQUIRE_EQ(*swap(&a, &b), 1);
}


// overlapping swap

COND_TMP_BINARY
(
        (ENV_STD::is_same_v < ENV_STD::remove_cv_t < TLhs > , ENV_STD::remove_volatile_t < TRhs >>) &&
        (ENV_STD::is_swappable_v < TLhs > )
)
callb inl oswap(TLhs* from, TRhs* to, size_t amount = single) noex
{
    typ(value_t) = ENV_STD::remove_cv_t<TRhs>;

    ret swap(from, to, amount);
}

ENV_TEST_CASE("overlapping swap")
{
    int a[]{1, 2, 3};
    nonce(a);
    REQUIRE_EQ(*oswap(&a[0], &a[1], 2_s), 1);
}


#endif // ENV_TRANSPORT_HPP
