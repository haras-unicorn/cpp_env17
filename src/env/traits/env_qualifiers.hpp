#ifndef ENV_QUALIFIERS_HPP
#define ENV_QUALIFIERS_HPP


// TODO: optimize with intrinsics

// ref

COND_CHECK_UNARY(is_lvalue_ref, ENV_STD::is_lvalue_reference<T>);

COND_CHECK_UNARY(is_rvalue_ref, ENV_STD::is_rvalue_reference<T>);

COND_CHECK_UNARY(is_ref, ENV_STD::is_reference<T>);

enm ref_type_t{lvalue, rvalue, none};

template<name T>
let_cmp ref_type_g =
        is_lvalue_ref_g<T> ? ref_type_t::lvalue :
        is_rvalue_ref_g<T> ? ref_type_t::rvalue :
                             ref_type_t::none;

template<name T>
typ(remove_ref_gt) = ENV_STD::remove_reference_t<T>;

template<name T>
typ(add_lvalue_ref_gt) = ENV_STD::add_lvalue_reference_t<T>;

template<name T>
typ(add_rvalue_ref_gt) = ENV_STD::add_rvalue_reference_t<T>;

template<name T, ref_type_t RefType>
typ(set_ref_gnt) =
        ENV_STD::conditional_t<
                RefType == ref_type_t::lvalue,
                add_lvalue_ref_gt<remove_ref_gt<T>>,
                ENV_STD::conditional_t<
                        RefType == ref_type_t::rvalue,
                        add_rvalue_ref_gt<remove_ref_gt<T>>,
                        remove_ref_gt<T>>>;

template<name TFrom, name TTo>
typ(copy_ref_ggt) = set_ref_gnt<TTo, ref_type_g<TFrom>>;

ENV_TEST_CASE("ref qualifier")
{
    SUBCASE("check")
    {
        SUBCASE("lvalue")
        {
            REQUIRES(is_lvalue_ref_g<int&>);
            REQUIRES_FALSE(is_lvalue_ref_g<const int&&>);
        }

        SUBCASE("rvalue")
        {
            REQUIRES(is_rvalue_ref_g<int&&>);
            REQUIRES_FALSE(is_rvalue_ref_g<const int&>);
        }

        SUBCASE("ref")
        {
            REQUIRES(is_ref_g<int&&>);
            REQUIRES(is_ref_g<const int&>);
            REQUIRES_FALSE(is_ref_g<volatile int>);
        }

        SUBCASE("type")
        {
            REQUIRE_EQ(ref_type_g<int&&>, ref_type_t::rvalue);
            REQUIRE_EQ(ref_type_g<const int&>, ref_type_t::lvalue);
            REQUIRE_EQ(ref_type_g<int>, ref_type_t::none);
        }
    }

    SUBCASE("remove")
    {
        REQUIRE_EQT(remove_ref_gt<int&>, int);
        REQUIRE_EQT(remove_ref_gt<const int&&>, const int);
        REQUIRE_EQT(remove_ref_gt<volatile int>, volatile int);
    }

    SUBCASE("add")
    {
        REQUIRE_EQT(add_lvalue_ref_gt<int>, int&);
        REQUIRE_EQT(add_rvalue_ref_gt<const int>, const int&&);
    }

    SUBCASE("set")
    {
        REQUIRE_EQT(set_ref_gnt<int&&, ref_type_t::lvalue>, int&);
        REQUIRE_EQT(set_ref_gnt<const int, ref_type_t::rvalue>, const int&&);
        REQUIRE_EQT(set_ref_gnt<volatile int&&, ref_type_t::none>, volatile int);
    }

    SUBCASE("copy")
    {
        REQUIRE_EQT(copy_ref_ggt<int&&, double>, double&&);
        REQUIRE_EQT(copy_ref_ggt<const int, volatile double&>, volatile double);
        REQUIRE_EQT(copy_ref_ggt<volatile int&, double>, double&);
    }
}


// const

COND_CHECK_UNARY(is_const, ENV_STD::is_const<remove_ref_gt<T>>);

template<name T>
typ(add_const_gt) = ENV_STD::add_const_t<T>;

template<name T>
typ(remove_const_gt) =
        set_ref_gnt<ENV_STD::remove_const_t<remove_ref_gt<T>>, ref_type_g<T>>;

template<name T, bool Condition>
typ(set_const_gnt) =
        ENV_STD::conditional_t<
                Condition,
                set_ref_gnt<ENV_STD::add_const_t<remove_ref_gt<T>>, ref_type_g<T>>,
                set_ref_gnt<ENV_STD::remove_const_t<remove_ref_gt<T>>, ref_type_g<T>>>;

template<name TFrom, name TTo>
typ(copy_const_ggt) = set_const_gnt<TTo, is_const_g<TFrom>>;

ENV_TEST_CASE("const qualifier")
{
    SUBCASE("check")
    {
        REQUIRES(is_const_g<const int&>);
        REQUIRES_FALSE(is_const_g<int&&>);
    }

    SUBCASE("remove")
    {
        REQUIRE_EQT(remove_const_gt<const int&>, int&);
        REQUIRE_EQT(remove_const_gt<int&&>, int&&);
    }

    SUBCASE("add")
    {
        REQUIRE_EQT(add_const_gt<int>, const int);
        REQUIRE_EQT(add_const_gt<const int>, const int);
    }

    SUBCASE("set")
    {
        REQUIRE_EQT(set_const_gnt<int&, true>, const int&);
        REQUIRE_EQT(set_const_gnt<const int&&, false>, int&&);
    }

    SUBCASE("copy")
    {
        REQUIRE_EQT(copy_const_ggt<int&&, const double>, double);
        REQUIRE_EQT(copy_const_ggt<const int&, double&&>, const double&&);
    }
}


// volatile

COND_CHECK_UNARY(is_volatile, ENV_STD::is_volatile<remove_ref_gt<T>>);

template<name T>
typ(remove_volatile_gt) =
        set_ref_gnt<ENV_STD::remove_volatile_t<remove_ref_gt<T>>, ref_type_g<T>>;

template<name T>
typ(add_volatile_gt) = ENV_STD::add_volatile_t<T>;

template<name T, bool Condition>
typ(set_volatile_gnt) =
        ENV_STD::conditional_t<
                Condition,
                set_ref_gnt<
                        ENV_STD::add_volatile_t<remove_ref_gt<T>>, ref_type_g<T>>,
                set_ref_gnt<
                        ENV_STD::remove_volatile_t<remove_ref_gt<T>>, ref_type_g<T>>>;

template<name TFrom, name TTo>
typ(copy_volatile_ggt) = set_volatile_gnt<TTo, is_volatile_g<TFrom>>;

ENV_TEST_CASE("volatile qualifier")
{
    SUBCASE("check")
    {
        REQUIRES(is_volatile_g<volatile int&>);
        REQUIRES_FALSE(is_volatile_g<int&&>);
    }

    SUBCASE("remove")
    {
        REQUIRE_EQT(remove_volatile_gt<volatile int&>, int&);
        REQUIRE_EQT(remove_volatile_gt<int&&>, int&&);
    }

    SUBCASE("add")
    {
        REQUIRE_EQT(add_volatile_gt<int>, volatile int);
        REQUIRE_EQT(add_volatile_gt<volatile int>, volatile int);
    }

    SUBCASE("set")
    {
        REQUIRE_EQT(set_volatile_gnt<int&, true>, volatile int&);
        REQUIRE_EQT(set_volatile_gnt<volatile int&&, false>, int&&);
    }

    SUBCASE("copy")
    {
        REQUIRE_EQT(copy_volatile_ggt<int&&, volatile double>, double);
        REQUIRE_EQT(copy_volatile_ggt<volatile int&, double&&>, volatile double&&);
    }
}


// cv

template<name T>
typ(remove_cv_gt) =
        set_ref_gnt<ENV_STD::remove_cv_t<remove_ref_gt<T>>, ref_type_g<T>>;

template<name T, bool Const, bool Volatile>
typ(set_cv_gnnt) = set_volatile_gnt<set_const_gnt<T, Const>, Volatile>;

template<name TFrom, name TTo>
typ(copy_cv_ggt) = set_cv_gnnt<TTo, is_const_g<TFrom>, is_volatile_g<TFrom>>;

ENV_TEST_CASE("cv qualifiers")
{
    SUBCASE("remove")
    {
        REQUIRE_EQT(remove_cv_gt<const volatile int&&>, int&&);
        REQUIRE_EQT(remove_cv_gt<const int&>, int&);
        REQUIRE_EQT(remove_cv_gt<volatile int>, int);
    }
    SUBCASE("set")
    {
        REQUIRE_EQT(set_cv_gnnt<const volatile int&&, true, false>, const int&&);
        REQUIRE_EQT(set_cv_gnnt<const int&, false, true>, volatile int&);
        REQUIRE_EQT(set_cv_gnnt<volatile int, true, true>, const volatile int);
    }
    SUBCASE("copy")
    {
        REQUIRE_EQT(copy_cv_ggt<const double, const volatile int&&>,
                    const int&&);
        REQUIRE_EQT(copy_cv_ggt<volatile double&&, const int&>, volatile int&);
        REQUIRE_EQT(copy_cv_ggt<const volatile float&, volatile int>, const volatile int);
    }
}


// qualifiers

template<name T>
typ(remove_qualifiers_gt) =
        ENV_STD::remove_cv_t<ENV_STD::remove_reference_t<T>>;

template<name T, bool Const, bool Volatile, ref_type_t RefType>
typ(set_qualifiers_gnnnt) =
        set_cv_gnnt<set_ref_gnt<T, RefType>, Const, Volatile>;

template<name TFrom, name TTo>
typ(copy_qualifiers_ggt) =
        set_qualifiers_gnnnt<
                TTo,
                is_const_g<TFrom>,
                is_volatile_g<TFrom>,
                ref_type_g<TFrom>>;

ENV_TEST_CASE("all qualifiers")
{
    SUBCASE("remove")
    {
        REQUIRE_EQT(unqualified_gt<const volatile int&&>, int);
        REQUIRE_EQT(unqualified_gt<const int&>, int);
        REQUIRE_EQT(unqualified_gt<volatile int>, int);
    }
    SUBCASE("set")
    {
        REQUIRE_EQT(set_qualifiers_gnnnt<
                            const volatile int&&, true, false, ref_type_t::lvalue>,
                    const int&);
        REQUIRE_EQT(set_qualifiers_gnnnt<
                            const int&, false, true, ref_type_t::rvalue>,
                    volatile int&&);
        REQUIRE_EQT(set_qualifiers_gnnnt<
                            volatile int, true, true, ref_type_t::none>,
                    const volatile int);
    }
    SUBCASE("copy")
    {
        REQUIRE_EQT(copy_qualifiers_ggt<const double, const volatile int&&>,
                    const int);
        REQUIRE_EQT(copy_qualifiers_ggt<volatile double&&, const int&>,
                    volatile int&&);
        REQUIRE_EQT(copy_qualifiers_ggt<const volatile float&, volatile int>,
                    const volatile int&);
    }
}


// qualified for

EXPR_CHECK_BINARY(is_qualified_for, hoard<TRhs>(declval<TLhs>()));

ENV_TEST_CASE("qualified for")
{
    strct test_t{};

    REQUIRES(is_qualified_for_g<const test_t, const volatile test_t>);
    REQUIRES(is_qualified_for_g<const test_t, test_t>);
    // oh, MSVC, why, oh, why are you so bad...
    //    REQUIRES
    //    (is_qualified_for_g<volatile test_t, test_t>);
    //    REQUIRES
    //    (is_qualified_for_g<const volatile test_t, const test_t>);
    //    REQUIRES
    //    (is_qualified_for_g<volatile test_t, volatile test_t>);
    //    REQUIRES
    //    (is_qualified_for_g<const volatile test_t, volatile test_t>);
    REQUIRES(is_qualified_for_g<test_t&, test_t>);
    REQUIRES(is_qualified_for_g<test_t&&, test_t>);
    REQUIRES_FALSE(is_qualified_for_g<volatile test_t&&, test_t>);
    REQUIRES(is_qualified_for_g<volatile test_t&&, volatile test_t&&>); // forwarded
    REQUIRES_FALSE(is_qualified_for_g<test_t, test_t&>);
    REQUIRES(is_qualified_for_g<test_t, test_t&&>);
    REQUIRES(is_qualified_for_g<test_t&, test_t&>);
    REQUIRES_FALSE(is_qualified_for_g<test_t&&, test_t&>);
    REQUIRES(is_qualified_for_g<test_t&&, const test_t&>);
    REQUIRES_FALSE(is_qualified_for_g<test_t&&, const volatile test_t&>);
    REQUIRES_FALSE(is_qualified_for_g<volatile test_t&&, const test_t&>);
    REQUIRES(is_qualified_for_g<const test_t&&, const test_t&>);
}

// like is_qualified_for, but uses reference collapsing rules of && for both sides
EXPR_CHECK_BINARY(is_forward_qualified_for, consume<TRhs>(declvalr<TLhs>()));

ENV_TEST_CASE("forward qualified for")
{
    strct test_t
    {
        test_t(const test_t&) = delete;
        test_t(test_t &&) = delete;
    };

    REQUIRES(is_forward_qualified_for_g<const test_t, const volatile test_t>);
    REQUIRES_FALSE(is_forward_qualified_for_g<const test_t, test_t>);
    REQUIRES_FALSE(is_forward_qualified_for_g<volatile test_t, test_t>);
    REQUIRES_FALSE(is_forward_qualified_for_g<const volatile test_t, const test_t>);
    REQUIRES(is_forward_qualified_for_g<volatile test_t, volatile test_t>);
    REQUIRES_FALSE(is_forward_qualified_for_g<const volatile test_t, volatile test_t>);
    REQUIRES_FALSE(is_forward_qualified_for_g<test_t&, test_t>);
    REQUIRES(is_forward_qualified_for_g<test_t&&, test_t>);
    REQUIRES_FALSE(is_forward_qualified_for_g<volatile test_t&&, test_t>);
    REQUIRES(is_forward_qualified_for_g<volatile test_t&&, volatile test_t&&>);
    REQUIRES_FALSE(is_forward_qualified_for_g<volatile test_t&&, const test_t>);
    REQUIRES(is_forward_qualified_for_g<test_t&&, const test_t&&>);
    REQUIRES_FALSE(is_forward_qualified_for_g<test_t, test_t&>);
    REQUIRES(is_forward_qualified_for_g<test_t, test_t&&>);
    REQUIRES(is_forward_qualified_for_g<test_t&, test_t&>);
    REQUIRES_FALSE(is_forward_qualified_for_g<test_t&&, test_t&>);
    REQUIRES(is_forward_qualified_for_g<test_t&&, const test_t&>);
    REQUIRES_FALSE(is_forward_qualified_for_g<test_t&&, const volatile test_t&>);
    REQUIRES_FALSE(is_forward_qualified_for_g<volatile test_t&&, const test_t&>);
    REQUIRES(is_forward_qualified_for_g<const test_t&&, const test_t&>);
}

// checks if lhs is qualified for a call of a function with the qualifiers of the rhs
COND_CHECK_BINARY(
        is_call_qualified_for,
        cond_gggt<is_ref_gs<TRhs>,
                  is_forward_qualified_for_gs<TLhs, TRhs>,
                  is_forward_qualified_for_gs<remove_ref_gt<TLhs>, TRhs>>);

ENV_TEST_CASE("call qualified for")
{
    strct test_t
    {
        test_t(const test_t&) = delete;
        test_t(test_t &&) = delete;
    };

    // bug because it should check the required constructor regardless of elision
    // https://stackoverflow.com/questions/20589622/copy-move-elision-versus-explicitly-deleted-copy-move-constructors
    // test_t a(test_t{}); // copy elision!!

    REQUIRES(is_call_qualified_for_g<test_t, const test_t>);
    REQUIRES(is_call_qualified_for_g<const test_t, const test_t>);
    REQUIRES(is_call_qualified_for_g<volatile test_t, volatile test_t>);
    REQUIRES(is_call_qualified_for_g<volatile test_t, const volatile test_t>);
    REQUIRES(is_call_qualified_for_g<const test_t, const volatile test_t>);
    REQUIRES_FALSE(is_call_qualified_for_g<const test_t, test_t>);
    REQUIRES_FALSE(is_call_qualified_for_g<volatile test_t, test_t>);
    REQUIRES_FALSE(is_call_qualified_for_g<const volatile test_t, volatile test_t>);
    REQUIRES(is_call_qualified_for_g<test_t&, test_t>);
    REQUIRES_FALSE(is_call_qualified_for_g<test_t, test_t&>);
    REQUIRES(is_call_qualified_for_g<test_t&, const test_t>);
    REQUIRES(is_call_qualified_for_g<const test_t&&, const test_t>);
    REQUIRES(is_call_qualified_for_g<volatile test_t&, volatile test_t>);
    REQUIRES(is_call_qualified_for_g<volatile test_t&, const volatile test_t>);
    REQUIRES(is_call_qualified_for_g<const test_t&&, const volatile test_t>);
    REQUIRES(is_call_qualified_for_g<test_t&&, test_t>);
    REQUIRES_FALSE(is_call_qualified_for_g<test_t, test_t&>);
    REQUIRES(is_call_qualified_for_g<test_t, test_t&&>);
    REQUIRES(is_call_qualified_for_g<test_t&, test_t&>);
    REQUIRES_FALSE(is_call_qualified_for_g<test_t&&, test_t&>);
    REQUIRES(is_call_qualified_for_g<test_t&&, const test_t&>);
    REQUIRES_FALSE(is_call_qualified_for_g<test_t&&, const volatile test_t&>);
    REQUIRES_FALSE(is_call_qualified_for_g<volatile test_t&&, const test_t&>);
    REQUIRES(is_call_qualified_for_g<const test_t&&, const test_t&>);
}


#endif // ENV_QUALIFIERS_HPP
