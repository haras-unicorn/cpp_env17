#ifndef ENV_COMPRESSED_TRAITS_HPP
#define ENV_COMPRESSED_TRAITS_HPP


ENV_DETAIL_BEGIN

tmp<name... T>
strct compressed_traits_vs;

tmp<>
strct compressed_traits_vs<> : public type_gt<ENV::empty_t>
{
    DECL_BASE(type, (type_gt < empty_t > ));
    using name _type_base_t::type;

    let_cmp static rank{rank_zero};


    tmp<index_t At, dep_name>
    cmp_obj bool static is_in_range{ENV_STD::is_same_v<TDep, void> && false};

    COND_TMP((index_t At, dep_name), is_in_range<At, TDep>)
    cmp_callb static get(const type&) noex { }

    COND_TMP((index_t At, dep_name), is_in_range<At, TDep>)
    cmp_callb static get(type&) noex { }
};

tmp<name T>
strct compressed_traits_vs<T> : public type_gt<ENV::unitary_gt<T>>
{
    DECL_BASE(type, (type_gt < ENV::unitary_gt < T >> ));
    using name _type_base_t::type;

    let_cmp static rank{rank_one};

    tmp<index_t At>
    cmp_obj bool static is_in_range{At < ENV_STD::size_t{rank}};

    COND_TMP((index_t At), is_in_range<At>)
    cmp_fn static get(const type& _subject) noex -> const auto&
    {
        ret _subject.get();
    }

    COND_TMP((index_t At), is_in_range<At>)
    cmp_fn static get(type& _subject) noex -> auto&
    {
        ret _subject.get();
    }
};

tmp<name TLhs, name TRhs>
strct compressed_traits_vs<TLhs, TRhs> :
        public type_gt<ENV::compressed_pair_ggt<TLhs, TRhs>>
{
    DECL_BASE(type, (type_gt < compressed_pair_ggt < TLhs, TRhs >>));
    using name _type_base_t::type;

    let_cmp static rank{rank_two};

    tmp<index_t At>
    cmp_obj bool static is_in_range{At < ENV_STD::size_t{rank}};

    COND_TMP((index_t At), is_in_range<At>)
    cmp_fn static get(const type& _subject) noex -> const auto&
    {
        if_cmp(At == first)
        {
            ret _subject.get_lhs();
        }
        else
        {
            ret _subject.get_rhs();
        }
    }

    COND_TMP((index_t At), is_in_range<At>)
    cmp_fn static get(type& _subject) noex -> auto&
    {
        if_cmp(At == first)
        {
            ret _subject.get_lhs();
        }
        else
        {
            ret _subject.get_rhs();
        }
    }
};

tmp<name THead, name... TRest>
strct compressed_traits_vs<THead, TRest...> :
        public type_gt<
                ENV::compressed_pair_ggt<
                        THead,
                        name ENV::detail::compressed_traits_vs<TRest...>::type>>
{
    DECL_BASE
    (
            type,
            (
                    type_gt < compressed_pair_ggt <
                    THead,
                            name compressed_traits_vs<TRest...>::type >>
            )
    );
    using name _type_base_t::type;

    typ(rest_traits_t) = compressed_traits_vs<TRest...>;

    let_cmp static rank{rank_of_v < THead, TRest...>};

    tmp<index_t At>
    cmp_obj bool static is_in_range{At < ENV_STD::size_t{rank}};


    COND_TMP((index_t At), is_in_range<At>)
    cmp_fn static get(const type& _subject) noex -> const auto&
    {
        if_cmp(At == first)
        {
            ret _subject.get_lhs();
        }
        else
        {
            ret rest_traits_t::tmp get<index_t{At - 1}>(_subject.get_rhs());
        }
    }

    COND_TMP((index_t At), is_in_range<At>)
    cmp_fn static get(type& _subject) noex -> auto&
    {
        if_cmp(At == first)
        {
            ret _subject.get_lhs();
        }
        else
        {
            ret rest_traits_t::tmp get<index_t{At - 1}>(_subject.get_rhs());
        }
    }
};

ENV_DETAIL_END


ENV_TEST_CASE("compressed traits")
{
    REQUIRES(size_of_g<detail::compressed_traits_vs<>::type> ==
             size_of_g<empty_t>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_traits_vs<>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_traits_vs<>::type >);

    REQUIRES(size_of_g<detail::compressed_traits_vs<int>::type> ==
             size_of_g<int>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_traits_vs<int>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_traits_vs<int>::type >);

    // ??
//    REQUIRES(size_of_g<detail::compressed_traits_vs<empty_t>::type> ==
//             size_of_g<empty_t>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_traits_vs<empty_t>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_traits_vs<empty_t>::type >);

    REQUIRES(size_of_g<detail::compressed_traits_vs<int, int>::type> ==
             size_of_g<int> * 2);
    REQUIRES(size_of_g<detail::compressed_traits_vs<int, empty_t>::type> ==
             size_of_g<int>);
    REQUIRES(size_of_g<detail::compressed_traits_vs<empty_t, int>::type> ==
             size_of_g<int>);
    REQUIRES(size_of_g<
            detail::compressed_traits_vs<test::empty_t, empty_t>::type>
             == size_of_g<empty_t>);

    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_traits_vs<int, int>::type >);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_traits_vs<int, empty_t>::type >);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_traits_vs<empty_t, int>::type >);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_traits_vs<test::empty_t, empty_t>::type >);

    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_traits_vs<int, int>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_traits_vs<int, empty_t>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_traits_vs<empty_t, int>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_traits_vs<test::empty_t, empty_t>::type >);

    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_traits_vs<int, empty_t, int>::type >);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_traits_vs<int, empty_t, int>::type >);
    REQUIRES(size_of_g<detail::compressed_traits_vs<int, empty_t, int>::type>
             == size_of_g<int> * 2);

    REQUIRES(detail::compressed_traits_vs<>::rank == 0);
    REQUIRES(detail::compressed_traits_vs<int>::rank == 1);
    REQUIRES(detail::compressed_traits_vs<int, empty_t>::rank == 2);
    REQUIRES(detail::compressed_traits_vs<int, empty_t, int>::rank == 3);


    compressed_pair_ggt<
            int, compressed_pair_ggt<
                    int, compressed_pair_ggt<
                            empty_t, compressed_pair_ggt<
                                    int, empty_t>>>>
    cmp compressed{1, 2, 3};

    REQUIRE_EQ(compressed.get_lhs(), 1);
    REQUIRE_EQ(compressed.get_rhs().get_lhs(), 2);
    REQUIRE_EQ(compressed.get_rhs().get_rhs().get_rhs().get_lhs(), 3);

    // hmm so this doesn't crash llvm on debug...
    typ(traits_t) = detail::compressed_traits_vs<
            int, int, empty_t, int, empty_t>;
    REQUIRE_EQ(traits_t::get<0_i>(compressed), 1);
    REQUIRE_EQ(traits_t::get<1_i>(compressed), 2);
    REQUIRE_EQ(traits_t::get<3_i>(compressed), 3);
}


#endif // ENV_COMPRESSED_TRAITS_HPP
