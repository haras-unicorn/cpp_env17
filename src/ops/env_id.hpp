#ifndef ENV_ID_HPP
#define ENV_ID_HPP

// No reinterpret_cast because that is not allowed in cmp functions.
// It seems like this is the same as reinterpret casting ptr -> uintptr_t -> id_t.

tmp<name T> cmp_fn id(T *ptr) noex { ret(id_t) ptr; }

ENV_TEST_CASE("id")
{
    int a{}, b{};
    REQUIRE_NE(id(&a), id(&b));
    REQUIRE_EQ(id(&a), id(&a));
}

// identifiable

EXPR_CHECK_UNARY(is_identifiable, (declvalr<T>().id()));

COND_CONCEPT(identifiable, (is_identifiable_g<C>));

tmp<name T> cmp_fn id(identifiable_c<T> &&subject) noex(noex(subject.id()))->id_t { ret subject.id(); }

ENV_TEST_CASE("identifiable")
{
    strct
    {
        cmp_fn id() const noex { ret 1_id; }
    }
    test{};
    REQUIRE_EQ(id(test), 1);
}

// id operations

#define MAKE_ID                                       \
    ACCESS_BEGIN(private);                            \
    fun inl static _make_id() noex->FWA_CORE::id_t    \
    {                                                 \
        obj static FWA_STD::atomic<id_t> _counter{0}; \
        return _counter++;                            \
    }                                                 \
    DEF((id_t), id, (_make_id()));                    \
    ACCESS_END(public)

#define DEFAULT_GET_ID_BODY (ret this->_get_id();)
#define DEFAULT_ID_HASH_BODY (ret FWA_CORE::hash(FWA_CORE::id(*this));)
#define DEFAULT_ID_EQUALS_BODY (ret FWA_CORE::id(*this) == FWA_CORE::id(rhs);)

// no cmp because of reinterpret_cast...

#define DEF_ELABORATE_ID_OPT(_get_id, _tmp, _hash, _equals)                                  \
    fun inl id() const noex->FWA_CORE::id_t { SPREAD(_get_id); }                             \
    fun inl static id(const _this_t &subject) noex->FWA_CORE::id_t { ret subject.id(); }     \
    fun inl get_id() const noex->FWA_CORE::id_t { ret this->id(); }                          \
    fun inl static get_id(const _this_t &subject) noex->FWA_CORE::id_t { ret subject.id(); } \
                                                                                             \
    ON_COND((!FWA_STD::is_same_v<FWA_CORE::id_t, FWA_CORE::hash_t>))                         \
    con op FWA_CORE::id_t() const noex { ret this->id(); };                                  \
                                                                                             \
    INTER(CMP_, _tmp, HASH) { SPREAD(_hash); };                                              \
    INTER(CMP_, _tmp, EQUALITY) { SPREAD(_equals); };

#define DEF_ELABORATE_ID(_get_id, _hash, _equals) DEF_ELABORATE_ID_OPT(_get_id, SKIP, _hash, _equals)
#define DEF_ELABORATE_TMP_ID(_get_id, _hash, _equals) DEF_ELABORATE_ID_OPT(_get_id, TMP_, _hash, _equals)

#define DEF_ID_WITH_OPT(_get_id, _tmp) DEF_ELABORATE_ID_OPT(_get_id, _tmp, DEFAULT_ID_HASH_BODY, DEFAULT_ID_EQUALS_BODY)
#define DEF_ID_WITH(_get_id) DEF_ID_WITH_OPT(_get_id, SKIP)
#define DEF_TMP_ID_WITH(_get_id) DEF_ID_WITH_OPT(_get_id, TMP_)

#define DEF_ID_OPT(_tmp) \
    MAKE_ID;             \
    DEF_ID_WITH_OPT(DEFAULT_GET_ID_BODY, _tmp)
#define DEF_ID DEF_ID_OPT(SKIP)
#define DEF_TMP_ID DEF_ID_OPT(TMP_)

FWA_NAMESPACE_TEST_BEGIN

cls id_struct_t
{
private:
    DECL_THIS(id_struct_t);

public:
    ENABLE_IF_COMPAT((is_imp_convertible_g<T, id_struct_t>));

    DEF_TMP_ID;
};

FWA_NAMESPACE_TEST_END

ENV_TEST_CASE("id operations")
{
    obj test::id_struct_t a{};
    obj test::id_struct_t b{};

    REQUIRE_EQ(a.get_id(), a.get_id());
    REQUIRE_NE(id(a), b.get_id());
    REQUIRES(FWA_CORE::is_identifiable_g<test::id_struct_t>);

    SUBCASE("hash")
    {
        REQUIRE_FALSE(a.hash_is_equal_to(b));
        REQUIRE_NE(a.hash(), b.hash());
        REQUIRE_EQ(hash(a), hasher(a));
    }

    SUBCASE("equality")
    {
        REQUIRE_EQ(a, a);
        REQUIRE_NE(a, b);
    }
}

#endif // ENV_ID_HPP
