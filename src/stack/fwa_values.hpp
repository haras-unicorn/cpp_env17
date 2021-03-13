#ifndef ENV_VALUES_HPP
#define ENV_VALUES_HPP

// precision

enm precision_t{
    normal,
    precise,
    highly_precise};

// concepts

COND_CHECK_UNARY(is_val, (FWA_STD::is_floating_point_v<T>));

COND_CONCEPT(val, (is_val_g<C>));

COND_CHECK_UNARY(is_compliant_val, (FWA_STD::is_same_v<T, float>));

COND_CONCEPT(compliant_val, (is_compliant_val_g<C>));

// literals

WHOLE_L(bit_depth, bd, natural_t);

FLOATING_L(sample_rate, sr, rational_t);

FLOATING_L(val, v, f_t);

FLOATING_L(p_val, pv, pf_t);

FLOATING_L(hp_val, hpv, hpf_t);

// traits

FWA_NAMESPACE_DETAIL_BEGIN

tmp<name T>
    strct val_gs
{
        typ(val_t) = T;
        typ(val_ct) = FWA_STD::add_const_t<val_t>;
        typ(val_mt) = FWA_STD::remove_const_t<val_t>;

        cmp_obj static bool is_processable{
            FWA_STD::is_same_v<val_gs::val_t, FWA_CORE::val_t> ||
            FWA_STD::is_same_v<val_gs::val_t, FWA_CORE::p_val_t>};

        cmp_obj static precision_t precision{
            FWA_STD::is_same_v<val_gs::val_mt, FWA_CORE::p_val_t> ? precision_t::precise : FWA_STD::is_same_v<val_gs::val_mt, FWA_CORE::hp_val_t> ? precision_t::highly_precise
                                                                                                                                                  : precision_t::normal};
        cmp_obj static bool is_precise{
            FWA_STD::is_same_v<val_gs::val_mt, FWA_CORE::p_val_t> ||
            FWA_STD::is_same_v<val_gs::val_mt, FWA_CORE::hp_val_t>};

        cmp_obj static bit_depth_t bit_depth{
            FWA_STD::is_same_v<val_gs::val_mt, FWA_CORE::val_t> ? 24 : FWA_STD::is_same_v<val_gs::val_mt, FWA_CORE::p_val_t> ? 53
                                                                                                                             : 0};
};

tmp<name T> let_cmp is_processable_g{val_gs<T>::is_processable};

tmp<name T> let_cmp precision_g{val_gs<T>::precision};

tmp<name T> let_cmp is_precise_g{val_gs<T>::is_precise};

tmp<name T> let_cmp bit_depth_g{val_gs<T>::bit_depth};

FWA_NAMESPACE_DETAIL_END

tmp<name T> typ(val_gs) = detail::val_gs<val_c<T>>;

tmp<name T> let_cmp is_processable_g{detail::is_processable_g<val_c<T>>};

tmp<name T> let_cmp precision_g{detail::precision_g<val_c<T>>};

tmp<name T> let_cmp is_precise_g{detail::is_precise_g<val_c<T>>};

tmp<name T> let_cmp bit_depth_g{detail::bit_depth_g<val_c<T>>};

ENV_TEST_CASE("val traits")
{
        REQUIRE_EQT(val_gs<p_val_t>::val_t, pf_t);
        REQUIRE_EQ(val_gs<hp_val_t>::precision, precision_t::highly_precise);
        REQUIRE_EQ(val_gs<hp_val_t>::is_processable, false);
        REQUIRE_EQ(val_gs<hp_val_t>::is_precise, true);
        REQUIRE_EQ(val_gs<val_t>::bit_depth, 24);
}

// precision traits

// There is already a val_nt

tmp<precision_t Precision>
    typ(val_nt) =
        FWA_STD::conditional_t<
            Precision == precision_t::precise, p_val_t, FWA_STD::conditional_t<Precision == precision_t::highly_precise, hp_val_t, val_t>>;

tmp<precision_t Precision> typ(val_ns) = val_gs<val_nt<Precision>>;

tmp<precision_t Precision> let_cmp is_processable_n{val_ns<Precision>::is_processable};

tmp<precision_t Precision> let_cmp precision_n{val_ns<Precision>::precision};

tmp<precision_t Precision> let_cmp is_precise_n{val_ns<Precision>::is_precise};

tmp<precision_t Precision> let_cmp bit_depth_n{val_ns<Precision>::bit_depth};

ENV_TEST_CASE("precision traits")
{
        REQUIRE_EQT(val_ns<precision_t::normal>::val_t, float);
        REQUIRE_EQ(val_ns<precision_t::highly_precise>::precision, precision_t::highly_precise);
        REQUIRE_EQ(val_ns<precision_t::highly_precise>::is_processable, false);
        REQUIRE_EQ(val_ns<precision_t::highly_precise>::is_precise, true);
        REQUIRE_EQ(val_ns<precision_t::normal>::bit_depth, 24);
}

#endif // ENV_VALUES_HPP
