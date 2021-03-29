#ifndef ENV_DEFAULTED_HPP
#define ENV_DEFAULTED_HPP


ENV_DETAIL_BEGIN

tmp<name T, name TConstruct, name = requirement_t>
strct defaulted_ggt;

tmp<name T, name TConstruct>
strct defaulted_ggt<
        T, TConstruct,
        EXPR_TYPE(T{unqualified_gt < TConstruct > { }()})> : public T
{
    DECL_BASE(defaulted, (T));
    using _defaulted_base_t::_defaulted_base_t;

    con cmp inl defaulted_ggt()
    noexpr(_defaulted_base_t{unqualified_gt < TConstruct > { }()}):
            _defaulted_base_t{unqualified_gt < TConstruct > { }()} { }
};


tmp<name T, deduc Construct, name = requirement_t>
strct defaulted_gnt;

tmp<name T, deduc Construct>
strct defaulted_gnt<T, Construct, EXPR_TYPE(T{Construct})> : public T
{
    DECL_BASE(defaulted, (T));
    using _defaulted_base_t::_defaulted_base_t;

    con cmp inl defaulted_gnt() noexpr(_defaulted_base_t{Construct}):
            _defaulted_base_t{Construct} { }
};

tmp<name T, deduc Construct>
strct defaulted_gnt<T, Construct, EXPR_TYPE(T{Construct()})> : public T
{
    DECL_BASE(defaulted, (T));
    using _defaulted_base_t::_defaulted_base_t;

    con cmp inl defaulted_gnt() noexpr(_defaulted_base_t{Construct()}):
            _defaulted_base_t{Construct()} { }
};

ENV_DETAIL_END


tmp<name T, name TConstruct>
typ(defaulted_ggt) = detail::defaulted_ggt<T, TConstruct>;

tmp<name T, deduc Construct>
typ(defaulted_gnt) = detail::defaulted_gnt<T, Construct>;


ENV_TEST_CASE("defaulted")
{
    defaulted_gnt<unitary_gt<int>, 1> def_one{ };
    REQUIRE_EQ(def_one.get(), 1);

    REQUIRES(ENV_STD::is_standard_layout_v <
             defaulted_gnt < unitary_gt<int>, 1 >>);


    // until c++20
    strct cons_t { cmp_fn op()() const noex { ret 2; } };
    defaulted_ggt<unitary_gt<int>, cons_t> def_two{ };
    REQUIRE_EQ(def_two.get(), 2);

    REQUIRES(ENV_STD::is_standard_layout_v <
             defaulted_ggt<unitary_gt<int>, cons_t >>);


    // c++20 closures have default constructors...
//    let cmp cons{[] { ret 3; }};
//    nonce(cons);
//    int a{decl(cons){ }()};
//    defaulted_ggt<unitary_gt<int>, decl(cons)> def_three{ };
//    REQUIRE_EQ(def_three.get(), 3);
}


#endif // ENV_DEFAULTED_HPP
