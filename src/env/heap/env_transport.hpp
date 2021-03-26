#ifndef ENV_TRANSPORT_HPP
#define ENV_TRANSPORT_HPP


ENV_DETAIL_BEGIN

COND_CHECK_BINARY
(
        are_bulk_copy_eligible,
        (ENV_STD::is_trivially_copyable_v <
         unqualified_gt < subject_gt < TRhs>>>) &&
        (ENV_STD::is_same_v<
                unqualified_gt<subject_gt<TLhs>>,
                unqualified_gt<subject_gt<TRhs>>>)
        (ENV_STD::is_pointer_v<TLhs> && ENV_STD::is_pointer_v<TRhs>)
);

ENV_DETAIL_END


// copy

COND_TMP((exc_name, name TFrom, name TTo),
         ENV::is_placeable_on_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl copy(
        iterator_c <TFrom> from_begin, TFrom from_end,
        iterator_c <TTo> to_begin,
        exc_arg)
noex(noex(*to_begin = copy(*from_begin)) && is_noex_pol)
{
    WHEN_ARG
    (((from_begin == nil || from_end == nil || to_begin == nil) &&
      (from_begin != from_end)),
    /* should check: */ (ret;),
    /* should adjust: */ (ret;));

    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (detail::are_bulk_copy_eligible_g < TFrom, TTo >)
    {
        let size = ENV_STD::distance(from_begin, from_end);
        ENV_STD::memcpy(to_begin, from_begin, size * sizeof(value_t));
        ret to_begin + size;
    }
    else
    {
        while (from_begin != from_end) *to_begin++ = copy(*from_begin++);
        ret to_begin;
    }
}

ENV_TEST_CASE("copy")
{
    const int a{1};
    int b{2};
    copy(&a, &a + 1, &b, ignore_ex);
    REQUIRES_FALSE(noexcept(copy(&a, &a, &b, throw_ex)));
    REQUIRE_THROWS(copy(&a, scast<const int*>(nullptr), &b, throw_ex));
    REQUIRE_EQ(b, 1);
}


// overlapping copy

COND_TMP((exc_name, name TFrom, name TTo),
         ENV::is_placeable_on_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl ocopy(
        iterator_c <TFrom> from_begin, TFrom from_end,
        iterator_c <TTo> to_begin,
        exc_arg)
noex(noex(*to_begin = copy(*from_begin)) && is_noex_pol)
{
    WHEN_ARG
    (((from_begin == nil || from_end == nil || to_begin == nil) &&
      (from_begin != from_end)),
    /* should check: */ (ret;),
    /* should adjust: */ (ret;));

    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (detail::are_bulk_copy_eligible_g < TFrom, TTo >)
    {
        let size = ENV_STD::distance(from_begin, from_end);
        ENV_STD::memmove(to_begin, from_begin, size * sizeof(value_t));
        ret to_begin + size;
    }
    else
    {
        while (from_begin != from_end) *to_begin++ = copy(*from_begin++);
        ret to_begin;
    }
}

ENV_TEST_CASE("overlapping copy")
{
    int a[]{1, 2, 3};
    ocopy(&a[0], &a[2], &a[1]);
    REQUIRES_FALSE(noexcept(ocopy(a + 0, a + 2, a + 1, throw_ex)));
    REQUIRE_THROWS(ocopy(a + 0, scast<int*>(nullptr), a + 1, throw_ex));
    REQUIRE_EQ(a[1], 1);
}


// uninitialized copy

COND_TMP
((exc_name, name TAlloc, name TFrom, name TTo),
 ENV::is_placeable_on_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl ucopy(
        TAlloc& alloc,
        iterator_c <TFrom> from_begin, TFrom from_end,
        iterator_c <TTo> to_begin,
        exc_arg)
noex(noex(ENV_STD::allocator_traits<TAlloc>::construct(
        alloc, to_begin, copy(*from_begin))) && is_noex_pol)
{
    WHEN_ARG
    (((from_begin == nil || from_end == nil || to_begin == nil) &&
      (from_begin != from_end)),
    /* should check: */ (ret;),
    /* should adjust: */ (ret;));

    while (from_begin != from_end)
        ENV_STD::allocator_traits<TAlloc>::construct(
                alloc, to_begin++, copy(*from_begin++));
    ret to_begin;
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
    ucopy(alloc, source, source_end, dest_begin);
    REQUIRES_FALSE(
            noexcept(ucopy(
                    alloc,
                    source, source_end, dest_begin,
                    throw_ex)));
    REQUIRE_THROWS(
            ucopy(alloc,
                  source, scast<value_t*>(nullptr), dest_begin,
                  throw_ex));

    REQUIRE(ENV_STD::all_of(dest_begin, dest_end,
                            [](auto i) { return i == 1; }));

    ENV_STD::for_each(dest_begin, dest_end,
                      [&alloc](auto& i) { traits_t::destroy(alloc, &i); });
    traits_t::deallocate(alloc, dest_begin, amount);
}


// move

COND_TMP((exc_name, name TFrom, name TTo),
         ENV::is_writeable_to_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl move(
        iterator_c <TFrom> from_begin, TFrom from_end,
        iterator_c <TTo> to_begin,
        exc_arg)
noex(noex(*to_begin = move(*from_begin)) && is_noex_pol)
{
    WHEN_ARG
    (((from_begin == nil || from_end == nil || to_begin == nil) &&
      (from_begin != from_end)),
    /* should check: */ (ret;),
    /* should adjust: */ (ret;));

    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (detail::are_bulk_copy_eligible_g < TFrom, TTo >)
    {
        let size = ENV_STD::distance(from_begin, from_end);
        ENV_STD::memcpy(to_begin, from_begin, size * sizeof(value_t));
        ret to_begin + size;
    }
    else
    {
        while (from_begin != from_end) *to_begin++ = move(*from_begin++);
        ret to_begin;
    }
}

ENV_TEST_CASE("move")
{
    const int a{1};
    int b{2};
    move(&a, &a + 1, &b, ignore_ex);
    REQUIRES_FALSE(noexcept(move(&a, &a, &b, throw_ex)));
    REQUIRE_THROWS(move(&a, scast<const int*>(nullptr), &b, throw_ex));
    REQUIRE_EQ(b, 1);
}


// overlapping move

COND_TMP((exc_name, name TFrom, name TTo),
         ENV::is_writeable_to_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl omove(
        iterator_c <TFrom> from_begin, TFrom from_end,
        iterator_c <TTo> to_begin,
        exc_arg)
noex(noex(*to_begin = move(*from_begin)) && is_noex_pol)
{
    WHEN_ARG
    (((from_begin == nil || from_end == nil || to_begin == nil) &&
      (from_begin != from_end)),
    /* should check: */ (ret;),
    /* should adjust: */ (ret;));

    typ(value_t) = unqualified_gt <subject_gt<TTo>>;

    if_cmp (detail::are_bulk_copy_eligible_g < TFrom, TTo >)
    {
        let size = ENV_STD::distance(from_begin, from_end);
        ENV_STD::memmove(to_begin, from_begin, size * sizeof(value_t));
        ret to_begin + size;
    }
    else
    {
        while (from_begin != from_end) *to_begin++ = move(*from_begin++);
        ret to_begin;
    }
}

ENV_TEST_CASE("overlapping move")
{
    int a[]{1, 2, 3};
    omove(&a[0], &a[2], &a[1]);
    REQUIRES_FALSE(noexcept(omove(a + 0, a + 2, &a[1], throw_ex)));
    REQUIRE_THROWS(omove(a + 0, scast<int*>(nullptr), &a[1], throw_ex));
    REQUIRE_EQ(a[1], 1);
}


// uninitialized move

COND_TMP((exc_name, name TAlloc, name TFrom, name TTo),
         ENV::is_writeable_to_g<const ENV::subject_gt <TFrom>&, TTo>)
callb inl umove(
        TAlloc& alloc, iterator_c <TFrom> from_begin, TFrom from_end,
        iterator_c <TTo> to_begin, exc_arg)
noex(noex(ENV_STD::allocator_traits<TAlloc>::construct(
        alloc, to_begin, move(*from_begin))) &&
     is_noex_pol)
{
    WHEN_ARG
    (((from_begin == nil || from_end == nil || to_begin == nil) &&
      (from_begin != from_end)),
    /* should check: */ (ret;),
    /* should adjust: */ (ret;));

    while (from_begin != from_end)
        ENV_STD::allocator_traits<TAlloc>::construct(
                alloc, to_begin++, move(*from_begin++));
    ret to_begin;
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
    umove(alloc, source, source_end, dest_begin);
    REQUIRES_FALSE(noexcept(umove(alloc,
                                  source, source_end, dest_begin,
                                  throw_ex)));
    REQUIRE_THROWS(umove(alloc,
                         source, scast<value_t*>(nullptr), dest_begin,
                         throw_ex));

    REQUIRE(ENV_STD::all_of(dest_begin, dest_end,
                            [](auto i) { return i == 1; }));

    ENV_STD::for_each(dest_begin, dest_end,
                      [&alloc](auto& i) { traits_t::destroy(alloc, &i); });
    traits_t::deallocate(alloc, dest_begin, amount);
}


// swap

COND_TMP((exc_name, name TLhs, name TRhs),
         ENV::are_interchangeable_g < TLhs, TRhs >)
callb inl swap(
        iterator_c <TLhs> lhs_begin, TLhs lhs_end, iterator_c <TRhs> rhs_begin,
        exc_arg)
noex(noex(ENV_STD::swap(*lhs_begin, *rhs_begin)) && is_noex_pol)
{
    WHEN_ARG
    (((lhs_begin == nil || lhs_end == nil || rhs_begin == nil) &&
      (lhs_begin != lhs_end)),
    /* should check: */ (ret;),
    /* should adjust: */ (ret;));

    while (lhs_begin != lhs_end) ENV_STD::swap(*lhs_begin++, *rhs_begin++);
    ret rhs_begin;
}

ENV_TEST_CASE("swap")
{
    int a{1};
    int b{2};
    swap(&a, &a + 1, &b);
    REQUIRES_FALSE(noexcept(swap(&a, scast<int*>(nullptr), &b, throw_ex)));
    REQUIRE_THROWS(swap(&a, scast<int*>(nullptr), &b, throw_ex));
    REQUIRE_EQ(b, 1);
}


// emplace

COND_TMP((exc_name, name TAlloc, name TIter, name... TVar),
         ENV::is_emplaceable_g < TIter, ENV::v < TVar...>>)
callb inl
emplace(TAlloc& alloc, iterator_c <TIter> begin, TIter end, TVar&& ... args)
noex(noex(ENV_STD::allocator_traits<TAlloc>::construct(
        alloc, begin, ENV_STD::forward<TVar>(args)...)) &&
     is_noex_pol)
{
    WHEN_ARG
    (((begin == nil || end == nil) && (begin != end)),
    /* should check: */ (ret;),
    /* should adjust: */ (ret;));

    while (begin != end)
        ENV_STD::allocator_traits<TAlloc>::construct(
                alloc, begin++, ENV_STD::forward<TVar>(args)...);
    ret begin;
}

ENV_TEST_CASE("emplace")
{
    typ(value_t) = ENV_STD::pair<int, int>;
    typ(alloc_t) = ENV_STD::allocator<value_t>;
    typ(traits_t) = ENV_STD::allocator_traits<alloc_t>;

    let cmp amount = 10_s;

    alloc_t alloc{ };
    let begin = traits_t::allocate(alloc, amount);
    let end = begin + amount;

    emplace<ignore_ex_t>(alloc, begin, end, 1, 2);
    REQUIRES_FALSE(noexcept(emplace<throw_ex_t>(
            alloc, scast<value_t*>(nullptr), end, 1, 2)));
    REQUIRE_THROWS(emplace<throw_ex_t>(
            alloc, scast<value_t*>(nullptr), end, 1, 2));
    let check = ENV_STD::all_of(
            begin, end, [](auto p) { ret p == ENV_STD::pair{1, 2}; });
    REQUIRE(check);
}


#endif // ENV_TRANSPORT_HPP
