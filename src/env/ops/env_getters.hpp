#ifndef ENV_GETTERS_HPP
#define ENV_GETTERS_HPP


// member getters

// TODO: more tests

#define MEM_NAME(_name) CAT(_, _name)
#define MEM_TYPE(_name) INTER(_, _name, _t)

#define _GETTER_NAME(_name) CAT(_get_, _name)
#define GETTER_NAME(_name) CAT(get_, _name)

#define GET_MEM(_member) this->_GETTER_NAME(_member)()

// getter impls

#define SINGLE_GETTER_BODY_IMPL(_tmp, _type, _name, _pre, _post, _body)                                  \
    SPREAD(_tmp)                                                                                         \
    RETURN_ATTRIBUTES _pre inl SPREAD(_type) _name() _post noex /* NOLINT(readability-const-ret-type) */ \
    {                                                                                                    \
        SPREAD(_body)                                                                                    \
    }

#define GETTER_BODY_IMPL(_tmp, _type, _name, _pre_const, _body)                             \
    SINGLE_GETTER_BODY_IMPL(_tmp, (const SPREAD(_type) &), _name, _pre_const, const, _body) \
    SINGLE_GETTER_BODY_IMPL(_tmp, (SPREAD(_type) &), _name, SKIP, SKIP, _body)

// const and mutable

#define GETTER_BODY(_tmp, _type, _name, _body) \
    GETTER_BODY_IMPL(_tmp, _type, _name, SKIP, _body)

#define GETTER_FML(_tmp, _type, _name, _formula) \
    GETTER_BODY(_tmp, _type, _name, FML_BODY(_formula))

#define GETTER(_name, ...) \
    GETTER_FML((), (auto), _name, (__VA_ARGS__))

#define _MEM_GETTER_TMP(_tmp, _member)              \
    GETTER_FML(                                     \
        _tmp,                                       \
        (MEM_TYPE(_member)), _GETTER_NAME(_member), \
        (MEM_NAME(_member)))

#define _MEM_GETTER(_member) \
    _MEM_GETTER_TMP((), _member)

#define MEM_GETTER_TMP(_tmp, _member)              \
    GETTER_FML(                                    \
        _tmp,                                      \
        (MEM_TYPE(_member)), GETTER_NAME(_member), \
        (GET_MEM(_member)))

#define MEM_GETTER(_member) \
    MEM_GETTER_TMP((), _member)

// const or mutable

// cmp

#define CMP_GETTER_BODY(_tmp, _type, _name, _body) \
    SINGLE_GETTER_BODY_IMPL(_tmp, _type, _name, cmp, const, _body)

#define CMP_GETTER_FML(_tmp, _type, _name, _formula) \
    CMP_GETTER_BODY(_tmp, _type, _name, FML_BODY(_formula))

#define CMP_GETTER(_name, ...) CMP_GETTER_FML((), (auto), _name, (__VA_ARGS__))

#define MEM_CMP_GETTER(_member)                    \
    CMP_GETTER_FML(                                \
        (),                                        \
        (MEM_TYPE(_member)), GETTER_NAME(_member), \
        (MEM_NAME(_member)))

#define MEM_CMP_REF_GETTER(_member)                        \
    CMP_GETTER_FML(                                        \
        (),                                                \
        (const MEM_TYPE(_member) &), GETTER_NAME(_member), \
        (MEM_NAME(_member)))

// runtime

// const

#define CONST_GETTER_BODY(_tmp, _type, _name, _body) \
    SINGLE_GETTER_BODY_IMPL(_tmp, _type, _name, SKIP, const, _body)

#define CONST_GETTER_FML(_tmp, _type, _name, _formula) \
    CONST_GETTER_BODY(_tmp, _type, _name, FML_BODY(_formula))

#define CONST_GETTER(_name, ...) CONST_GETTER_FML((), (auto), _name, (__VA_ARGS__))

#define MEM_VAL_GETTER(_member)                    \
    CONST_GETTER_FML(                              \
        (),                                        \
        (MEM_TYPE(_member)), GETTER_NAME(_member), \
        (GET_MEM(_member)))

#define MEM_CONST_GETTER(_member)                          \
    CONST_GETTER_FML(                                      \
        (),                                                \
        (const MEM_TYPE(_member) &), GETTER_NAME(_member), \
        (GET_MEM(_member)))

// mutable

#define MUT_GETTER_BODY(_tmp, _type, _name, _body) \
    SINGLE_GETTER_BODY_IMPL(_tmp, _type, _name, SKIP, SKIP, _body)

#define MUT_GETTER_FML(_tmp, _type, _name, _formula) \
    MUT_GETTER_BODY(_tmp, _type, _name, FML_BODY(_formula))

#define MUT_GETTER(_name, ...) MUT_GETTER_FML((), (auto), _name, (__VA_ARGS__))

#define MEM_MUT_GETTER_TMP(_tmp, _member)            \
    MUT_GETTER_FML(                                  \
        _tmp,                                        \
        (MEM_TYPE(_member) &), GETTER_NAME(_member), \
        (GET_MEM(_member)))

#define MEM_MUT_GETTER(_member) \
    MEM_MUT_GETTER_TMP((), _member)


// members

#define DEF_MEM_TYPE(_type, _name) typ(MEM_TYPE(_name)) = SPREAD(_type)
#define DEF_DEDUCED_MEM_TYPE(_name, _init) typ(MEM_TYPE(_name)) = ENV::unqualified_gt<decltype(SPREAD(_init))>

#define DECL_MEM(_name) \
    MEM_TYPE(_name)     \
    MEM_NAME(_name)
#define DEF_MEM(_type, _name, _init) \
    DECL_MEM(_name) { SPREAD(_init) }
#define DEF_DEDUCED_MEM(_name, _init) \
    DECL_MEM(_name) { SPREAD(_init) }
#define DEF_NIL_MEM(_type, _name) \
    DECL_MEM(_name) { ENV::nil }

#define DECL(_type, _name)      \
    ACCESS_BEGIN(protected);    \
    DEF_MEM_TYPE(_type, _name); \
    _MEM_GETTER(_name);         \
                                \
private:                        \
    DECL_MEM(_name);            \
    ACCESS_END(public)

#define DEF(_type, _name, _init)  \
    ACCESS_BEGIN(protected);      \
    DEF_MEM_TYPE(_type, _name);   \
    _MEM_GETTER(_name);           \
                                  \
private:                          \
    DEF_MEM(_type, _name, _init); \
    ACCESS_END(public)

#define MEM(_name, _init)               \
    ACCESS_BEGIN(protected);            \
    DEF_DEDUCED_MEM_TYPE(_name, _init); \
    _MEM_GETTER(_name);                 \
                                        \
private:                                \
    DEF_DEDUCED_MEM(_name, _init);      \
    ACCESS_END(public)

#define NIL(_type, _name)       \
    ACCESS_BEGIN(protected);    \
    DEF_MEM_TYPE(_type, _name); \
    _MEM_GETTER(_name);         \
                                \
private:                        \
    DEF_NIL_MEM(_type, _name);  \
    ACCESS_END(public)

#define _DECL(_type, _name)     \
    ACCESS_BEGIN(private);      \
    DEF_MEM_TYPE(_type, _name); \
    _MEM_GETTER(_name);         \
    DECL_MEM(_name);            \
    ACCESS_END(public)

#define _DEF(_type, _name, _init) \
    ACCESS_BEGIN(private);        \
    DEF_MEM_TYPE(_type, _name);   \
    _MEM_GETTER(_name);           \
    DEF_MEM(_type, _name, _init); \
    ACCESS_END(public)

#define _MEM(_name, _init)              \
    ACCESS_BEGIN(private);              \
    DEF_DEDUCED_MEM_TYPE(_name, _init); \
    _MEM_GETTER(_name);                 \
    DEF_DEDUCED_MEM(_name, _init);      \
    ACCESS_END(public)

#define _NIL(_type, _name)      \
    ACCESS_BEGIN(private);      \
    DEF_MEM_TYPE(_type, _name); \
    _MEM_GETTER(_name);         \
    DEF_NIL_MEM(_type, _name);  \
    ACCESS_END(public)


ENV_TEST_CASE("members")
{
    strct test
    {
        DECL((int), x);
        DEF((int), y, (0));
        MEM(z, (0));
        NIL((int), w);

        _DECL((int), px);
        _DEF((int), py, (0));
        _MEM(pz, (0));
        _NIL((int), pw);

        callb op()() const noex
        {
            SUBCASE("type")
            {
                REQUIRE_EQT(_x_t, int);
                REQUIRE_EQT(_y_t, int);
                REQUIRE_EQT(_z_t, int);
                REQUIRE_EQT(_w_t, int);

                REQUIRE_EQT(_px_t, int);
                REQUIRE_EQT(_py_t, int);
                REQUIRE_EQT(_pz_t, int);
                REQUIRE_EQT(_pw_t, int);
            }

            SUBCASE("value")
            {
                REQUIRE_EQ(_x, 0);
                REQUIRE_EQ(_y, 0);
                REQUIRE_EQ(_z, 0);
                REQUIRE_EQ(_w, 0);

                REQUIRE_EQ(_px, 0);
                REQUIRE_EQ(_py, 0);
                REQUIRE_EQ(_pz, 0);
                REQUIRE_EQ(_pw, 0);
            }

            SUBCASE("getter")
            {
                REQUIRE_EQ(_get_x(), 0);
                REQUIRE_EQ(_get_y(), 0);
                REQUIRE_EQ(_get_z(), 0);
                REQUIRE_EQ(_get_w(), 0);

                REQUIRE_EQ(_get_px(), 0);
                REQUIRE_EQ(_get_py(), 0);
                REQUIRE_EQ(_get_pz(), 0);
                REQUIRE_EQ(_get_pw(), 0);
            }
        }
    };

    test{ }();
}


ENV_NAMESPACE_TEST_BEGIN

cls with_reference_getter_t
{
    DECL((const char *), member);

    con cmp with_reference_getter_t(_member_t value) : _member{ENV_STD::move(value)} { }

    MEM_GETTER(member);
};

cls with_value_getter_t
{
    DECL((const char *), member);

    con cmp with_value_getter_t(_member_t value) : _member{ENV_STD::move(value)} { }

    MEM_CMP_GETTER(member);
};

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("member getters")
{
    REQUIRES(is_ref_g < decltype(test::with_reference_getter_t{""}.get_member()) >);
    REQUIRES(!is_ref_g < decltype(test::with_value_getter_t{""}.get_member()) >);

    REQUIRE_EQ(test::with_reference_getter_t{"my_reference"}.get_member(), "my_reference");
    REQUIRE_EQ(test::with_value_getter_t{"my_value"}.get_member(), "my_value");

    SUBCASE("reference separate")
    {
        strct with_separate_reference_getters_t
        {
            DECL((const char *), member);

            con cmp with_separate_reference_getters_t(_member_t value) : _member{ENV_STD::move(value)} { }

            MEM_CONST_GETTER(member);
            MEM_MUT_GETTER(member);
        };

        REQUIRES(is_ref_g < decltype(with_separate_reference_getters_t{""}.get_member()) >);
        REQUIRE_EQ(with_separate_reference_getters_t{"my_reference"}.get_member(), "my_reference");
    };
}


// singleton getters

#define PROTECTED_SINGLETON_NAME _instance
#define PUBLIC_SINGLETON_NAME instance

#define GET_SINGLETON _this_t::PROTECTED_SINGLETON_NAME()

#define DECL_SINGLETON_GETTER(_name) fun static inl _name() noex

#define _SINGLETON_GETTER                                                 \
    friend struct std::default_delete<_this_t>;                           \
    friend std::unique_ptr<_this_t> std::make_unique<_this_t>();          \
                                                                          \
    DECL_SINGLETON_GETTER(PROTECTED_SINGLETON_NAME) -> _this_t&           \
    {                                                                     \
        let static _instance{ENV_STD::make_unique<_this_t>()};            \
        ret *_instance;                                                   \
    }


#define SINGLETON_GETTER                                      \
    DECL_SINGLETON_GETTER(PUBLIC_SINGLETON_NAME) -> _this_t&  \
    {                                                         \
        ret GET_SINGLETON;                                    \
    }                                                         \
                                                              \
private:                                                      \
    _SINGLETON_GETTER;                                        \
    ACCESS_END(public)

#define CONST_SINGLETON_GETTER                                      \
    DECL_SINGLETON_GETTER(PUBLIC_SINGLETON_NAME) -> const _this_t&  \
    {                                                               \
        ret GET_SINGLETON;                                          \
    }                                                               \
                                                                    \
private:                                                            \
    _SINGLETON_GETTER;                                              \
    ACCESS_END(public)

#define THREADED_MUT_SINGLETON_GETTER                                       \
    DECL_SINGLETON_GETTER(PUBLIC_SINGLETON_NAME)                            \
    {                                                                       \
        obj static ENV_STD::mutex _mutex{};                                 \
        ret ENV_STD::pair<ENV_STD::unique_lock<ENV_STD::mutex>, _this_t&>   \
                {                                                           \
                    ENV_STD::unique_lock<ENV_STD::mutex>{_mutex},           \
                    GET_SINGLETON                                           \
                };                                                          \
    }                                                                       \
                                                                            \
private:                                                                    \
    _SINGLETON_GETTER;                                                      \
    ACCESS_END(public)

#define THREADED_CONST_SINGLETON_GETTER                                           \
    DECL_SINGLETON_GETTER(PUBLIC_SINGLETON_NAME)                                  \
    {                                                                             \
        obj static ENV_STD::mutex _mutex{};                                       \
        ret ENV_STD::pair<ENV_STD::unique_lock<ENV_STD::mutex>, const _this_t&>   \
                {                                                                 \
                        ENV_STD::unique_lock<ENV_STD::mutex>{_mutex},             \
                        GET_SINGLETON                                             \
                };                                                                \
    }                                                                             \
                                                                                  \
private:                                                                          \
    _SINGLETON_GETTER;                                                            \
    ACCESS_END(public)


ENV_TEST_CASE("singleton getters")
{
    SUBCASE("singleton")
    {
        cls singleton_t : public test::with_reference_getter_t
        {
            DECL_THIS(singleton_t);

            singleton_t() : with_reference_getter_t{"singleton"} { }


            SINGLETON_GETTER;
        };

        REQUIRE_EQT(decl(singleton_t::instance()), singleton_t &);
        singleton_t::instance().get_member() = "overwrite";
        REQUIRE_EQ(singleton_t::instance().get_member(), "overwrite");
    }

    SUBCASE("const singleton")
    {
        cls singleton_t : public test::with_reference_getter_t
        {
            DECL_THIS(singleton_t);

            con singleton_t() : with_reference_getter_t{"const singleton"} { }

            CONST_SINGLETON_GETTER;
        };

        REQUIRE_EQT(decltype(singleton_t::instance()), const singleton_t &);
        REQUIRES(is_const_g < decltype(singleton_t::instance().get_member()) >);
        REQUIRE_EQ(singleton_t::instance().get_member(), "const singleton");
    }

    SUBCASE("thread safe singleton")
    {
        cls thread_safe_singleton_t : public test::with_reference_getter_t
        {
        private:
            DECL_THIS(thread_safe_singleton_t);

        public:
            con thread_safe_singleton_t() : with_reference_getter_t{"thread safe singleton"} { }

            THREADED_MUT_SINGLETON_GETTER;
        };

        {
            auto[lock, singleton] = thread_safe_singleton_t::instance();
            singleton.get_member() = "overwrite";
            REQUIRE_EQT(decltype(singleton), thread_safe_singleton_t &);
        }

        {
            auto[lock, singleton] = thread_safe_singleton_t::instance();
            REQUIRE_EQ(singleton.get_member(), "overwrite");
        }
    }

    SUBCASE("const thread safe singleton getter")
    {
        cls thread_safe_singleton_t : public test::with_reference_getter_t
        {
            DECL_THIS(thread_safe_singleton_t);

            thread_safe_singleton_t() : with_reference_getter_t{"const thread safe singleton"} { }

            THREADED_CONST_SINGLETON_GETTER;
        };

        auto[lock, singleton] = thread_safe_singleton_t::instance();
        REQUIRE_EQT(decltype(singleton), const thread_safe_singleton_t &);
        REQUIRES(is_const_g < decltype(singleton.get_member()) >);
        REQUIRE_EQ(singleton.get_member(), "const thread safe singleton");
    }
}


#endif // ENV_GETTERS_HPP
