#ifndef FWA_CORE_KEYWORDS_HPP
#define FWA_CORE_KEYWORDS_HPP


// keywords

#define tmp template
#define name typename
#if FWA_CPP >= 17
#define sass static_assert
#define sass_msg static_assert
#else // FWA_CPP >= 17
#define sass(...) static_assert(__VA_ARGS__, STRING(Assertion #__VA_ARGS__ failed.))
#define sass_msg static_assert
#endif // FWA_CPP >= 17

#define cmp constexpr
#define inl inline
#define noex noexcept
#if FWA_CPP >= 17
#define if_cmp if cmp
#else // FWA_CPP >= 17
#define if_cmp if
#endif // FWA_CPP >= 17

#define op operator
#if FWA_CPP >= 14
#define deduc decltype(auto)
#else // FWA_CPP >= 14
#define deduc auto
#endif // FWA_CPP >= 14
#define decl decltype
#define ret return

#define scast static_cast
#define rcast reinterpret_cast
#define ccast const_cast
#define dcast dynamic_cast
#define unused static_cast<void>

FWA_NAMESPACE_TEST_BEGIN

tmp<name T>
struct [[maybe_unused]] template_name_test_t { };

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("keyword keywords")
{
    struct TYPE_ATTRIBUTES test
    {
        NO_RETURN_ATTRIBUTES inl void op()(int b = 1) noex
        {
            unused(b);
            if_cmp(true) ret;

            OBJECT_ATTRIBUTES cmp decl(1) a{ };
            ret;
        }
    };

    OBJECT_ATTRIBUTES auto _i = 1;
    OBJECT_ATTRIBUTES const auto _scast = scast<double>(_i);
    OBJECT_ATTRIBUTES const auto _ccast = ccast<const int&>(_i);
    OBJECT_ATTRIBUTES const auto _rcast = rcast<FWA_STD::byte*>(&_i);

    class TYPE_ATTRIBUTES base_t { };
    class TYPE_ATTRIBUTES derived_t : public base_t { } derived{ };
    OBJECT_ATTRIBUTES const auto _dcast = dcast<base_t&>(derived);

    sass(true);
    sass_msg(true, "I'm true!");
}


// types

#define typ_impl(_name) using _name
#define typ_p(_name) typ_impl(_name)
#define typ(_name) typ_p(_name) TYPE_ATTRIBUTES

#define strct_p struct
#define strct strct_p TYPE_ATTRIBUTES

#define cls_p class
#define cls cls_p TYPE_ATTRIBUTES

#if FWA_CPP >= 17
#define enm_p enum struct
#define enm enm_p TYPE_ATTRIBUTES
#else // FWA_CPP >= 17
#define enm_p enum struct
#define enm enm_p
#endif // FWA_CPP >= 17

FWA_CORE_TEST_CASE("type keywords")
{
    strct s { };
    cls c { };
    enm e { };
    typ(fst) = s;
}


// objects

#define obj_p
#define obj OBJECT_ATTRIBUTES obj_p
#if FWA_CPP >= 17
#define cmp_obj_p obj_p inl cmp
#else // FWA_CPP >= 17
#define cmp_obj_p obj_p cmp
#endif // FWA_CPP >= 17
#define cmp_obj OBJECT_ATTRIBUTES cmp_obj_p

#define temp_p deduc
#define temp obj temp_p
#define mut_p auto
#define mut obj mut_p
#define let_p const mut_p
#define let obj let_p

#if FWA_CPP >= 17
#define let_cmp_p let_p inl cmp
#else // FWA_CPP >= 17
#define let_cmp_p let_p cmp
#endif // FWA_CPP >= 17
#define let_cmp obj let_cmp_p

FWA_CORE_TEST_CASE("object keywords")
{
    obj int d{1};
    temp c{2};
    let a{1};
    mut b{a};
}


// functions

#define imp_p
#define imp RETURN_ATTRIBUTES imp_p
#define cmp_imp_p inl cmp
#define cmp_imp RETURN_ATTRIBUTES cmp_imp_p
#define con_p explicit
#define con RETURN_ATTRIBUTES con_p
#define cmp_con_p explicit inl cmp
#define cmp_con RETURN_ATTRIBUTES cmp_con_p

#if FWA_CPP >= 14
#define fun_p auto
#else // FWA_CPP >= 14
#define fun_p
#endif // FWA_CPP >= 14
#define fun RETURN_ATTRIBUTES fun_p
#define cmp_fn_p fun_p inl cmp
#define cmp_fn RETURN_ATTRIBUTES cmp_fn_p

#if FWA_CPP >= 14
#define callb_p auto
#else // FWA_CPP >= 14
#define callb_p
#endif // FWA_CPP >= 14
#define callb NO_RETURN_ATTRIBUTES callb_p
#define cmp_callb_p callb_p inl cmp
#define cmp_callb NO_RETURN_ATTRIBUTES cmp_callb_p


FWA_CORE_TEST_CASE("function keywords")
{
    strct fs
    {
        fun inl fi() { ret 1; }
        cmp_fn static fc() { ret 1; }

        callb static inl cb() -> void { }
        cmp_callb static cbc() -> void { }

        con inl op int() noex { ret 4; }
        cmp_con op double() { ret 4.0; }

        imp inl op char() { ret 4; }
        cmp_imp op float() { ret 4.0; }
    };
}


#endif // FWA_CORE_KEYWORDS_HPP
