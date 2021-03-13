#ifndef ENV_MAP_HPP
#define ENV_MAP_HPP

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name TKey, name TElements> typ(map_ggt) =
    FWA_STD::unordered_map<
        TKey, TElements,
        FWA_CORE::hasher_t, FWA_STD::equal_to<TKey>,
        allocator_gt<FWA_STD::pair<const TKey, TElements>>>;

tmp<name TKey, name TElements> typ(view_map_ggt) = map_ggt<TKey, ptr_gt<TElements>>;

tmp<name TKey, name TElements> typ(poly_map_ggt) = map_ggt<TKey, poly_gt<TElements>>;

FWA_NAMESPACE_DETAIL_END

tmp<name TKey, name TElements> typ(map_ggt) = detail::map_ggt<key_c<TKey>, TElements>;

tmp<name TKey, name TElements> typ(view_map_ggt) = detail::view_map_ggt<key_c<TKey>, TElements>;

tmp<name TKey, name TElements> typ(poly_map_ggt) = detail::poly_map_ggt<key_c<TKey>, TElements>;

tmp<name... T> typ(map_vt) = map_ggt<common_vt<typename T::first_type...>, common_vt<typename T::second_type...>>;

// sized

tmp<name TKey, name TElements>
    fun inl smap(size_t size = empty)->map_ggt<TKey, TElements>
{
    map_ggt<TKey, TElements> result;
    result.reserve(next_pow2(size));
    return result;
}

ENV_TEST_CASE("sized map")
{
    let _empty = smap<int, int>();
    // some optimization?
    REQUIRE_EQ(_empty.bucket_count(), 8);

    let _single = smap<int, int>(9_s);
    REQUIRE_EQ(_single.bucket_count(), 16);
}

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name TKeyCommon = void_t, name TElementsCommon = void_t, name... TPairs>
    typ(map_common_vt) =
        map_ggt<
            detail::container_common_vt<TKeyCommon, name unqualified_gt<TPairs>::first_type...>,
            detail::container_common_vt<TElementsCommon, name unqualified_gt<TPairs>::second_type...>>;

FWA_NAMESPACE_DETAIL_END

// map

COND_TMP((name TKeyCommon = void_t, name TElementsCommon = void_t, name... TPairs), (sizeof...(TPairs) != empty))
fun inl map(TPairs &&...pairs)->detail::map_common_vt<TKeyCommon, TElementsCommon, TPairs...>
{
    detail::map_common_vt<TKeyCommon, TElementsCommon, TPairs...> result;
    result.reserve(next_pow2(sizeof...(pairs)));
    ((result.emplace(FWA_STD::forward<TPairs>(pairs))), ...);

    return result;
}

ENV_TEST_CASE("map")
{
    SUBCASE("integral")
    {
        let a{0};
        mut result{map(tpl(0, 1), tpl(1, a), tpl(2, 3.4))};

        REQUIRE_EQ(result[0], 1);
        REQUIRE_EQ(result[1], 0);
        REQUIRE_EQ(result[2], 3.4);
        REQUIRE_EQ(result.size(), 3);
    }

    SUBCASE("struct")
    {
        strct test_t
        {
            DECL((const char *), label);
            MEM_GETTER(label);

            AUTO_LIFE(test_t, CMP,
                      (
                          : _label{"default"}),
                      (
                          : _label{"copied"}),
                      (
                          : _label{"moved"}));
        }
        to_copy{}, to_move{};

        let to_copy_tpl{tpl(0, to_copy)};
        mut result{
            map(
                to_copy_tpl,
                tpl(1, FWA_STD::move(to_move)),
                tpl(2, test_t{}))};
        REQUIRE_EQ(result[0].get_label(), "copied");
        REQUIRE_EQ(result[1].get_label(), "moved");
        REQUIRE_EQ(result[2].get_label(), "moved");
        REQUIRE_EQ(result.bucket_count(), 8);
    }

    SUBCASE("base")
    {
        strct base_t
        {
            DECL((const char *), label);
            MEM_GETTER(label);

            AUTO_LIFE(base_t, CMP,
                      (
                          : _label{"default"}),
                      (
                          : _label{"copied"}),
                      (
                          : _label{"moved"}));
        }
        base_to_copy{}, base_to_move{};

        strct derived_t : public base_t
        {
            using base_t::base_t;
            using base_t::operator=;
        }
        derived_to_copy{}, derived_to_move{};

        strct other_derived_t : public base_t
        {
            using base_t::base_t;
            using base_t::operator=;
        }
        other_derived_to_copy{}, other_derived_to_move{};

        let derived_to_copy_tpl{tpl(0, derived_to_copy)};
        mut result{
            map<int, base_t>(
                derived_to_copy_tpl,
                tpl(1, FWA_STD::move(derived_to_move)),
                tpl(2, other_derived_to_copy),
                tpl(3, FWA_STD::move(other_derived_to_move)),
                tpl(4, other_derived_t{}))};
        REQUIRE_EQ(result[0].get_label(), "copied");
        REQUIRE_EQ(result[1].get_label(), "moved");
        REQUIRE_EQ(result[2].get_label(), "moved");
        REQUIRE_EQ(result[3].get_label(), "moved");
        REQUIRE_EQ(result[4].get_label(), "moved");
        REQUIRE_EQ(result.bucket_count(), 8);
    }
}

#endif // ENV_MAP_HPP
