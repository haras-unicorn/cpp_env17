#ifndef ENV_COMPRESSED_HPP
#define ENV_COMPRESSED_HPP


ENV_DETAIL_BEGIN

tmp<name TLhs, name TRhs, name = requirement_t>
strct compressed_pair_ggt;

tmp<name TLhs, name TRhs>
strct compressed_pair_ggt<
        TLhs, TRhs,
        ENV::require_nt<
                ENV::is_empty_base_g<TLhs> &&
                ENV::is_empty_base_g<TRhs> &&
                ENV::is_stable_g<TLhs> &&
                ENV::is_stable_g<TRhs>>> : public TLhs
{
    typ(lhs_t) = TLhs;
    typ(rhs_t) = TRhs;

    DECL((rhs_t), rhs);

    DEFAULT_LIFE(compressed_pair_ggt, CMP);

protected:
    GETTER_FML((), (lhs_t), _get_lhs, (*this));
};

tmp<name TLhs, name TRhs>
strct compressed_pair_ggt<
        TLhs, TRhs,
        ENV::require_nt<
                ENV::is_empty_base_g<TLhs> &&
                !ENV::is_empty_base_g<TRhs> &&
                ENV::is_stable_g<TLhs> &&
                ENV::is_stable_g<TRhs>>> : private TLhs
{
    typ(lhs_t) = TLhs;
    typ(rhs_t) = TRhs;

    DECL((rhs_t), rhs);

    DEFAULT_LIFE(compressed_pair_ggt, CMP);

protected:
    GETTER_FML((), (lhs_t), _get_lhs, (*this));
};

tmp<name TLhs, name TRhs>
strct compressed_pair_ggt<
        TLhs, TRhs,
        ENV::require_nt<
                !ENV::is_empty_base_g<TLhs> &&
                ENV::is_empty_base_g<TRhs> &&
                ENV::is_stable_g<TLhs> &&
                ENV::is_stable_g<TRhs>>> : private TRhs
{
    typ(lhs_t) = TLhs;
    typ(rhs_t) = TRhs;

    DECL((lhs_t), lhs);

    DEFAULT_LIFE(compressed_pair_ggt, CMP);

protected:
    GETTER_FML((), (rhs_t), _get_rhs, (*this));
};

tmp<name TLhs, name TRhs>
strct compressed_pair_ggt<
        TLhs, TRhs,
        ENV::require_nt<
                !ENV::is_empty_base_g<TLhs> &&
                !ENV::is_empty_base_g<TRhs> &&
                ENV::is_stable_g<TLhs> &&
                ENV::is_stable_g<TRhs>>>
{
    typ(lhs_t) = TLhs;
    typ(rhs_t) = TRhs;

    DECL((lhs_t), lhs);
    DECL((rhs_t), rhs);

    DEFAULT_LIFE(compressed_pair_ggt, CMP);
};

ENV_DETAIL_END


ENV_TEST_BEGIN

strct empty_t { };

ENV_TEST_END

ENV_TEST_CASE("compressed pair")
{
    REQUIRES(size_of_g<detail::compressed_pair_ggt<int, int>> ==
             size_of_g<int> * 2);
    REQUIRES(size_of_g<detail::compressed_pair_ggt<int, test::empty_t>> ==
             size_of_g<int>);
    REQUIRES(size_of_g<detail::compressed_pair_ggt<test::empty_t, int>> ==
             size_of_g<int>);
    REQUIRES(
            size_of_g<detail::compressed_pair_ggt<test::empty_t, empty_t>> ==
            size_of_g<empty_t>);

    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_pair_ggt<int, int>>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_pair_ggt<int, test::empty_t>>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_pair_ggt<test::empty_t, int>>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_pair_ggt<test::empty_t, empty_t>>);

    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_pair_ggt<int, int>>);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_pair_ggt<int, test::empty_t>>);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_pair_ggt<test::empty_t, int>>);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_pair_ggt<test::empty_t, empty_t>>);
}


ENV_DETAIL_BEGIN

tmp<name... T>
strct compressed_base_gs;

tmp<>
strct compressed_base_gs<> : public type_gt<empty_t>
{
};

tmp<name T>
strct compressed_base_gs<T> : public type_gt<unitary_gt < T>>
{
};

tmp<name TLhs, name TRhs>
strct compressed_base_gs<TLhs, TRhs> :
        public type_gt<compressed_pair_ggt < TLhs, TRhs>>
{
};

tmp<name THead, name... TRest>
strct compressed_base_gs<THead, TRest...> :
        public type_gt<
                compressed_pair_ggt <
                THead,
                name compressed_base_gs<TRest...>::type>>
{
};

ENV_DETAIL_END

ENV_TEST_CASE("compressed base")
{
    REQUIRES(size_of_g<detail::compressed_base_gs<>::type> ==
             size_of_g<empty_t>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_base_gs<>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_base_gs<>::type >);

    REQUIRES(size_of_g<detail::compressed_base_gs<int>::type> ==
             size_of_g<int>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_base_gs<int>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_base_gs<int>::type >);

    // ??
//    REQUIRES(size_of_g<detail::compressed_base_gs<empty_t>::type> ==
//             size_of_g<empty_t>);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_base_gs<empty_t>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_base_gs<empty_t>::type >);

    REQUIRES(size_of_g<detail::compressed_base_gs<int, int>::type> ==
             size_of_g<int> * 2);
    REQUIRES(size_of_g<detail::compressed_base_gs<int, empty_t>::type> ==
             size_of_g<int>);
    REQUIRES(size_of_g<detail::compressed_base_gs<empty_t, int>::type> ==
             size_of_g<int>);
    REQUIRES(size_of_g<detail::compressed_base_gs<test::empty_t, empty_t>::type>
             == size_of_g<empty_t>);

    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_base_gs<int, int>::type >);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_base_gs<int, empty_t>::type >);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_base_gs<empty_t, int>::type >);
    REQUIRES(ENV_STD::is_trivial_v <
             detail::compressed_base_gs<test::empty_t, empty_t>::type >);

    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_base_gs<int, int>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_base_gs<int, empty_t>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_base_gs<empty_t, int>::type >);
    REQUIRES(ENV_STD::is_standard_layout_v <
             detail::compressed_base_gs<test::empty_t, empty_t>::type >);
}


#endif // ENV_COMPRESSED_HPP
