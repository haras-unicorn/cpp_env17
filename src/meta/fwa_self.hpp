#ifndef ENV_SELF_HPP
#define ENV_SELF_HPP

// this type

#define DECL_THIS(_name)                              \
    ACCESS_BEGIN(private);                            \
    typ(_this_t) = _name;                             \
    typ(_this_ct) = FWA_STD::add_const_t<_this_t>;    \
    typ(_this_mt) = FWA_STD::remove_const_t<_this_t>; \
                                                      \
    typ(_this_ptr_t) = _this_t *;                     \
    typ(_c_this_ptr_t) = _this_ct *;                  \
    typ(_m_this_ptr_t) = _this_mt *;                  \
    ACCESS_END(public)

ENV_TEST_CASE("this type")
{
    strct test_t
    {
        DECL_THIS(test_t);

        fun inl op()() const noex
        {
            REQUIRE_EQT(_this_t, test_t);
            REQUIRE_EQT(_this_ct, const test_t);
            REQUIRE_EQT(_c_this_ptr_t, const test_t *);
        }
    }
    test{};

    test();
};

// no inheritor

strct no_inheritor_s
{
public:
    COND_CLASS_CHECK_UNARY(is_hash_eq_compatible_with, (FWA_CORE::first_nv<false, T>));

    COND_CLASS_CHECK_UNARY(is_equality_compatible_with, (FWA_CORE::first_nv<false, T>));

    COND_CLASS_CHECK_UNARY(is_comparison_compatible_with, (FWA_CORE::first_nv<false, T>));
};

tmp<name TInheritor>
    let_cmp is_inheritor_g{!FWA_STD::is_same_v<TInheritor, no_inheritor_s>};

tmp<name TThis, name TInheritor>
    typ(self_ggt) = FWA_STD::conditional_t<is_inheritor_g<TInheritor>, TInheritor, TThis>;

ENV_TEST_CASE("no inheritor type")
{
    SUBCASE("comparison checks")
    {
        REQUIRES_FALSE(no_inheritor_s::is_hash_eq_compatible_with_g<int>);
        REQUIRES_FALSE(no_inheritor_s::is_hash_eq_compatible_with_g<void>);
        REQUIRES_FALSE(no_inheritor_s::is_hash_eq_compatible_with_g<char>);

        REQUIRES_FALSE(no_inheritor_s::is_equality_compatible_with_g<int>);
        REQUIRES_FALSE(no_inheritor_s::is_equality_compatible_with_g<void>);
        REQUIRES_FALSE(no_inheritor_s::is_equality_compatible_with_g<char>);

        REQUIRES_FALSE(no_inheritor_s::is_comparison_compatible_with_g<int>);
        REQUIRES_FALSE(no_inheritor_s::is_comparison_compatible_with_g<void>);
        REQUIRES_FALSE(no_inheritor_s::is_comparison_compatible_with_g<char>);
    }

    SUBCASE("no inheritor check")
    {
        REQUIRES(is_inheritor_g<int>);
        REQUIRES_FALSE(is_inheritor_g<no_inheritor_s>);
    }

    SUBCASE("self")
    {
        REQUIRE_EQT(self_ggt<int, double>, double);
        REQUIRE_EQT(self_ggt<int, no_inheritor_s>, int);
    }
}

// inheritor type

#define inheritor_name name TInheritor = FWA_CORE::no_inheritor_s

#define DECL_INHERITOR_TYPE(_type)                              \
    ACCESS_BEGIN(private);                                      \
    typ(_inheritor_t) = SPREAD(_type);                          \
    typ(_inheritor_ct) = FWA_STD::add_const_t<_inheritor_t>;    \
    typ(_inheritor_mt) = FWA_STD::remove_const_t<_inheritor_t>; \
                                                                \
    typ(_inheritor_ptr_t) = _inheritor_t *;                     \
    typ(_c_inheritor_ptr_t) = _inheritor_ct *;                  \
    typ(_m_inheritor_ptr_t) = _inheritor_mt *;                  \
    ACCESS_END(public)

#define DECL_INHERITOR DECL_INHERITOR_TYPE((TInheritor))

FWA_NAMESPACE_TEST_BEGIN

tmp<inheritor_name>
    strct with_inheritor
{
    DECL_INHERITOR;

    fun inl test()
    {
        REQUIRE_EQT(_inheritor_t, int);
        REQUIRE_EQT(_inheritor_ct, const int);
        REQUIRE_EQT(_c_inheritor_ptr_t, const int *);
    }
};

FWA_NAMESPACE_TEST_END

ENV_TEST_CASE("inheritor type")
{
    test::with_inheritor<int>{}.test();
}

// self

#define DECL_SELF_INHERITOR(_name, _inheritor_type)                                         \
    DECL_THIS(_name);                                                                       \
    DECL_INHERITOR_TYPE(_inheritor_type);                                                   \
                                                                                            \
    ACCESS_BEGIN(private);                                                                  \
    cmp_obj static bool _has_inheritor = FWA_CORE::is_inheritor_g<SPREAD(_inheritor_type)>; \
                                                                                            \
    typ(_self_t) = FWA_CORE::self_ggt<_this_t, SPREAD(_inheritor_type)>;                    \
    typ(_self_ct) = FWA_STD::add_const_t<_self_t>;                                          \
    typ(_self_mt) = FWA_STD::remove_const_t<_self_t>;                                       \
                                                                                            \
    typ(_self_ptr_t) = _self_t *;                                                           \
    typ(_c_self_ptr_t) = _self_ct *;                                                        \
    typ(_m_self_ptr_t) = _self_mt *;                                                        \
    ACCESS_END(public)

#define DECL_SELF(_name) DECL_SELF_INHERITOR(_name, (TInheritor))

#define DECL_SELF_NO_INHERITOR(_name) DECL_SELF_INHERITOR(_name, (FWA_CORE::no_inheritor_s))

FWA_NAMESPACE_TEST_BEGIN

tmp<inheritor_name>
    strct with_self_t
{
    DECL_SELF(with_self_t);

    fun test()
    {
        SUBCASE("with inheritor")
        {
            REQUIRE_EQT(with_self_t<int>::_self_t, int);
            REQUIRE_EQT(with_self_t<>::_self_ct, const test::with_self_t<no_inheritor_s>);
            REQUIRE_EQT(with_self_t<int>::_c_self_ptr_t, const int *);
        }
    }

    tmp<name> friend struct with_self_t;
};

strct with_self_no_inheritor_t
{
    DECL_SELF_NO_INHERITOR(with_self_no_inheritor_t);

    fun test()
    {
        SUBCASE("without inheritor")
        {
            REQUIRE_EQT(_self_t, with_self_no_inheritor_t);
            REQUIRE_EQT(_self_ct, const with_self_no_inheritor_t);
            REQUIRE_EQT(_c_self_ptr_t, const with_self_no_inheritor_t *);
        }
    }
};

FWA_NAMESPACE_TEST_END

ENV_TEST_CASE("self")
{
    test::with_self_t<>{}.test();

    // static not allowed in local struct
    test::with_self_no_inheritor_t{}.test();
};

    // bases

#define BASE_NAME(_name) INTER(_, _name, _base_t)
#define BASE_CONST_NAME(_name) INTER(_, _name, _base_ct)
#define BASE_MUT_NAME(_name) INTER(_, _name, _base_mt)

#define BASE_PTR_NAME(_name) INTER(_, _name, _base_ptr_t)
#define BASE_CONST_PTR_NAME(_name) INTER(_c_, _name, _base_ptr_t)
#define BASE_MUT_PTR_NAME(_name) INTER(_m_, _name, _base_ptr_t)

#define _AS_BASE_NAME(_name) INTER(_as_, _name, _base)
#define AS_BASE_NAME(_name) INTER(as_, _name, _base)

#define GET_AS_BASE(_base) this->_AS_BASE_NAME(_base)()

#define DEF_AS_BASE_BODY(_return, _name, _body) \
    cmp_fn _name() const noex->const SPREAD(_return){SPREAD(_body)} fun inl _name() noex->SPREAD(_return){SPREAD(_body)} CLASS_SEMI

#define DEF_AS_BASE_FML(_return, _name, _formula) DEF_AS_BASE_BODY(_return, _name, FML_BODY(_formula))
#define _DEF_AS_BASE(_base) DEF_AS_BASE_FML((BASE_NAME(_base) *), _AS_BASE_NAME(_base), (this))
#define DEF_AS_BASE(_base) DEF_AS_BASE_FML((BASE_NAME(_base) &), AS_BASE_NAME(_base), (*GET_AS_BASE(_base)))

#define DECL_BASE(_name, _type)                                            \
    ACCESS_BEGIN(private);                                                 \
    typ(BASE_NAME(_name)) = SPREAD(_type);                                 \
    typ(BASE_CONST_NAME(_name)) = FWA_STD::add_const_t<BASE_NAME(_name)>;  \
    typ(BASE_MUT_NAME(_name)) = FWA_STD::remove_const_t<BASE_NAME(_name)>; \
                                                                           \
    typ(BASE_PTR_NAME(_name)) = BASE_NAME(_name) *;                        \
    typ(BASE_CONST_PTR_NAME(_name)) = BASE_CONST_NAME(_name) *;            \
    typ(BASE_MUT_PTR_NAME(_name)) = BASE_MUT_NAME(_name) *;                \
                                                                           \
    _DEF_AS_BASE(_name);                                                   \
    ACCESS_END(public)

ENV_TEST_CASE("bases")
{
    strct base_t{};
    strct derived_t : public base_t
    {
        DECL_BASE(my, (base_t));

        fun test()
        {
            REQUIRE_EQT(_my_base_t, base_t);
            REQUIRE_EQT(_my_base_ct, const base_t);
            REQUIRE_EQT(_c_my_base_ptr_t, const base_t *);
        }
    }
    derived{};

    derived.test();
};

    // self getters

#define CRTP_INHERITOR(_name, _inheritor_type)    \
    DECL_SELF_INHERITOR(_name, _inheritor_type);  \
                                                  \
    ACCESS_BEGIN(private);                        \
    cmp_fn _self() const noex->_c_self_ptr_t      \
    {                                             \
        if_cmp(_has_inheritor)                    \
            ret static_cast<_c_self_ptr_t>(this); \
        else ret this;                            \
    }                                             \
                                                  \
    fun inl _self() noex->_m_self_ptr_t           \
    {                                             \
        if_cmp(_has_inheritor)                    \
            ret static_cast<_m_self_ptr_t>(this); \
        else ret this;                            \
    }                                             \
    ACCESS_END(public)

#define CRTP(_name) CRTP_INHERITOR(_name, (TInheritor))

#define CRTP_BASE(_name, _type) \
    DECL_BASE(_name, _type);    \
    friend BASE_NAME(_name)

FWA_NAMESPACE_TEST_BEGIN

tmp<name TInheritor = no_inheritor_s>
    cls crtp_t
{
private:
    CRTP(crtp_t);

public:
    cmp_fn get_label() const noex { ret _self()->_get_label(); }

protected:
    cmp_fn _get_label() const noex->const char * { ret "base"; }
};

FWA_NAMESPACE_TEST_END

ENV_TEST_CASE("self getters")
{
    cls subclass_t : public test::crtp_t<subclass_t>
    {
    private:
        CRTP_BASE(crtp, (test::crtp_t<subclass_t>));

    protected:
        FWA_CLANG_SUPPRESS_PUSH("HidingNonVirtualFunction")

        cmp_fn _get_label() const noex->const char * { ret "derived"; }

        FWA_CLANG_SUPPRESS_POP
    };

    subclass_t sub{};
    test::crtp_t<> base{};

    // sub.get_as_crtp_base returns the base but it has the inheritor in the tmp, so
    // it returns itself in _self() and that results in unwanted behaviour for this test.

    REQUIRE_EQ(base.get_label(), "base");
    REQUIRE_EQ(sub.get_label(), "derived");
}

#endif // ENV_SELF_HPP
