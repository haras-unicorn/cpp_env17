#ifndef ENV_VALUES_HPP
#define ENV_VALUES_HPP


// copy/move

tmp<name T> cmp_fn move(T&& t) noex -> remove_ref_gt <T>&& { ret scast<remove_ref_gt<T>&&>(t); }

tmp<name T> cmp_fn copy(T& t) noex -> const T& { ret scast<const T&>(t); }

ENV_TEST_CASE("copy/move")
{
    REQUIRE_EQT(decl(move(declvalr<int>())), int &&);
    REQUIRE_EQT(decl(move(declvalr<const int>())), const int&&);
    REQUIRE_EQT(decl(move(declvall<int>())), int &&);
    REQUIRE_EQT(decl(move(declvall<const int>())), const int&&);

    REQUIRE_EQT(decl(copy(declvalr<const int>())), const int&);
    REQUIRE_EQT(decl(copy(declvall<int>())), const int &);
    REQUIRE_EQT(decl(copy(declvall<const int>())), const int&);
}


// precision

enm precision_t
{
    normal,
    precise,
    highly_precise
};


// concepts

COND_CHECK_UNARY(is_val, (ENV_STD::is_floating_point_v < T > ));

COND_CONCEPT(val, (is_val_g<C>));

COND_CHECK_UNARY(is_compliant_val, (ENV_STD::is_same_v < T, float >));

COND_CONCEPT(compliant_val, (is_compliant_val_g<C>));


// literals

WHOLE_L(bit_depth, bd, natural_t);

FLOATING_L(sample_rate, sr, rational_t);

FLOATING_L(val, v, f_t);

FLOATING_L(p_val, pv, pf_t);

FLOATING_L(hp_val, hpv, hpf_t);


// traits

ENV_DETAIL_BEGIN

tmp<name T>
strct val_gs
{
    typ(val_t) = T;
    typ(val_ct) = ENV_STD::add_const_t<val_t>;
    typ(val_mt) = ENV_STD::remove_const_t<val_t>;

    cmp_obj static bool is_processable
            {
                    ENV_STD::is_same_v<val_gs::val_t, ENV::val_t> ||
                    ENV_STD::is_same_v<val_gs::val_t, ENV::p_val_t>
            };

    cmp_obj static precision_t precision
            {
                    ENV_STD::is_same_v<val_gs::val_mt, ENV::p_val_t> ? precision_t::precise :
                    ENV_STD::is_same_v<val_gs::val_mt, ENV::hp_val_t> ? precision_t::highly_precise :
                    precision_t::normal
            };
    cmp_obj static bool is_precise
            {
                    ENV_STD::is_same_v<val_gs::val_mt, ENV::p_val_t> ||
                    ENV_STD::is_same_v<val_gs::val_mt, ENV::hp_val_t>
            };

    cmp_obj static bit_depth_t bit_depth
            {
                    ENV_STD::is_same_v<val_gs::val_mt, ENV::val_t> ? 24 :
                    ENV_STD::is_same_v<val_gs::val_mt, ENV::p_val_t> ? 53 : 0
            };
};

tmp<name T> let_cmp is_processable_g{val_gs<T>::is_processable};

tmp<name T> let_cmp precision_g{val_gs<T>::precision};

tmp<name T> let_cmp is_precise_g{val_gs<T>::is_precise};

tmp<name T> let_cmp bit_depth_g{val_gs<T>::bit_depth};

ENV_DETAIL_END

tmp<name T> typ(val_gs) = detail::val_gs<val_r<T>>;

tmp<name T> let_cmp is_processable_g{detail::is_processable_g<val_r<T>>};

tmp<name T> let_cmp precision_g{detail::precision_g<val_r<T>>};

tmp<name T> let_cmp is_precise_g{detail::is_precise_g<val_r<T>>};

tmp<name T> let_cmp bit_depth_g{detail::bit_depth_g<val_r<T>>};

ENV_TEST_CASE("val traits")
{
    REQUIRE_EQT(val_gs<p_val_t>::val_t, pf_t);

    REQUIRES(val_gs<hp_val_t>::precision == precision_t::highly_precise);
    REQUIRES_FALSE(val_gs<hp_val_t>::is_processable);
    REQUIRES(val_gs<hp_val_t>::is_precise);
    REQUIRES(val_gs<val_t>::bit_depth == 24);

    REQUIRES(precision_g < hp_val_t > == precision_t::highly_precise);
    REQUIRES_FALSE(is_processable_g < hp_val_t >);
    REQUIRES(is_precise_g < hp_val_t >);
    REQUIRES(bit_depth_g < val_t > == 24);
}


// precision traits

// There is already a val_nt

tmp<precision_t Precision>
typ(val_nt) =
ENV_STD::conditional_t<
        Precision == precision_t::precise, p_val_t, ENV_STD::conditional_t<
                Precision == precision_t::highly_precise, hp_val_t, val_t>>;

tmp<precision_t Precision> typ(val_ns) = val_gs<val_nt<Precision>>;

tmp<precision_t Precision> let_cmp is_processable_n{val_ns<Precision>::is_processable};

tmp<precision_t Precision> let_cmp precision_n{val_ns<Precision>::precision};

tmp<precision_t Precision> let_cmp is_precise_n{val_ns<Precision>::is_precise};

tmp<precision_t Precision> let_cmp bit_depth_n{val_ns<Precision>::bit_depth};

ENV_TEST_CASE("precision traits")
{
    REQUIRE_EQT(val_ns<precision_t::normal>::val_t, float);

    REQUIRES(val_ns<precision_t::highly_precise>::precision == precision_t::highly_precise);
    REQUIRES_FALSE(val_ns<precision_t::highly_precise>::is_processable);
    REQUIRES(val_ns<precision_t::highly_precise>::is_precise);
    REQUIRES(val_ns<precision_t::normal>::bit_depth == 24);

    REQUIRES(precision_n < precision_t::highly_precise > == precision_t::highly_precise);
    REQUIRES_FALSE(is_processable_n < precision_t::highly_precise >);
    REQUIRES(is_precise_n < precision_t::highly_precise >);
    REQUIRES(bit_depth_n < precision_t::normal > == 24);
}


#endif // ENV_VALUES_HPP
