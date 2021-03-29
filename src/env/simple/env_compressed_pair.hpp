#ifndef ENV_COMPRESSED_PAIR_HPP
#define ENV_COMPRESSED_PAIR_HPP


tmp<name TLhs, name TRhs, name = requirement_t>
strct compressed_pair_ggt;

tmp<name TLhs, name TRhs>
strct compressed_pair_ggt<
        TLhs, TRhs,
        ENV::require_nt<
                !ENV_STD::is_same_v<TLhs, TRhs> &&
                ENV::is_empty_base_g<TLhs> &&
                ENV::is_empty_base_g<TRhs> &&
                ENV::is_propagated_g<TLhs> &&
                ENV::is_propagated_g<TRhs>>> : public TLhs, public TRhs
{
    typ(lhs_t) = TLhs;
    typ(rhs_t) = TRhs;

    GETTER(get_lhs, _get_lhs());
    GETTER(get_rhs, _get_rhs());

protected:
    GETTER_FML((), (lhs_t), _get_lhs, (*this));
    GETTER_FML((), (rhs_t), _get_rhs, (*this));


public:
    EXPR_TMP
    ((name TCLhs, name... TCRhs),
     nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
     rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...})
    con cmp inl compressed_pair_ggt(TCLhs&& lhs, TCRhs&& ... rhs)
    noexpr
    (
            nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
            rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...}
    ):
            lhs_t{ENV_STD::forward<TCLhs>(lhs)},
            rhs_t{ENV_STD::forward<TCRhs>(rhs)...} { }

    EXPR_TMP
    ((name TCRhsHead, name... TCRhsRest),
     nonce(lhs_t{ }),
     rhs_t
             {
                 ENV_STD::forward<TCRhsHead>(declvalr<TCRhsHead>()),
                 ENV_STD::forward<TCRhsRest>(declvalr<TCRhsRest>())...
             })
    con cmp inl compressed_pair_ggt(
            TCRhsHead&& rhs_head,
            TCRhsRest&& ... rhs_rest)
    noexpr
    (
            nonce(lhs_t{ }),
            rhs_t
                    {
                        ENV_STD::forward<TCRhsHead>(declvalr<TCRhsHead>()),
                        ENV_STD::forward<TCRhsRest>(declvalr<TCRhsRest>())...
                    }
    ):
            lhs_t{ },
            rhs_t
                    {
                            ENV_STD::forward<TCRhsHead>(rhs_head),
                            ENV_STD::forward<TCRhsRest>(rhs_rest)...
                    } { }

    DEFAULT_LIFE(compressed_pair_ggt, CMP);
};

tmp<name T>
strct compressed_pair_ggt<
        T, T,
        ENV::require_nt<
                ENV::is_empty_base_g<T> &&
                ENV::is_propagated_g<T>>> : public T
{
    typ(lhs_t) = T;
    typ(rhs_t) = T;

    GETTER(get_lhs, _get_lhs());
    GETTER(get_rhs, _get_rhs());

    DECL((rhs_t), rhs);

protected:
    GETTER_FML((), (lhs_t), _get_lhs, (*this));


public:
    EXPR_TMP
    ((name TCLhs, name... TCRhs),
     nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
     rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...})
    con cmp inl compressed_pair_ggt(TCLhs&& lhs, TCRhs&& ... rhs)
    noexpr
    (
            nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
            rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...}
    ):
            lhs_t{ENV_STD::forward<TCLhs>(lhs)},
            _rhs{ENV_STD::forward<TCRhs>(rhs)...} { }

    EXPR_TMP
    ((name TCRhsHead, name... TCRhsRest),
     nonce(lhs_t{ }),
     rhs_t
             {
                 ENV_STD::forward<TCRhsHead>(declvalr<TCRhsHead>()),
                 ENV_STD::forward<TCRhsRest>(declvalr<TCRhsRest>())...
             })
    con cmp inl compressed_pair_ggt(
            TCRhsHead&& rhs_head,
            TCRhsRest&& ... rhs_rest)
    noexpr
    (
            nonce(lhs_t{ }),
            rhs_t
                    {
                        ENV_STD::forward<TCRhsHead>(declvalr<TCRhsHead>()),
                        ENV_STD::forward<TCRhsRest>(declvalr<TCRhsRest>())...
                    }
    ):
            lhs_t{ },
            _rhs
                    {
                            ENV_STD::forward<TCRhsHead>(rhs_head),
                            ENV_STD::forward<TCRhsRest>(rhs_rest)...
                    } { }

    DEFAULT_LIFE(compressed_pair_ggt, CMP);
};


tmp<name TLhs, name TRhs>
strct compressed_pair_ggt<
        TLhs, TRhs,
        ENV::require_nt<
                ENV::is_empty_base_g<TLhs> &&
                !ENV::is_empty_base_g<TRhs> &&
                ENV::is_propagated_g<TLhs> &&
                ENV::is_propagated_g<TRhs>>> : public TLhs
{
    typ(lhs_t) = TLhs;
    typ(rhs_t) = TRhs;

    GETTER(get_lhs, _get_lhs());
    GETTER(get_rhs, _get_rhs());

    DECL((rhs_t), rhs);

protected:
    GETTER_FML((), (lhs_t), _get_lhs, (*this));


public:
    EXPR_TMP
    ((name TCLhs, name... TCRhs),
     nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
     rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...})
    con cmp inl compressed_pair_ggt(TCLhs&& lhs, TCRhs&& ... rhs)
    noexpr
    (
            nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
            rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...}
    ):
            lhs_t{ENV_STD::forward<TCLhs>(lhs)},
            _rhs{ENV_STD::forward<TCRhs>(rhs)...} { }

    EXPR_TMP
    ((name TCRhsHead, name... TCRhsRest),
     nonce(lhs_t{ }),
     rhs_t
             {
                 ENV_STD::forward<TCRhsHead>(declvalr<TCRhsHead>()),
                 ENV_STD::forward<TCRhsRest>(declvalr<TCRhsRest>())...
             })
    con cmp inl compressed_pair_ggt(
            TCRhsHead&& rhs_head,
            TCRhsRest&& ... rhs_rest)
    noexpr
    (
            nonce(lhs_t{ }),
            rhs_t
                    {
                        ENV_STD::forward<TCRhsHead>(declvalr<TCRhsHead>()),
                        ENV_STD::forward<TCRhsRest>(declvalr<TCRhsRest>())...
                    }
    ):
            lhs_t{ },
            _rhs
                    {
                            ENV_STD::forward<TCRhsHead>(rhs_head),
                            ENV_STD::forward<TCRhsRest>(rhs_rest)...
                    } { }

    DEFAULT_LIFE(compressed_pair_ggt, CMP);
};


tmp<name TLhs, name TRhs>
strct compressed_pair_ggt<
        TLhs, TRhs,
        ENV::require_nt<
                !ENV::is_empty_base_g<TLhs> &&
                ENV::is_empty_base_g<TRhs> &&
                ENV::is_propagated_g<TLhs> &&
                ENV::is_propagated_g<TRhs>>> : public TRhs
{
    typ(lhs_t) = TLhs;
    typ(rhs_t) = TRhs;

    GETTER(get_lhs, _get_lhs());
    GETTER(get_rhs, _get_rhs());

    DECL((lhs_t), lhs);

protected:
    GETTER_FML((), (rhs_t), _get_rhs, (*this));


public:
    EXPR_TMP
    ((name TCLhs, name... TCRhs),
     nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
     rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...})
    con cmp inl compressed_pair_ggt(TCLhs&& lhs, TCRhs&& ... rhs)
    noexpr
    (
            nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
            rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...}
    ):
            rhs_t{ENV_STD::forward<TCRhs>(rhs)...},
            _lhs{ENV_STD::forward<TCLhs>(lhs)} { }

    EXPR_TMP
    ((name TCLhs),
     nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
     rhs_t{ })
    con cmp inl compressed_pair_ggt(TCLhs&& lhs)
    noexpr
    (
            nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
            rhs_t{ }
    ):
            rhs_t{ },
            _lhs{ENV_STD::forward<TCLhs>(lhs)} { }

    DEFAULT_LIFE(compressed_pair_ggt, CMP);
};


tmp<name TLhs, name TRhs>
strct compressed_pair_ggt<
        TLhs, TRhs,
        ENV::require_nt<
                !ENV::is_empty_base_g<TLhs> &&
                !ENV::is_empty_base_g<TRhs> &&
                ENV::is_propagated_g<TLhs> &&
                ENV::is_propagated_g<TRhs>>>
{
    typ(lhs_t) = TLhs;
    typ(rhs_t) = TRhs;

    GETTER(get_lhs, _get_lhs());
    GETTER(get_rhs, _get_rhs());

    DECL((lhs_t), lhs);
    DECL((rhs_t), rhs);


public:
    EXPR_TMP
    ((name TCLhs, name... TCRhs),
     nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
     rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...})
    con cmp inl compressed_pair_ggt(TCLhs&& lhs, TCRhs&& ... rhs)
    noexpr
    (
            nonce(lhs_t{ENV_STD::forward<TCLhs>(declvalr<TCLhs>())}),
            rhs_t{ENV_STD::forward<TCRhs>(declvalr<TCRhs>())...}
    ):
            _lhs{ENV_STD::forward<TCLhs>(lhs)},
            _rhs{ENV_STD::forward<TCRhs>(rhs)...} { }

    DEFAULT_LIFE(compressed_pair_ggt, CMP);
};


ENV_TEST_BEGIN

strct empty_t { };

ENV_TEST_END

ENV_TEST_CASE("compressed pair")
{
    REQUIRES(size_of_g<compressed_pair_ggt<int, int>> ==
             size_of_g<int> * 2);
    REQUIRES(size_of_g<compressed_pair_ggt<int, test::empty_t>> ==
             size_of_g<int>);
    REQUIRES(size_of_g<compressed_pair_ggt<test::empty_t, int>> ==
             size_of_g<int>);
    REQUIRES(
            size_of_g<compressed_pair_ggt<test::empty_t, empty_t>> ==
            size_of_g<empty_t>);

    REQUIRES(ENV_STD::is_trivial_v <
             compressed_pair_ggt<int, int>>);
    REQUIRES(ENV_STD::is_trivial_v <
             compressed_pair_ggt<int, test::empty_t>>);
    REQUIRES(ENV_STD::is_trivial_v <
             compressed_pair_ggt<test::empty_t, int>>);
    REQUIRES(ENV_STD::is_trivial_v <
             compressed_pair_ggt<test::empty_t, empty_t>>);

    REQUIRES(ENV_STD::is_standard_layout_v <
             compressed_pair_ggt<int, int>>);
    REQUIRES(ENV_STD::is_standard_layout_v <
             compressed_pair_ggt<int, test::empty_t>>);
    REQUIRES(ENV_STD::is_standard_layout_v <
             compressed_pair_ggt<test::empty_t, int>>);
    REQUIRES(ENV_STD::is_standard_layout_v <
             compressed_pair_ggt<test::empty_t, empty_t>>);


    cmp int a{1}, b{2};
    cmp compressed_pair_ggt<int, int> two_ints{a, b};
    nonce(two_ints);
    cmp compressed_pair_ggt<empty_t, int> one_int{1};
    nonce(one_int);


    compressed_pair_ggt<
            int, compressed_pair_ggt<
                    empty_t, compressed_pair_ggt<
                            empty_t, int>>>
    cmp nested{1, 2};

    REQUIRE_EQ(nested.get_lhs(), 1);
    REQUIRE_EQ(nested.get_rhs().get_rhs().get_rhs(), 2);


    compressed_pair_ggt<
            int, compressed_pair_ggt<
                    int, compressed_pair_ggt<
                            empty_t, compressed_pair_ggt<
                                    int, empty_t>>>>
    cmp compressed{a, b, 3};

    REQUIRE_EQ(compressed.get_lhs(), 1);
    REQUIRE_EQ(compressed.get_rhs().get_lhs(), 2);
    REQUIRE_EQ(compressed.get_rhs().get_rhs().get_rhs().get_lhs(), 3);


    compressed_pair_ggt<
            empty_t, compressed_pair_ggt<
                    int, compressed_pair_ggt<
                            int, empty_t>>>
    cmp idk{1, 2};

    REQUIRE_EQ(idk.get_rhs().get_lhs(), 1);
    REQUIRE_EQ(idk.get_rhs().get_rhs().get_lhs(), 2);


    compressed_pair_ggt<
            int, compressed_pair_ggt<
                    empty_t, compressed_pair_ggt<
                            int, compressed_pair_ggt<
                                    int, empty_t>>>>
    cmp rearranged{a, b, 3};

    REQUIRE_EQ(rearranged.get_lhs(), 1);
    REQUIRE_EQ(rearranged.get_rhs().get_rhs().get_lhs(), 2);
    REQUIRE_EQ(rearranged.get_rhs().get_rhs().get_rhs().get_lhs(), 3);
}


#endif // ENV_COMPRESSED_PAIR_HPP
