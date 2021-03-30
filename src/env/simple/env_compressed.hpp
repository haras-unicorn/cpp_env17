#ifndef ENV_COMPRESSED_HPP
#define ENV_COMPRESSED_HPP


tmp<name... T>
strct compressed_vt : protected detail::compressed_traits_vs<T...>::type
{
protected:
    typ(traits_t) = detail::compressed_traits_vs<T...>;

    DECL_BASE(compressed, (name traits_t::type));


public:
    using _compressed_base_t::_compressed_base_t;

    DEFAULT_LIFE(compressed_vt, CMP);


    let_cmp static rank{traits_t::rank};

    tmp<index_t At> let_cmp static is_in_range{traits_t::tmp is_in_range<At>};


    COND_TMP((index_t At), is_in_range<At>)
    cmp_fn get() const noex -> const auto& { ret traits_t::tmp get<At>(*this); }

    COND_TMP((index_t At), is_in_range<At>)
    cmp_fn get() noex -> auto& { ret traits_t::tmp get<At>(*this); }


    cmp_obj static bool is_equatable{};
};


// ok, so, the problem was the templates
// NOTE: check if you're using EXPR_TMP instead of COND_TMP!
ENV_TEST_CASE("compressed")
{
    compressed_vt<int, empty_t, int, int, empty_t> compressed{1, 2, 3};
    nonce(compressed);
//     also crashes llvm on debug
    REQUIRE_EQ(compressed.get<0_i>(), 1);
    REQUIRE_EQ(compressed.get<2_i>(), 2);
    REQUIRE_EQ(compressed.get<3_i>(), 3);


//    ENV_HANA::tuple<int, empty_t, int, int, empty_t> hana_tuple{1, 2, 3};
//    REQUIRE_EQ(hana_tuple.get<0>(), 1);


    strct vector_t :
            protected compressed_vt<int, empty_t, int, int, empty_t>
    {
        DECL_BASE
        (
                compressed,
                (compressed_vt<int, empty_t, int, int, empty_t>)
        );


        // these things in particular crash llvm on debug
        GETTER(get_x, get<0_i>());
        GETTER(get_y, get<2_i>());
        GETTER(get_z, get<3_i>());


        con cmp inl vector_t(int x, int y, int z) noex:
                _compressed_base_t{x, y, z} { }

        // so I just learned that trivial types can't be constexpr until c++20...
        DEFAULT_LIFE(vector_t, NOEX);
    };

    REQUIRES(ENV_STD::is_trivial_v < vector_t >);
    REQUIRES(ENV_STD::is_standard_layout_v < vector_t >);
    REQUIRES(size_of_g < vector_t > == size_of_g < int > *3);

    cmp vector_t vec{1, 2, 3};
    nonce(vec);
    REQUIRE_EQ(vec.get_x(), 1);
    REQUIRE_EQ(vec.get_y(), 2);
    REQUIRE_EQ(vec.get_z(), 3);
}


#endif // ENV_COMPRESSED_HPP
