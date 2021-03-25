#ifndef ENV_KEYWORDS_HPP
#define ENV_KEYWORDS_HPP


// keywords

#define tmp template
#define name typename
#if ENV_CPP >= 17
#define sass static_assert
#define sass_msg static_assert
#else // ENV_CPP >= 17
#define sass(...) static_assert(__VA_ARGS__, STRING(Assertion #__VA_ARGS__ failed.))
#define sass_msg static_assert
#endif // ENV_CPP >= 17

#if ENV_CPP >= 17
#define nonced [[maybe_unused]]
#else
#define nonced
#endif
#define nonce static_cast<void>
#define noop nonce(0)

#define cmp constexpr
#define inl inline
#define noex noexcept
#define noexpr(...) noex(noex(nonce(__VA_ARGS__)))
#if ENV_CPP >= 17
#define if_cmp if cmp
#else // ENV_CPP >= 17
#define if_cmp if
#endif // ENV_CPP >= 17

#define op operator
#if ENV_CPP >= 14
#define deduc decltype(auto)
#else // ENV_CPP >= 14
#define deduc auto
#endif // ENV_CPP >= 14
#define decl decltype
#define ret return

#define scast static_cast
#define rcast reinterpret_cast
#define ccast const_cast
#define dcast dynamic_cast

ENV_TEST_BEGIN

tmp<name T>
struct [[maybe_unused]] template_name_test_t
{
};

ENV_TEST_END

ENV_TEST_CASE("keyword keywords")
{
    struct [[TYPE_ATTRIBUTES]] test
    {
        [[NO_RETURN_ATTRIBUTES]] inl void op()(int b = 1) noex
        {
            [[OBJECT_ATTRIBUTES]] cmp decl(1) a{ };
            nonce(a);

            nonce(noop, b);
            if_cmp(true) ret;
        }
    };

    [[OBJECT_ATTRIBUTES]] auto _i = 1;
    [[OBJECT_ATTRIBUTES]] const auto _scast = scast<int>(_i);
    [[OBJECT_ATTRIBUTES]] const auto _ccast = ccast<const int&>(_scast);
    [[OBJECT_ATTRIBUTES]] const auto _rcast = rcast<const ENV_STD::byte*>(&_ccast);
    nonce(_rcast);

    class [[TYPE_ATTRIBUTES]] base_t
    {
    };
    class [[TYPE_ATTRIBUTES]] derived_t : public base_t
    {
    } derived{ };
    nonced const auto _dcast = dcast<base_t&>(derived);
    nonce(_dcast);

    sass(true);
    sass_msg(true, "I'm true!");
}


// types

#define typ_p using
#define typ_a(_name, ...) typ_p _name [[__VA_ARGS__]]
#define typ(_name) typ_p _name [[TYPE_ATTRIBUTES]]

#define strct_p struct
#define strct strct_p [[TYPE_ATTRIBUTES]]

#define cls_p class
#define cls cls_p [[TYPE_ATTRIBUTES]]

#if ENV_CPP >= 17
#define enm_p enum struct
#define enm enm_p [[TYPE_ATTRIBUTES]]
#else // ENV_CPP >= 17
#define enm_p enum struct
#define enm enm_p
#endif // ENV_CPP >= 17

ENV_TEST_CASE("type keywords")
{
    strct s
    {
    };
    cls c { };
    enm e { };
    typ(t) = s;
    t a{ };
    static_cast<void>(a);
}


// objects

#define obj_p
#define obj [[OBJECT_ATTRIBUTES]] obj_p
#if ENV_CPP >= 17
#define cmp_obj_p obj_p inl cmp
#else // ENV_CPP >= 17
#define cmp_obj_p obj_p cmp
#endif // ENV_CPP >= 17
#define cmp_obj [[OBJECT_ATTRIBUTES]] cmp_obj_p

#define temp_p deduc
#define temp obj temp_p
#define mut_p auto
#define mut obj mut_p
#define let_p const mut_p
#define let obj let_p

#if ENV_CPP >= 17
#define let_cmp_p let_p inl cmp
#else // ENV_CPP >= 17
#define let_cmp_p let_p cmp
#endif // ENV_CPP >= 17
#define let_cmp obj let_cmp_p

ENV_TEST_CASE("object keywords")
{
    obj int d{1};
    nonce(d);
    temp c{2};
    nonce(c);
    let a{1};
    nonce(a);
    mut b{a};
    nonce(b);
}


// functions

#define imp_p
#define imp [[RETURN_ATTRIBUTES]] imp_p
#define cmp_imp_p inl cmp
#define cmp_imp [[RETURN_ATTRIBUTES]] cmp_imp_p
#define con_p explicit
#define con [[RETURN_ATTRIBUTES]] con_p
#define cmp_con_p explicit inl cmp
#define cmp_con [[RETURN_ATTRIBUTES]] cmp_con_p

#if ENV_CPP >= 14
#define fun_p auto
#else // ENV_CPP >= 14
#define fun_p
#endif // ENV_CPP >= 14
#define fun [[RETURN_ATTRIBUTES]] fun_p
#define cmp_fn_p fun_p inl cmp
#define cmp_fn [[RETURN_ATTRIBUTES]] cmp_fn_p

#if ENV_CPP >= 14
#define callb_p auto
#else // ENV_CPP >= 14
#define callb_p
#endif // ENV_CPP >= 14
#define callb [[NO_RETURN_ATTRIBUTES]] callb_p
#define cmp_callb_p callb_p inl cmp
#define cmp_callb [[NO_RETURN_ATTRIBUTES]] cmp_callb_p


#define dest_p
#define dest [[NO_RETURN_ATTRIBUTES]] dest_p
#if ENV_CPP >= 20
#define cmp_dest_p callb_p inl cmp
#define cmp_dest [[NO_RETURN_ATTRIBUTES]] cmp_callb_p
#else
#define cmp_dest_p dest_p
#define cmp_dest [[NO_RETURN_ATTRIBUTES]] cmp_dest_p
#endif

#define asgn_p
#define asgn [[NO_RETURN_ATTRIBUTES]] asgn_p

ENV_TEST_CASE("function keywords")
{
    strct my_struct
    {
        fun inl fi() { ret 1; }
        cmp_fn static fc() { ret 1; }

        callb static inl cb() -> void { }
        cmp_callb static cbc() -> void { }

        con inl op int() noex { ret 4; }
        cmp_con op double() { ret 4.0; }

        imp inl op char() { ret 4; }
        cmp_imp op float() { ret 4.0; }

        asgn my_struct& op=(const my_struct&) noex = default;
        cmp_dest ~my_struct() noex = default;
    };

    nonce(my_struct{ }.fi());
    nonce(my_struct::fc());
    nonce(my_struct::cbc());
    my_struct::cb();
}


#endif // ENV_KEYWORDS_HPP
