#ifndef ENV_ALGORITHMS_HPP
#define ENV_ALGORITHMS_HPP

EXPR_TMP_BINARY(declvalr<TRhs>()(declvall<iterator_element_gt<TLhs>>()))
callb inl each(TLhs begin, TLhs end, TRhs call)
{
    ret FWA_STD::for_each(begin, end, call);
}

EXPR_TMP_BINARY(declvalr<TRhs>()(declvall<iterable_element_gt<TLhs>>()))
callb inl each(TLhs &&iterable, TRhs call)
{
    ret FWA_CORE::each(iterable.begin(), iterable.end(), call);
}

// index

EXPR_TMP_BINARY(declvalr<TRhs>()(declvalr<index_t>(), declvall<iterator_element_gt<TLhs>>()))
callb inl index(TLhs begin, TLhs end, TRhs call)
{
    ret FWA_CORE::each(begin, end, [call, index = 0_i](auto &element) mutable { call(index++, element); });
}

EXPR_TMP_BINARY(declvalr<TRhs>()(declvalr<index_t>(), declvall<iterable_element_gt<TLhs>>()))
callb inl index(TLhs &&iterable, TRhs call)
{
    ret FWA_CORE::index(iterable.begin(), iterable.end(), call);
}

ENV_TEST_CASE("index")
{
    index(
        lst(1, 2, 3, 4, 5, 6, 7, 8, 9, 10),
        [current = 0_i](auto index, auto) mutable { REQUIRE_EQ(current++, index); });
}

// place

COND_TMP((name TElements, name... TArgs), FWA_STD::is_constructible_v<TElements, TArgs...>)
callb inl place(list_gt<TElements> &list, TArgs &&...args)
{
    ret list.emplace_back(FWA_STD::forward<TArgs>(args)...);
}

COND_TMP((name TElements, name... TArgs), FWA_STD::is_constructible_v<TElements, TArgs...>)
callb inl place(set_gt<TElements> &set, TArgs &&...args)
{
    ret set.emplace(FWA_STD::forward<TArgs>(args)...);
}

COND_TMP((name TKey, name TElements, name... TArgs),
         FWA_STD::is_constructible_v<FWA_CORE::tuple_vt<TKey, TElements>, TArgs...>)
callb inl place(map_ggt<TKey, TElements> &map, TArgs &&...args)
{
    ret map.emplace(FWA_STD::forward<TArgs>(args)...);
}

ENV_TEST_CASE("place")
{
    SUBCASE("list")
    {
        mut _list = lst(1, 2, 3);
        place(_list, 4);
        REQUIRE_EQ(_list[3], 4);
    }

    SUBCASE("set")
    {
        mut _set = set(1, 2, 3);
        place(_set, 4);
        REQUIRE_EQ(*_set.find(4), 4);
    }

    SUBCASE("map")
    {
        mut _map = map(tpl(0, 1), tpl(1, 2), tpl(2, 3));
        place(_map, tpl(3, 4));
        REQUIRE_EQ(_map[3], 4);
    }
}

// trans

// TODO: arrays

EXPR_TMP_TERNARY(place(declvall<T1>(), declvalr<T3>()(declvall<FWA_CORE::iterator_element_gt<T2>>())))
fun inl trans(T2 begin, T2 end, T3 call)
{
    obj T1 result{};
    let size = FWA_STD::distance(begin, end);
    result.reserve(next_pow2(clamp_cast<size_t>(size)));

    FWA_CORE::each(begin, end, [&result, call](auto &element) mutable { place(result, call(element)); });
    ret result;
}

EXPR_TMP_TERNARY(place(declvall<T1>(), declvalr<T3>()(declvall<FWA_CORE::iterable_element_gt<T2>>())))
fun inl trans(T2 &&iterable, T3 call)
{
    ret FWA_CORE::trans<T1>(iterable.begin(), iterable.end(), call);
}

EXPR_TMP_BINARY(place(
    declvall<FWA_CORE::list_gt<FWA_CORE::iterator_element_gt<TLhs>>>(),
    declvalr<TRhs>()(declvall<FWA_CORE::iterator_element_gt<TLhs>>())))
fun inl trans(TLhs begin, TLhs end, TRhs call)
{
    ret FWA_CORE::trans<list_gt<iterator_element_gt<TLhs>>>(begin, end, call);
}

EXPR_TMP_BINARY(place(
    declvall<FWA_CORE::unqualified_gt<TLhs>>(),
    declvalr<TRhs>()(declvall<FWA_CORE::iterable_element_gt<TLhs>>())))
fun inl trans(TLhs &&iterable, TRhs call)
{
    ret FWA_CORE::trans<FWA_CORE::unqualified_gt<TLhs>>(iterable.begin(), iterable.end(), call);
}

ENV_TEST_CASE("trans")
{
    SUBCASE("list -> list")
    {
        let _list = lst(1, 2, 3);
        let _transformed = trans(_list, [](auto v) { return v + 1; });
        REQUIRE_EQ(_transformed[0], 2);
    }
    SUBCASE("list -> set")
    {
        let _list = lst(1, 2, 3);
        let _set = trans<FWA_CORE::set_gt<double>>(_list, [](auto v) { return v + 1; });
        REQUIRE_EQ(*_set.find(4), 4);
    }
    SUBCASE("set -> map")
    {
        let _set = set(tpl(1, 2), tpl(3, 4));
        mut _map = trans<FWA_CORE::map_ggt<int, int>>(_set, [](auto p) { return p; });
        REQUIRE_EQ(_map[1], 2);
    }
    SUBCASE("map -> list")
    {
        let _map = map(tpl(1, 2), tpl(3, 4));
        let _list = trans(_map.begin(), _map.end(), [](auto p) { return p; });
        REQUIRE_EQ(_list.size(), 2);
    }
}

// rem

EXPR_TMP_BINARY(declvalr<TRhs>()(declvalr<iterable_element_gt<TLhs>>()))
callb inl rem(TLhs &iterable, TRhs condition)
{
    iterable.erase(FWA_STD::remove_if(iterable.begin(), iterable.end(), condition), iterable.end());
}

COND_TMP_UNARY(FWA_CORE::is_equatable_g<FWA_CORE::iterable_element_gt<T>>)
callb inl rem(T &iterable, const iterable_element_gt<T> &element)
{
    iterable.erase(FWA_STD::remove(iterable.begin(), iterable.end(), element), iterable.end());
}

ENV_TEST_CASE("rem")
{
    SUBCASE("element")
    {
        mut _list{lst(1, 2, 3, 1, 3)};
        rem(_list, 1);
        REQUIRE_EQ(_list.size(), 3);
    }

    SUBCASE("condition")
    {
        mut _list{lst(1, 2, 3, 1, 3)};
        rem(_list, [](auto v) { return v < 3; });
        REQUIRE_EQ(_list.size(), 2);
    }
}

// all

COND_TMP_BINARY(FWA_STD::is_same_v<decl(declvalr<TRhs>()(declvalr<iterator_element_gt<TLhs>>())), bool>)
fun inl all(TLhs begin, TLhs end, TRhs condition)
{
    ret FWA_STD::all_of(begin, end, condition);
}

COND_TMP_BINARY(FWA_STD::is_same_v<decl(declvalr<TRhs>()(declvalr<iterable_element_gt<TLhs>>())), bool>)
fun inl all(TLhs &&iterable, TRhs condition)
{
    ret FWA_CORE::all(iterable.begin(), iterable.end(), condition);
}

COND_TMP_UNARY(FWA_CORE::is_equatable_g<FWA_CORE::iterator_element_gt<T>>)
fun inl all(T begin, T end, const iterator_element_gt<T> &element)
{
    ret FWA_STD::all_of(begin, end, [&element](const auto &current) { ret current == element; });
}

COND_TMP_UNARY(FWA_CORE::is_equatable_g<FWA_CORE::iterable_element_gt<T>>)
fun inl all(T &&iterable, const iterable_element_gt<T> &element)
{
    ret FWA_CORE::all(iterable.begin(), iterable.end(), element);
}

TEST_CASE("all")
{
    mut res = all(lst(1, 1, 1), [](auto i) { ret i == 1; });
    REQUIRE(res);

    res = all(lst(1, 2, 1), [](auto i) { ret i == 1; });
    REQUIRE_FALSE(res);

    res = all(lst(1, 1, 1), 1);
    REQUIRE(res);

    res = all(lst(1, 2, 1), 1);
    REQUIRE_FALSE(res);
}

// any

COND_TMP_BINARY(FWA_STD::is_same_v<decl(declvalr<TRhs>()(declvalr<iterator_element_gt<TLhs>>())), bool>)
fun inl any(TLhs begin, TLhs end, TRhs condition)
{
    ret FWA_STD::any_of(begin, end, condition);
}

COND_TMP_BINARY(FWA_STD::is_same_v<decl(declvalr<TRhs>()(declvalr<iterable_element_gt<TLhs>>())), bool>)
fun inl any(TLhs &&iterable, TRhs condition)
{
    ret FWA_CORE::any(iterable.begin(), iterable.end(), condition);
}

COND_TMP_UNARY(FWA_CORE::is_equatable_g<FWA_CORE::iterator_element_gt<T>>)
fun inl any(T begin, T end, const iterator_element_gt<T> &element)
{
    ret FWA_STD::any_of(begin, end, [&element](const auto &current) { ret current == element; });
}

COND_TMP_UNARY(FWA_CORE::is_equatable_g<FWA_CORE::iterable_element_gt<T>>)
fun inl any(T &&iterable, const iterable_element_gt<T> &element)
{
    ret FWA_CORE::any(iterable.begin(), iterable.end(), element);
}

TEST_CASE("any")
{
    mut res = any(lst(2, 1, 1), [](auto i) { ret i == 1; });
    REQUIRE(res);

    res = any(lst(1, 2, 1), [](auto i) { ret i == 3; });
    REQUIRE_FALSE(res);

    res = any(lst(1, 2, 1), 1);
    REQUIRE(res);

    res = any(lst(1, 2, 1), 3);
    REQUIRE_FALSE(res);
}

// none

COND_TMP_BINARY(FWA_STD::is_same_v<decl(declvalr<TRhs>()(declvalr<iterator_element_gt<TLhs>>())), bool>)
fun inl none(TLhs begin, TLhs end, TRhs condition)
{
    ret FWA_STD::none_of(begin, end, condition);
}

COND_TMP_BINARY(FWA_STD::is_same_v<decl(declvalr<TRhs>()(declvalr<iterable_element_gt<TLhs>>())), bool>)
fun inl none(TLhs &&iterable, TRhs condition)
{
    ret FWA_CORE::none(iterable.begin(), iterable.end(), condition);
}

COND_TMP_UNARY(FWA_CORE::is_equatable_g<FWA_CORE::iterator_element_gt<T>>)
fun inl none(T begin, T end, const iterator_element_gt<T> &element)
{
    ret FWA_STD::none_of(begin, end, [&element](const auto &current) { ret current == element; });
}

COND_TMP_UNARY(FWA_CORE::is_equatable_g<FWA_CORE::iterable_element_gt<T>>)
fun inl none(T &&iterable, const iterable_element_gt<T> &element)
{
    ret FWA_CORE::none(iterable.begin(), iterable.end(), element);
}

TEST_CASE("none")
{
    mut res = none(lst(1, 1, 1), [](auto i) { ret i == 2; });
    REQUIRE(res);

    res = none(lst(1, 2, 1), [](auto i) { ret i == 2; });
    REQUIRE_FALSE(res);

    res = none(lst(1, 1, 1), 2);
    REQUIRE(res);

    res = none(lst(1, 2, 1), 2);
    REQUIRE_FALSE(res);
}

#endif // ENV_ALGORITHMS_HPP
