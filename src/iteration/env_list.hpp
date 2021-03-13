#ifndef ENV_LIST_HPP
#define ENV_LIST_HPP

// aliases

tmp<name TElements> typ(list_gt) = ENV_STD::vector<TElements, allocator_gt<TElements>>;

tmp<name TElements> typ(view_list_gt) = list_gt<ptr_gt<TElements>>;

tmp<name TElements> typ(poly_list_gt) = list_gt<poly_gt<TElements>>;

tmp<name... TElements> typ(list_vt) = list_gt<common_vt<TElements...>>;

// sized

tmp<name TElements>
    fun inl slst(size_t size = empty)->list_gt<TElements>
{
    list_gt<TElements> result;
    result.reserve(next_pow2(size));
    return result;
}

tmp<name TElements>
    fun inl slst(size_t size, const TElements &to_copy)->list_gt<TElements>
{
    mut result = slst<TElements>(size);

    mut _size = underlying_cast(size);
    do
    {
        result.emplace_back(to_copy);
    } while (--_size != empty);

    return result;
}

COND_TMP((name TElements, name... TArgs), (ENV_STD::is_constructible_v<TElements, const TArgs &...>))
fun inl slst(size_t size, const TArgs &...args)->list_gt<TElements>
{
    mut result = slst<TElements>(size);

    mut _size = underlying_cast(size);
    do
    {
        result.emplace_back(TElements{args...});
    } while (--_size != empty);

    return result;
}

ENV_TEST_CASE("sized list")
{
    SUBCASE("sized")
    {
        let _empty = slst<int>();
        REQUIRE_EQ(_empty.capacity(), 0);

        let _single = slst<int>(triple);
        REQUIRE_EQ(_single.capacity(), 4);
    }

    SUBCASE("copy")
    {
        let _copy = slst(10_s, 1);
        REQUIRE_EQ(_copy.capacity(), 16);
        REQUIRE(ENV_STD::all_of(_copy.begin(), _copy.end(), [](auto current) { return current == 1; }));
    }

    SUBCASE("emplace")
    {
        let _emplace = slst<int>(10_s, 2);
        REQUIRE_EQ(_emplace.capacity(), 16);
        REQUIRE(ENV_STD::all_of(_emplace.begin(), _emplace.end(), [](auto current) { return current == 2; }));
    }
}

// list

// TODO: make this better

ENV_NAMESPACE_DETAIL_BEGIN

tmp<name TCommon, name... TElements>
    typ(container_common_vt) =
        common_vt<
            ENV_STD::conditional_t<ENV_STD::is_same_v<TCommon, void_t>,
                                   name variadic_vt<TElements...>::tmp at_nt<0>,
                                   TCommon>,
            TElements...>;

ENV_NAMESPACE_DETAIL_END

COND_TMP((name TCommon = void_t, name... TElements), (sizeof...(TElements) != empty))
fun inl lst(TElements &&...elements)->list_gt<detail::container_common_vt<TCommon, TElements...>>
{
    mut result = slst<detail::container_common_vt<TCommon, TElements...>>(size_t{sizeof...(TElements)});
    ((result.emplace_back(ENV_STD::forward<TElements>(elements))), ...);

    return result;
}

ENV_TEST_CASE("list")
{
    SUBCASE("integral")
    {
        let a{0};
        let result = lst(1, a, 3.4);

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

        mut result{lst(to_copy, ENV_STD::move(to_move), test_t{})};
        REQUIRE_EQ(result[0].get_label(), "copied");
        REQUIRE_EQ(result[1].get_label(), "moved");
        REQUIRE_EQ(result[2].get_label(), "moved");
        REQUIRE_EQ(result.capacity(), 4);
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

        mut result{
            lst<base_t>(
                derived_to_copy,
                ENV_STD::move(derived_to_move),
                other_derived_to_copy,
                ENV_STD::move(other_derived_to_move),
                other_derived_t{})};
        REQUIRE_EQ(result[0].get_label(), "copied");
        REQUIRE_EQ(result[1].get_label(), "moved");
        REQUIRE_EQ(result[2].get_label(), "copied");
        REQUIRE_EQ(result[3].get_label(), "moved");
        REQUIRE_EQ(result[4].get_label(), "moved");
        REQUIRE_EQ(result.capacity(), 8);
    }
}

#endif // ENV_LIST_HPP
