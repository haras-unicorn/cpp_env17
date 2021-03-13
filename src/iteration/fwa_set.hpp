#ifndef FWA_CORE_SET_HPP
#define FWA_CORE_SET_HPP


FWA_NAMESPACE_DETAIL_BEGIN

tmp<name TElements> typ(set_gt) =
FWA_STD::unordered_set<
        TElements,
        FWA_CORE::hasher_t, FWA_STD::equal_to<TElements>,
        allocator_gt < TElements>>;

tmp<name TElements> typ(view_set_gt) = set_gt<ptr_gt < TElements>>;

tmp<name TElements> typ(poly_set_gt) = set_gt<poly_gt < TElements>>;

FWA_NAMESPACE_DETAIL_END


tmp<name TElements> typ(set_gt) = detail::set_gt<key_c < TElements>>;

tmp<name TElements> typ(view_set_gt) = detail::view_set_gt<key_c < TElements>>;

tmp<name TElements> typ(poly_set_gt) = detail::poly_set_gt<key_c < TElements>>;

tmp<name... TElements> typ(set_vt) = set_gt<common_vt < TElements...>>;


// sized - no copy because of uniqueness in sets

tmp<name TElements>
fun inl sset(size_t size = empty) -> set_gt<TElements>
{
    set_gt<TElements> result;
    result.reserve(next_pow2(size));
    return result;
}

FWA_CORE_TEST_CASE("sized set")
{
    let _empty = sset<int>();
    // some optimization?
    REQUIRE_EQ(_empty.bucket_count(), 8);

    let _single = sset<int>(9_s);
    REQUIRE_EQ(_single.bucket_count(), 16);
}


// set

COND_TMP((name TCommon = void_t, name... TElements), (sizeof...(TElements) != empty))
fun inl set(TElements&& ... elements) -> set_gt<detail::container_common_vt<TCommon, TElements...>>
{
    mut result = sset<detail::container_common_vt<TCommon, TElements...>>(size_t{sizeof...(TElements)});
    ((result.emplace(FWA_STD::forward<TElements>(elements))), ...);

    return result;
}

FWA_CORE_TEST_CASE("set")
{
    let _set = set(1, 2, 0.4, 2.0);
    REQUIRE_EQ(_set.size(), 3);
    REQUIRE_EQ(*_set.find(0.4), 0.4);
}


#endif // FWA_CORE_SET_HPP
