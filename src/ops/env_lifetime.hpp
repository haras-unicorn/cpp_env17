#ifndef ENV_LIFETIME_HPP
#define ENV_LIFETIME_HPP

// names

// modular definitions

#define DEF_DEFAULT_CONSTRUCT_AUTO(_construct, _copy, _move) \
    SPREAD(_construct) {}
#define DEF_DEFAULT_CONSTRUCT_MANUAL(__construct, _copy, _move) \
    SPREAD(__construct) { this->_construct(); }

#define DEF_COPY_AUTO(_construct, _copy, _move) \
    SPREAD(_copy) {}
#define DEF_COPY_MANUAL(_construct, _copy, _move) \
    SPREAD(_copy) { this->_copy_move(other); }
#define DEF_MOVE_AUTO(_construct, _copy, _move) \
    SPREAD(_move) {}
#define DEF_MOVE_MANUAL(_construct, _copy, _move) \
    SPREAD(_move) { this->_copy_move(ENV_STD::move(other)); }

#define DEF_OPERATOR_COPY_MOVE(_construct, _copy, _move) \
    {                                                    \
        this->_copy_move(other);                         \
        ret *this;                                       \
    }
#define DEF_OPERATOR_MOVE(_construct, _copy, _move) \
    {                                               \
        this->_copy_move(ENV_STD::move(other));     \
        ret *this;                                  \
    }

#define DEF_DESTRUCT(_construct, _copy, _move) \
    {                                          \
        this->_destruct();                     \
    }

// modular lifetime

// These will certainly trigger unused macro inspections because they are never used directly.

ENV_CLANG_SUPPRESS_PUSH("OCUnusedMacroInspection")

#define DELETE_METHOD = delete;
#define DEFAULT_METHOD = default;

#define WRAP_LIFETIME_ATTRIBUTE(_attribute, _name) DECL_##_attribute##_##_name

#define DECL_DEFAULT_CONSTRUCTOR(_name, _pre, _post) \
    RETURN_ATTRIBUTES _pre _name() _post

#define DECL_DELETED_DEFAULT_CONSTRUCTOR(_name) \
    DECL_DEFAULT_CONSTRUCTOR(_name, , DELETE_METHOD)

#define DECL_NO_ATTRIBUTE_DEFAULT_CONSTRUCTOR(_name, _body) \
    DECL_DEFAULT_CONSTRUCTOR(_name, , PACK(_body))

#define DECL_NOEX_DEFAULT_CONSTRUCTOR(_name, _body) \
    DECL_DEFAULT_CONSTRUCTOR(_name, , PACK(noex _body))

#define DECL_CMP_DEFAULT_CONSTRUCTOR(_name, _body) \
    DECL_DEFAULT_CONSTRUCTOR(_name, inl cmp, PACK(noex _body))

#define DECL_SUPER_DEFAULT_CONSTRUCTOR(_name, _body) \
    DECL_DEFAULT_CONSTRUCTOR(_name, , PACK(_body))

#define DECL_SUB_DEFAULT_CONSTRUCTOR(_name, _body) \
    DECL_DEFAULT_CONSTRUCTOR(_name, , PACK(_body))

#define DEF_DEFAULT_DEFAULT_CONSTRUCTOR(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, DEFAULT_CONSTRUCTOR)                         \
    (_name, DEFAULT_METHOD)

#define DEF_AUTO_DEFAULT_CONSTRUCTOR(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, DEFAULT_CONSTRUCTOR)                      \
    (_name, DEF_DEFAULT_CONSTRUCT_AUTO(_construct, _copy, _move))

#define DEF_MANUAL_DEFAULT_CONSTRUCTOR(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, DEFAULT_CONSTRUCTOR)                        \
    (_name, DEF_DEFAULT_CONSTRUCT_MANUAL(_construct, _copy, _move))

#define DECL_COPY_MOVE_CONSTRUCTORS(_name, _pre, _post_copy, _post_move)          \
    RETURN_ATTRIBUTES _pre _name(OBJECT_ATTRIBUTES const _name &other) _post_copy \
        RETURN_ATTRIBUTES _pre                                                    \
        _name(OBJECT_ATTRIBUTES _name &&other) _post_move /* NOLINT(performance-noex-move-constructor) */

#define DECL_DELETED_COPY_MOVE_CONSTRUCTORS(_name) \
    DECL_COPY_MOVE_CONSTRUCTORS(_name, , DELETE_METHOD, DELETE_METHOD)

#define DECL_NO_ATTRIBUTE_COPY_MOVE_CONSTRUCTORS(_name, _copy_body, _move_body) \
    DECL_COPY_MOVE_CONSTRUCTORS(_name, , PACK(_copy_body), PACK(_move_body))

#define DECL_NOEX_COPY_MOVE_CONSTRUCTORS(_name, _copy_body, _move_body) \
    DECL_COPY_MOVE_CONSTRUCTORS(_name, , PACK(noex _copy_body), PACK(noex _move_body))

#define DECL_CMP_COPY_MOVE_CONSTRUCTORS(_name, _copy_body, _move_body) \
    DECL_COPY_MOVE_CONSTRUCTORS(_name, inl cmp, PACK(noex _copy_body), PACK(noex _move_body))

#define DECL_SUPER_COPY_MOVE_CONSTRUCTORS(_name, _copy_body, _move_body) \
    DECL_COPY_MOVE_CONSTRUCTORS(_name, , PACK(_copy_body), PACK(_move_body))

#define DECL_SUB_COPY_MOVE_CONSTRUCTORS(_name, _copy_body, _move_body) \
    DECL_COPY_MOVE_CONSTRUCTORS(_name, , PACK(_copy_body), PACK(_move_body))

#define DEF_DEFAULT_COPY_MOVE_CONSTRUCTORS(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, COPY_MOVE_CONSTRUCTORS)                         \
    (_name, DEFAULT_METHOD, DEFAULT_METHOD)

#define DEF_AUTO_COPY_MOVE_CONSTRUCTORS(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, COPY_MOVE_CONSTRUCTORS)                      \
    (_name, DEF_COPY_AUTO(_construct, _copy, _move), DEF_MOVE_AUTO(_construct, _copy, _move))

#define DEF_MANUAL_COPY_MOVE_CONSTRUCTORS(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, COPY_MOVE_CONSTRUCTORS)                        \
    (_name, DEF_COPY_MANUAL(_construct, _copy, _move), DEF_MOVE_MANUAL(_construct, _copy, _move))

#define DECL_ASSIGNMENT_OPERATORS(_name, _pre, _post_copy, _post_move)                  \
    RETURN_ATTRIBUTES _pre _name &op = (OBJECT_ATTRIBUTES const _name &other)_post_copy \
        RETURN_ATTRIBUTES _pre _name &op = (OBJECT_ATTRIBUTES _name && other) _post_move /* NOLINT(performance-noex-move-constructor) */

#define DECL_DELETED_ASSIGNMENT_OPERATORS(_name) \
    DECL_ASSIGNMENT_OPERATORS(_name, , DELETE_METHOD, DELETE_METHOD)

#define DECL_NO_ATTRIBUTE_ASSIGNMENT_OPERATORS(_name, _copy_body, _move_body) \
    DECL_ASSIGNMENT_OPERATORS(_name, , PACK(_copy_body), PACK(_move_body))

#define DECL_NOEX_ASSIGNMENT_OPERATORS(_name, _copy_body, _move_body) \
    DECL_ASSIGNMENT_OPERATORS(_name, , PACK(noex _copy_body), PACK(noex _move_body))

#define DECL_CMP_ASSIGNMENT_OPERATORS(_name, _copy_body, _move_body) \
    DECL_ASSIGNMENT_OPERATORS(_name, inl, PACK(noex _copy_body), PACK(noex _move_body))

#define DECL_SUPER_ASSIGNMENT_OPERATORS(_name, _copy_body, _move_body) \
    DECL_ASSIGNMENT_OPERATORS(_name, , PACK(_copy_body), PACK(_move_body))

#define DECL_SUB_ASSIGNMENT_OPERATORS(_name, _copy_body, _move_body) \
    DECL_ASSIGNMENT_OPERATORS(_name, , PACK(_copy_body), PACK(_move_body))

#define DEF_DEFAULT_ASSIGNMENT_OPERATORS(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, ASSIGNMENT_OPERATORS)                         \
    (_name, DEFAULT_METHOD, DEFAULT_METHOD)

#define DEF_AUTO_ASSIGNMENT_OPERATORS(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, ASSIGNMENT_OPERATORS)                      \
    (_name, DEFAULT_METHOD, DEFAULT_METHOD)

#define DEF_MANUAL_ASSIGNMENT_OPERATORS(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, ASSIGNMENT_OPERATORS)                        \
    (_name, DEF_OPERATOR_COPY_MOVE(_construct, _copy, _move), DEF_OPERATOR_MOVE(_construct, _copy, _move))

#define DECL_DESTRUCTOR(_name, _pre, _post) \
    NO_RETURN_ATTRIBUTES _pre ~_name() _post

#define DECL_DELETED_DESTRUCTOR(_name) \
    DECL_DESTRUCTOR(_name, , DELETE_METHOD)

#define DECL_NO_ATTRIBUTE_DESTRUCTOR(_name, _body) \
    DECL_DESTRUCTOR(_name, , PACK(_body))

#define DECL_NOEX_DESTRUCTOR(_name, _body) \
    DECL_DESTRUCTOR(_name, , PACK(noex _body))

#define DECL_CMP_DESTRUCTOR(_name, _body) \
    DECL_DESTRUCTOR(_name, inl, PACK(noex _body))

#define DECL_SUPER_DESTRUCTOR(_name, _body) \
    DECL_DESTRUCTOR(_name, virtual, PACK(_body))

#define DECL_SUB_DESTRUCTOR(_name, _body) \
    DECL_DESTRUCTOR(_name, , PACK(override _body))

#define DEF_DEFAULT_DESTRUCTOR(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, DESTRUCTOR)                         \
    (_name, DEFAULT_METHOD)

#define DEF_AUTO_DESTRUCTOR(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, DESTRUCTOR)                      \
    (_name, DEFAULT_METHOD)

#define DEF_MANUAL_DESTRUCTOR(_name, _attribute, _construct, _copy, _move) \
    WRAP_LIFETIME_ATTRIBUTE(_attribute, DESTRUCTOR)                        \
    (_name, DEF_DESTRUCT(_construct, _copy, _move))

ENV_CLANG_SUPPRESS_POP

// manual methods

#define DECL_LIFETIME_FUN(_name, _arguments, _pre, _post) \
    NO_RETURN_ATTRIBUTES _pre inl void _name _arguments _post

#define DECL_CONSTRUCT(_pre, _post) DECL_LIFETIME_FUN(_construct, (), _pre, _post)

#define DECL_NIL(_pre, _post) DECL_LIFETIME_FUN(_nil, (), _pre, _post)

#define DECL_COPY(_pre, _post) DECL_LIFETIME_FUN(_copy_move, ([[maybe_unused]] const _this_t &other), _pre, _post)
#define DECL_MOVE(_pre, _post) DECL_LIFETIME_FUN(_copy_move, ([[maybe_unused]] _this_t && other), _pre, _post)

#define DEF_MOVE(_pre, _post)    \
    DECL_MOVE(_pre, _post)       \
    {                            \
        this->_copy_move(other); \
        other._nil();            \
    }
#define DECL_COPY_MOVE(_pre, _post) \
    DEF_MOVE(_pre, _post)           \
    DECL_COPY(_pre, _post)

#define DECL_DESTRUCT(_pre, _post) DECL_LIFETIME_FUN(_destruct, (), _pre, _post)

// lifetime

// default

#define ELABORATE_DEFAULT_LIFE(_name, _constructor_attribute, _assignment_attribute, _destructor_attribute) \
    ACCESS_BEGIN(public);                                                                                   \
    DEF_DEFAULT_DEFAULT_CONSTRUCTOR(_name, _constructor_attribute, (), (), ())                              \
    DEF_DEFAULT_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, (), (), ())                           \
    DEF_DEFAULT_ASSIGNMENT_OPERATORS(_name, _assignment_attribute, (), (), ())                              \
    DEF_DEFAULT_DESTRUCTOR(_name, _destructor_attribute, (), (), ())                                        \
    SEMI

#define ELABORATE_DEFAULT_NOCON_LIFE(_name, _constructor_attribute, _assignment_attribute, _destructor_attribute) \
    ACCESS_BEGIN(public);                                                                                         \
    DEF_DEFAULT_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, (), (), ())                                 \
    DEF_DEFAULT_ASSIGNMENT_OPERATORS(_name, _assignment_attribute, (), (), ())                                    \
    DEF_DEFAULT_DESTRUCTOR(_name, _destructor_attribute, (), (), ())                                              \
    SEMI

#define DEFAULT_LIFE(_name, _attribute) \
    ELABORATE_DEFAULT_LIFE(_name, _attribute, _attribute, _attribute)

#define DEFAULT_NOCON_LIFE(_name, _attribute) \
    ELABORATE_DEFAULT_NOCON_LIFE(_name, _attribute, _attribute, _attribute)

ENV_TEST_CASE("default lifetime")
{
    SUBCASE("construct")
    {
        strct test
        {
            DEFAULT_LIFE(test, NO_ATTRIBUTE);
        };

        REQUIRE(ENV_STD::is_default_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_constructible_v<test>);
        REQUIRE(ENV_STD::is_move_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_assignable_v<test>);
        REQUIRE(ENV_STD::is_move_assignable_v<test>);
        REQUIRE(ENV_STD::is_destructible_v<test>);
    }

    SUBCASE("no construct")
    {
        strct test
        {
            DECL((const char *), member);

            DEFAULT_NOCON_LIFE(test, NOEX);
        };

        REQUIRE(!ENV_STD::is_default_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_constructible_v<test>);
        REQUIRE(ENV_STD::is_move_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_assignable_v<test>);
        REQUIRE(ENV_STD::is_move_assignable_v<test>);
        REQUIRE(ENV_STD::is_destructible_v<test>);
    }
}

// auto

#define ELABORATE_AUTO_LIFE(                                                                               \
    _name, _constructor_attribute, _assignment_attribute, _destructor_attribute, _construct, _copy, _move) \
    ACCESS_BEGIN(public);                                                                                  \
    DEF_AUTO_DEFAULT_CONSTRUCTOR(_name, _constructor_attribute, _construct, _copy, _move)                  \
    DEF_AUTO_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, _construct, _copy, _move)               \
    DEF_AUTO_ASSIGNMENT_OPERATORS(_name, _assignment_attribute, _construct, _copy, _move)                  \
    DEF_AUTO_DESTRUCTOR(_name, _destructor_attribute, _construct, _copy, _move)                            \
    SEMI

#define ELABORATE_AUTO_NOCON_LIFE(                                                             \
    _name, _constructor_attribute, _assignment_attribute, _destructor_attribute, _copy, _move) \
    ACCESS_BEGIN(public);                                                                      \
    DEF_AUTO_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, (), _copy, _move)           \
    DEF_AUTO_ASSIGNMENT_OPERATORS(_name, _assignment_attribute, (), _copy, _move)              \
    DEF_AUTO_DESTRUCTOR(_name, _destructor_attribute, (), _copy, _move)                        \
    SEMI

#define AUTO_LIFE(_name, _attribute, _construct, _copy, _move) \
    ELABORATE_AUTO_LIFE(_name, _attribute, _attribute, _attribute, _construct, _copy, _move)

#define AUTO_NOCON_LIFE(_name, _attribute, _copy, _move) \
    ELABORATE_AUTO_NOCON_LIFE(_name, _attribute, _attribute, _attribute, _copy, _move)

ENV_TEST_CASE("auto lifetime")
{
    SUBCASE("construct")
    {
        strct test
        {
            DECL((const char *), member);

            AUTO_LIFE(
                test, CMP,
                (
                    : _member{"default"}),
                (
                    : _member{"copied"}),
                (
                    : _member{"moved"}));

            MEM_GETTER(member);
        };

        REQUIRE(ENV_STD::is_default_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_constructible_v<test>);
        REQUIRE(ENV_STD::is_move_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_assignable_v<test>);
        REQUIRE(ENV_STD::is_move_assignable_v<test>);
        REQUIRE(ENV_STD::is_destructible_v<test>);

        test a{};
        REQUIRE_EQ(a.get_member(), "default");
        test b{a};
        REQUIRE_EQ(b.get_member(), "copied");
        test c{ENV_STD::move(b)};
        REQUIRE_EQ(c.get_member(), "moved");
    }

    SUBCASE("no construct")
    {
        strct test
        {
            DECL((const char *), member);

            cmp_con test(_member_t _member) : _member{ENV_STD::move(_member)} {}

            AUTO_NOCON_LIFE(
                test, CMP,
                (
                    : _member{"copied"}),
                (
                    : _member{"moved"}));

            MEM_GETTER(member);
        };

        REQUIRE(!ENV_STD::is_default_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_constructible_v<test>);
        REQUIRE(ENV_STD::is_move_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_assignable_v<test>);
        REQUIRE(ENV_STD::is_move_assignable_v<test>);
        REQUIRE(ENV_STD::is_destructible_v<test>);

        test a{"constructed"};
        REQUIRE_EQ(a.get_member(), "constructed");
        test b{a};
        REQUIRE_EQ(b.get_member(), "copied");
        test c{ENV_STD::move(b)};
        REQUIRE_EQ(c.get_member(), "moved");
    }
}

// manual

#define ELABORATE_MANUAL_LIFE(                                                                             \
    _name, _constructor_attribute, _assignment_attribute, _destructor_attribute, _construct, _copy, _move) \
    ACCESS_BEGIN(public);                                                                                  \
    DEF_MANUAL_DEFAULT_CONSTRUCTOR(_name, _constructor_attribute, _construct, _copy, _move)                \
    DEF_MANUAL_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, _construct, _copy, _move)             \
    DEF_MANUAL_ASSIGNMENT_OPERATORS(_name, _assignment_attribute, _construct, _copy, _move)                \
    DEF_MANUAL_DESTRUCTOR(_name, _destructor_attribute, _construct, _copy, _move)                          \
    SEMI

#define ELABORATE_MANUAL_NOCON_LIFE(                                                                       \
    _name, _constructor_attribute, _assignment_attribute, _destructor_attribute, _construct, _copy, _move) \
    ACCESS_BEGIN(public);                                                                                  \
    DEF_MANUAL_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, _construct, _copy, _move)             \
    DEF_MANUAL_ASSIGNMENT_OPERATORS(_name, _assignment_attribute, _construct, _copy, _move)                \
    DEF_MANUAL_DESTRUCTOR(_name, _destructor_attribute, _construct, _copy, _move)                          \
    SEMI

#define MANUAL_LIFE(_name, _attribute, _construct, _copy, _move) \
    ELABORATE_MANUAL_LIFE(_name, _attribute, _attribute, _attribute, _construct, _copy, _move)

#define MANUAL_NOCON_LIFE(_name, _attribute, _construct, _copy, _move) \
    ELABORATE_MANUAL_NOCON_LIFE(_name, _attribute, _attribute, _attribute, _construct, _copy, _move)

#define SIMPLE_MANUAL_LIFE(_name, _attribute) \
    MANUAL_LIFE(_name, _attribute, (), (), ())

#define SIMPLE_MANUAL_NOCON_LIFE(_name, _attribute) \
    MANUAL_NOCON_LIFE(_name, _attribute, (), (), ())

ENV_TEST_CASE("manual lifetime")
{
    SUBCASE("construct")
    {
        strct test
        {
            DECL_THIS(test);
            DECL((const char *), member);

            SIMPLE_MANUAL_LIFE(test, NOEX);

            MEM_GETTER(member);

        private:
            DECL_CONSTRUCT(, noex) { _member = "constructed"; }

            DECL_COPY(, noex) { _member = "copied"; }

            DECL_MOVE(, noex) { _member = "moved"; }

            DECL_DESTRUCT(, noex) { _member = "destructed"; }
        };

        REQUIRE(ENV_STD::is_default_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_constructible_v<test>);
        REQUIRE(ENV_STD::is_move_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_assignable_v<test>);
        REQUIRE(ENV_STD::is_move_assignable_v<test>);
        REQUIRE(ENV_STD::is_destructible_v<test>);

        test a{};
        REQUIRE_EQ(a.get_member(), "constructed");
        test b{a};
        REQUIRE_EQ(b.get_member(), "copied");
        test c{ENV_STD::move(b)};
        REQUIRE_EQ(c.get_member(), "moved");
        b.~test();
        REQUIRE_EQ(b.get_member(), "destructed");
    }

    SUBCASE("no construct")
    {
        strct test
        {
            DECL_THIS(test);
            DECL((const char *), member);

            cmp_con test(_member_t _member) : _member{ENV_STD::move(_member)} {}

            SIMPLE_MANUAL_NOCON_LIFE(test, NOEX);

            MEM_GETTER(member);

        private:
            DECL_CONSTRUCT(, noex) { _member = "constructed"; }

            DECL_NIL(, noex) { _member = "nil"; }

            DECL_COPY_MOVE(, noex) { _member = "copied"; }

            DECL_DESTRUCT(, noex) { _member = "destructed"; }
        };

        REQUIRE(!ENV_STD::is_default_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_constructible_v<test>);
        REQUIRE(ENV_STD::is_move_constructible_v<test>);
        REQUIRE(ENV_STD::is_copy_assignable_v<test>);
        REQUIRE(ENV_STD::is_move_assignable_v<test>);
        REQUIRE(ENV_STD::is_destructible_v<test>);

        test a{"constructed"};
        REQUIRE_EQ(a.get_member(), "constructed");
        test b{a};
        REQUIRE_EQ(b.get_member(), "copied");
        test c{ENV_STD::move(b)};
        REQUIRE_EQ(c.get_member(), "copied");
        REQUIRE_EQ(b.get_member(), "nil");
    }
}

// const lifetime

// default

#define ELABORATE_DEFAULT_CONST_LIFE(_name, _constructor_attribute, _destructor_attribute) \
    ACCESS_BEGIN(public);                                                                  \
    DEF_DEFAULT_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, (), (), ())          \
    DECL_DELETED_ASSIGNMENT_OPERATORS(_name)                                               \
    DEF_DEFAULT_DESTRUCTOR(_name, _destructor_attribute, (), (), ())                       \
    SEMI

#define DEFAULT_CONST_LIFE(_name, _attribute) \
    ELABORATE_DEFAULT_CONST_LIFE(_name, _attribute, _attribute)

ENV_TEST_CASE("const default lifetime")
{
    strct test
    {
        DECL((const char *), member);

        DEFAULT_CONST_LIFE(test, SUPER);
    };

    REQUIRE(!ENV_STD::is_default_constructible_v<test>);
    REQUIRE(ENV_STD::is_copy_constructible_v<test>);
    REQUIRE(ENV_STD::is_move_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_assignable_v<test>);
    REQUIRE(!ENV_STD::is_move_assignable_v<test>);
    REQUIRE(ENV_STD::is_destructible_v<test>);
}

// auto

#define ELABORATE_AUTO_CONST_LIFE(_name, _constructor_attribute, _destructor_attribute, _construct, _copy, _move) \
    ACCESS_BEGIN(public);                                                                                         \
    DEF_AUTO_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, _construct, _copy, _move)                      \
    DECL_DELETED_ASSIGNMENT_OPERATORS(_name)                                                                      \
    DEF_AUTO_DESTRUCTOR(_name, _destructor_attribute, _construct, _copy, _move)                                   \
    SEMI

#define AUTO_CONST_LIFE(_name, _attribute, _construct, _copy, _move) \
    ELABORATE_AUTO_CONST_LIFE(_name, _attribute, _attribute, _construct, _copy, _move)

ENV_TEST_CASE("const auto lifetime")
{
    strct test
    {
        DECL((const char *), member);

        cmp_con test(_member_t _member) : _member{ENV_STD::move(_member)} {}

        AUTO_CONST_LIFE(
            test, CMP,
            (
                : _member{"default"}),
            (
                : _member{"copied"}),
            (
                : _member{"moved"}));

        MEM_GETTER(member);
    };

    REQUIRE(!ENV_STD::is_default_constructible_v<test>);
    REQUIRE(ENV_STD::is_copy_constructible_v<test>);
    REQUIRE(ENV_STD::is_move_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_assignable_v<test>);
    REQUIRE(!ENV_STD::is_move_assignable_v<test>);
    REQUIRE(ENV_STD::is_destructible_v<test>);

    test a{"constructed"};
    REQUIRE_EQ(a.get_member(), "constructed");
    test b{a};
    REQUIRE_EQ(b.get_member(), "copied");
    test c{ENV_STD::move(b)};
    REQUIRE_EQ(c.get_member(), "moved");
}

// manual

#define ELABORATE_MANUAL_CONST_LIFE(_name, _constructor_attribute, _destructor_attribute, _construct, _copy, _move) \
    ACCESS_BEGIN(public);                                                                                           \
    DEF_MANUAL_COPY_MOVE_CONSTRUCTORS(_name, _constructor_attribute, _construct, _copy, _move)                      \
    DECL_DELETED_ASSIGNMENT_OPERATORS(_name)                                                                        \
    DEF_MANUAL_DESTRUCTOR(_name, _destructor_attribute, _construct, _copy, _move)                                   \
    SEMI

#define MANUAL_CONST_LIFE(_name, _attribute, _construct, _copy, _move) \
    ELABORATE_MANUAL_CONST_LIFE(_name, _attribute, _attribute, _construct, _copy, _move)

#define SIMPLE_MANUAL_CONST_LIFE(_name, _attribute) \
    MANUAL_CONST_LIFE(_name, _attribute, (), (), ())

ENV_TEST_CASE("const manual lifetime")
{
    strct test
    {
        DECL_THIS(test);
        DECL((const char *), member);

        cmp_con test(_member_t _member) : _member{ENV_STD::move(_member)} {}

        SIMPLE_MANUAL_CONST_LIFE(test, NOEX);

        MEM_GETTER(member);

    private:
        DECL_CONSTRUCT(, noex) { _member = "constructed"; }

        DECL_NIL(, noex) { _member = "nil"; }

        DECL_COPY_MOVE(, noex) { _member = "copied"; }

        DECL_DESTRUCT(, noex) { _member = "destructed"; }
    };

    REQUIRE(!ENV_STD::is_default_constructible_v<test>);
    REQUIRE(ENV_STD::is_copy_constructible_v<test>);
    REQUIRE(ENV_STD::is_move_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_assignable_v<test>);
    REQUIRE(!ENV_STD::is_move_assignable_v<test>);
    REQUIRE(ENV_STD::is_destructible_v<test>);

    test a{"constructed"};
    REQUIRE_EQ(a.get_member(), "constructed");
    test b{a};
    REQUIRE_EQ(b.get_member(), "copied");
    test c{ENV_STD::move(b)};
    REQUIRE_EQ(c.get_member(), "copied");
    REQUIRE_EQ(b.get_member(), "nil");
}

// singleton lifetime

// default

#define ELABORATE_DEFAULT_SINGLE_LIFE(_name, _constructor_attribute, _destructor_attribute) \
    ACCESS_BEGIN(public);                                                                   \
    DECL_DELETED_COPY_MOVE_CONSTRUCTORS(_name)                                              \
    DECL_DELETED_ASSIGNMENT_OPERATORS(_name)                                                \
protected:                                                                                  \
    DEF_DEFAULT_DEFAULT_CONSTRUCTOR(_name, _constructor_attribute, (), (), ())              \
    DEF_DEFAULT_DESTRUCTOR(_name, _destructor_attribute, (), (), ())                        \
    ACCESS_END(public)

#define DEFAULT_SINGLE_LIFE(_name, _attribute) \
    ELABORATE_DEFAULT_SINGLE_LIFE(_name, _attribute, _attribute)

ENV_TEST_CASE("default singleton lifetime")
{
    strct test
    {
        DECL_THIS(test);
        DEF((const char *), member, ("default"));

        DEFAULT_SINGLE_LIFE(test, SUPER);

        SINGLETON_GETTER();

        MEM_GETTER(member);
    };

    REQUIRE(!ENV_STD::is_default_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_constructible_v<test>);
    REQUIRE(!ENV_STD::is_move_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_assignable_v<test>);
    REQUIRE(!ENV_STD::is_move_assignable_v<test>);
    REQUIRE(!ENV_STD::is_destructible_v<test>);

    REQUIRE_EQ(test::instance().get_member(), "default");
}

// auto

#define ELABORATE_AUTO_SINGLE_LIFE(_name, _constructor_attribute, _destructor_attribute, _construct) \
    ACCESS_BEGIN(public);                                                                            \
    DECL_DELETED_COPY_MOVE_CONSTRUCTORS(_name)                                                       \
    DECL_DELETED_ASSIGNMENT_OPERATORS(_name)                                                         \
protected:                                                                                           \
    DEF_AUTO_DEFAULT_CONSTRUCTOR(_name, _constructor_attribute, _construct, (), ())                  \
    DEF_AUTO_DESTRUCTOR(_name, _destructor_attribute, _construct, (), ())                            \
    ACCESS_END(public)

#define AUTO_SINGLE_LIFE(_name, _attribute, _construct) \
    ELABORATE_AUTO_SINGLE_LIFE(_name, _attribute, _attribute, _construct)

ENV_TEST_CASE("singleton auto lifetime")
{
    strct test
    {
        DECL_THIS(test);
        DECL((const char *), member);

        AUTO_SINGLE_LIFE(test, CMP, (
                                        : _member{"default"}));

        SINGLETON_GETTER();

        MEM_GETTER(member);
    };

    REQUIRE(!ENV_STD::is_default_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_constructible_v<test>);
    REQUIRE(!ENV_STD::is_move_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_assignable_v<test>);
    REQUIRE(!ENV_STD::is_move_assignable_v<test>);
    REQUIRE(!ENV_STD::is_destructible_v<test>);

    REQUIRE_EQ(test::instance().get_member(), "default");
}

// manual

#define ELABORATE_MANUAL_SINGLE_LIFE(_name, _constructor_attribute, _destructor_attribute, _construct) \
    ACCESS_BEGIN(public);                                                                              \
    DECL_DELETED_COPY_MOVE_CONSTRUCTORS(_name)                                                         \
    DECL_DELETED_ASSIGNMENT_OPERATORS(_name)                                                           \
protected:                                                                                             \
    DEF_MANUAL_DEFAULT_CONSTRUCTOR(_name, _constructor_attribute, _construct, (), ())                  \
    DEF_MANUAL_DESTRUCTOR(_name, _destructor_attribute, _construct, (), ())                            \
    ACCESS_END(public)

#define MANUAL_SINGLE_LIFE(_name, _attribute, _construct) \
    ELABORATE_MANUAL_SINGLE_LIFE(_name, _attribute, _attribute, _construct)

#define SIMPLE_MANUAL_SINGLE_LIFE(_name, _attribute) \
    MANUAL_SINGLE_LIFE(_name, _attribute, ())

ENV_TEST_CASE("singleton manual lifetime")
{
    strct test
    {
        DECL_THIS(test);
        DECL((const char *), member);

        SIMPLE_MANUAL_SINGLE_LIFE(test, NOEX);

        SINGLETON_GETTER();

        MEM_GETTER(member);

    private:
        DECL_CONSTRUCT(, noex) { _member = "constructed"; }

        DECL_DESTRUCT(, noex) { _member = "destructed"; }
    };

    REQUIRE(!ENV_STD::is_default_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_constructible_v<test>);
    REQUIRE(!ENV_STD::is_move_constructible_v<test>);
    REQUIRE(!ENV_STD::is_copy_assignable_v<test>);
    REQUIRE(!ENV_STD::is_move_assignable_v<test>);
    REQUIRE(!ENV_STD::is_destructible_v<test>);

    REQUIRE_EQ(test::instance().get_member(), "constructed");
}

// subclass test

ENV_NAMESPACE_TEST_BEGIN

cls poly_base_t
{
    DECL_THIS(poly_base_t);
    DEF((const char *), poly_data, ("poly_base"));

    DEFAULT_LIFE(poly_base_t, SUPER);

    MEM_VAL_GETTER(poly_data);
};

ENV_NAMESPACE_TEST_END

ENV_TEST_CASE("subclass lifetime")
{
    strct subclass_t : public test::poly_base_t
    {
        DECL_THIS(subclass_t);

        MANUAL_LIFE(
            subclass_t,
            SUB,
            (
                : poly_base_t{}),
            (
                : poly_base_t{other}),
            (
                : poly_base_t{ENV_STD::move(other)}));

    protected:
        DECL_CONSTRUCT(, noex) { this->_get_poly_data() = "subclass"; }

        DECL_NIL(, noex) { this->_get_poly_data() = "nil"; }

        DECL_COPY_MOVE(, noex) { this->_get_poly_data() = "copied"; }

        DECL_DESTRUCT(, noex) { this->_get_poly_data() = "destructed"; }
    };

    subclass_t sub_constructed{};
    REQUIRE_EQ(sub_constructed.get_poly_data(), "subclass");
    subclass_t sub_copied = sub_constructed;
    REQUIRE_EQ(sub_copied.get_poly_data(), "copied");
    subclass_t sub_moved = ENV_STD::move(sub_constructed);
    REQUIRE_EQ(sub_moved.get_poly_data(), "copied");
    REQUIRE_EQ(sub_constructed.get_poly_data(), "nil");
}

#endif // ENV_LIFETIME_HPP
