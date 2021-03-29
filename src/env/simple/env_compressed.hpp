#ifndef ENV_COMPRESSED_HPP
#define ENV_COMPRESSED_HPP


tmp<name... T>
strct compressed_vt : public detail::compressed_traits_vs<T...>::type
{
    DEFAULT_LIFE(compressed_vt, CMP);


protected:
    typ(traits_t) = detail::compressed_traits_vs<T...>;


    DECL_BASE(compressed, (name traits_t::type));
    using _compressed_base_t::_compressed_base_t;


    let_cmp static rank{traits_t::rank};

    tmp<index_t At> let_cmp static is_in_range{traits_t::tmp is_in_range<At>};


    EXPR_TMP((index_t At), is_in_range<At>)
    cmp_fn _get() const noex -> const auto&
    {
        ret detail::compressed_traits_vs<T...>::tmp get<At>(*this);
    }

    EXPR_TMP((index_t At), is_in_range<At>)
    cmp_fn _get() noex -> auto&
    {
        ret detail::compressed_traits_vs<T...>::tmp get<At>(*this);
    }
};

ENV_TEST_CASE("compressed")
{
    strct vector_t :
            protected compressed_vt<int, empty_t, int, int, empty_t>
    {
        DECL_BASE
        (
                compressed,
                (compressed_vt<int, empty_t, int, int, empty_t>)
        );


        GETTER(get_x, _get<0_i>());
        GETTER(get_y, _get<2_i>());
        GETTER(get_z, _get<3_i>());


        con cmp inl vector_t(int x, int y, int z) noex:
                _compressed_base_t{x, y, z} { }

        // so I just learned that trivial types can't be constexpr until c++20...
        DEFAULT_LIFE(vector_t, NOEX);
    };

    REQUIRES(ENV_STD::is_trivial_v < vector_t >);
    REQUIRES(ENV_STD::is_standard_layout_v < vector_t >);
    REQUIRES(size_of_g < vector_t > == size_of_g < int > *3);

    cmp vector_t vec{1, 2, 3};
    REQUIRE_EQ(vec.get_x(), 1);
    REQUIRE_EQ(vec.get_y(), 2);
    REQUIRE_EQ(vec.get_z(), 3);
}


#endif // ENV_COMPRESSED_HPP
