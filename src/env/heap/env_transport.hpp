#ifndef ENV_TRANSPORT_HPP
#define ENV_TRANSPORT_HPP


ENV_DETAIL_BEGIN

COND_CHECK_BINARY
(
        are_bulk_copy_eligible,
        (ENV_STD::is_trivially_copyable_v < unqualified_gt < subject_gt < TRhs>>>) &&
        (ENV_STD::is_same_v<unqualified_gt<subject_gt<TLhs>>, unqualified_gt<subject_gt<TRhs>>>)
        (ENV_STD::is_pointer_v<TLhs> && ENV_STD::is_pointer_v<TRhs>)
);

ENV_DETAIL_END


// TODO: fix formatting

// copy

COND_TMP((exc_name, name TFrom, name TTo), ENV::is_writeable_to_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl copy(iterator_c <TFrom> from_begin, TFrom from_end, iterator_c <TTo> to_begin, exc_arg)
noex(noex(*to_begin = copy(*from_begin)) && noex_pol)
{
    WHEN_ARG((from_begin == nil || from_end == nil || to_begin == nil), (ret;), (ret;));

    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (detail::are_bulk_copy_eligible_g < TFrom, TTo >) ENV_STD::memcpy(
                to_begin, from_begin,
                ENV_STD::distance(from_begin, from_end) * sizeof(value_t));
    else
        while (from_begin != from_end) *to_begin++ = copy(*from_begin++);
}

ENV_TEST_CASE("copy")
{
    const int a{1};
    int b{2};
    copy(&a, &a + 1, &b, ignore_ex);
    REQUIRE_EQ(b, 1);
}


// overlapping copy

COND_TMP((exc_name, name TFrom, name TTo), ENV::is_writeable_to_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl ocopy(iterator_c <TFrom> from_begin, TFrom from_end, iterator_c <TTo> to_begin, exc_arg)
noex(noex(*to_begin = copy(*from_begin)) && noex_pol)
{
    WHEN_ARG((from_begin == nil || from_end == nil || to_begin == nil), (ret;), (ret;));

    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (detail::are_bulk_copy_eligible_g < TFrom, TTo >) ENV_STD::memmove(
                to_begin, from_begin,
                ENV_STD::distance(from_begin, from_end) * sizeof(value_t));
    else
        while (from_begin != from_end) *to_begin++ = copy(*from_begin++);
}

ENV_TEST_CASE("overlapping copy")
{
    int a[]{1, 2, 3};
    ocopy(&a[0], &a[2], &a[1]);
    REQUIRE_EQ(a[1], 1);
}


// uninitialized copy

COND_TMP((exc_name, name TFrom, name TTo, name TAlloc), ENV::is_placeable_on_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl ucopy(iterator_c <TFrom> from_begin, TFrom from_end, iterator_c <TTo> to_begin, TAlloc& alloc, exc_arg)
noex(noex(ENV_STD::allocator_traits<TAlloc>::construct(alloc, to_begin, copy(*from_begin))) && noex_pol)
{
    WHEN_ARG((from_begin == nil || from_end == nil || to_begin == nil), (ret;), (ret;));

    while (from_begin != from_end) ENV_STD::allocator_traits<TAlloc>::construct(alloc, to_begin++, copy(*from_begin++));
}

ENV_TEST_CASE("uninitialized copy")
{
    typ(value_t) = int;
    typ(alloc_t) = ENV_STD::allocator<value_t>;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    let cmp amount = 10_s;

    alloc_t alloc{ };
    value_t source[amount]{ };
    let source_end = source + amount;
    let dest_begin = traits_t::allocate(alloc, amount);
    let dest_end = dest_begin + amount;

    ENV_STD::fill(source, source_end, 1);
    ucopy(source, source_end, dest_begin, alloc);

    REQUIRE(ENV_STD::all_of(dest_begin, dest_end, [](auto i) { return i == 1; }));

    ENV_STD::for_each(dest_begin, dest_end, [&alloc](auto& i) { traits_t::destroy(alloc, &i); });
    traits_t::deallocate(alloc, dest_begin, amount);
}


// move

COND_TMP((exc_name, name TFrom, name TTo), ENV::is_writeable_to_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl move(iterator_c <TFrom> from_begin, TFrom from_end, iterator_c <TTo> to_begin, exc_arg)
noex(noex(*to_begin = move(*from_begin)) && noex_pol)
{
    WHEN_ARG((from_begin == nil || from_end == nil || to_begin == nil), (ret;), (ret;));

    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (detail::are_bulk_copy_eligible_g < TFrom, TTo >) ENV_STD::memcpy(
                to_begin, from_begin,
                ENV_STD::distance(from_begin, from_end) * sizeof(value_t));
    else
        while (from_begin != from_end) *to_begin++ = move(*from_begin++);
}

ENV_TEST_CASE("move")
{
    const int a{1};
    int b{2};
    move(&a, &a + 1, &b, ignore_ex);
    REQUIRE_EQ(b, 1);
}


// overlapping move

COND_TMP((exc_name, name TFrom, name TTo), ENV::is_writeable_to_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl omove(iterator_c <TFrom> from_begin, TFrom from_end, iterator_c <TTo> to_begin, exc_arg)
noex(noex(*to_begin = move(*from_begin)) && noex_pol)
{
    WHEN_ARG((from_begin == nil || from_end == nil || to_begin == nil), (ret;), (ret;));

    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (detail::are_bulk_copy_eligible_g < TFrom, TTo >) ENV_STD::memmove(
                to_begin, from_begin,
                ENV_STD::distance(from_begin, from_end) * sizeof(value_t));
    else
        while (from_begin != from_end) *to_begin++ = move(*from_begin++);
}

ENV_TEST_CASE("overlapping move")
{
    int a[]{1, 2, 3};
    omove(&a[0], &a[2], &a[1]);
    REQUIRE_EQ(a[1], 1);
}


// uninitialized move

COND_TMP((exc_name, name TFrom, name TTo, name TAlloc), ENV::is_placeable_on_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl umove(iterator_c <TFrom> from_begin, TFrom from_end, iterator_c <TTo> to_begin, TAlloc& alloc, exc_arg)
noex(noex(ENV_STD::allocator_traits<TAlloc>::construct(alloc, to_begin, move(*from_begin))) && noex_pol)
{
    WHEN_ARG((from_begin == nil || from_end == nil || to_begin == nil), (ret;), (ret;));

    while (from_begin != from_end) ENV_STD::allocator_traits<TAlloc>::construct(alloc, to_begin++, move(*from_begin++));
}

ENV_TEST_CASE("uninitialized move")
{
    typ(value_t) = int;
    typ(alloc_t) = ENV_STD::allocator<value_t>;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    let cmp amount = 10_s;

    alloc_t alloc{ };
    value_t source[amount]{ };
    let source_end = source + amount;
    let dest_begin = traits_t::allocate(alloc, amount);
    let dest_end = dest_begin + amount;

    ENV_STD::fill(source, source_end, 1);
    umove(source, source_end, dest_begin, alloc);

    REQUIRE(ENV_STD::all_of(dest_begin, dest_end, [](auto i) { return i == 1; }));

    ENV_STD::for_each(dest_begin, dest_end, [&alloc](auto& i) { traits_t::destroy(alloc, &i); });
    traits_t::deallocate(alloc, dest_begin, amount);
}


// swap

EXPR_TMP
(
        (exc_name, name TLhs, name TRhs),
        ENV_STD::swap(declvall < subject_gt < TLhs >> (), declvall < subject_gt < TRhs >> ())
)
callb inl swap(iterator_c <TLhs> lhs_begin, TLhs lhs_end, iterator_c <TRhs> rhs_begin)
noex(noex(ENV_STD::swap(*lhs_begin, *rhs_begin)) && noex_pol)
{
    while (lhs_begin != lhs_end) ENV_STD::swap(*lhs_begin++, *rhs_begin++);
}

ENV_TEST_CASE("swap")
{
    int a{1};
    int b{2};
    swap(&a, &a + 1, &b);
    REQUIRE_EQ(b, 1);
}


#endif // ENV_TRANSPORT_HPP
