#ifndef ENV_NIL_HPP
#define ENV_NIL_HPP


// checks

ENV_DETAIL_BEGIN

COND_CHECK_UNARY(is_zero_constructible, ENV_STD::is_integral_v < T > || ENV_STD::is_floating_point_v < T >);

EXPR_CHECK_UNARY(is_nullptr_constructible, T(nullptr));

ENV_DETAIL_END


ENV_TEST_BEGIN

strct default_constructible_t { int i; };

ENV_TEST_END

TEST_CASE("nil checks")
{
    REQUIRES(detail::is_nullptr_constructible_g<int*>);
    REQUIRES_FALSE(detail::is_zero_constructible_g<int*>);

    REQUIRES_FALSE(detail::is_nullptr_constructible_g<int>);
    REQUIRES(detail::is_zero_constructible_g<int>);

    REQUIRES_FALSE(detail::is_nullptr_constructible_g<test::default_constructible_t>);
    REQUIRES_FALSE(detail::is_zero_constructible_g<test::default_constructible_t>);
    REQUIRES(ENV_STD::is_default_constructible_v < test::default_constructible_t >);
}


// nil init

strct nil_t
{
    ENV_CLANG_SUPPRESS_PUSH("google-explicit-constructor")

    COND_TMP_UNARY
    (
            detail::is_nullptr_constructible_g<T>
    )
    imp inl cmp op T() const noex
    {
        ret T(nullptr);
    }

    COND_TMP_UNARY
    (
            !detail::is_nullptr_constructible_g<T> &&
            detail::is_zero_constructible_g<T>
    )
    imp inl cmp op T() const noex { ret T(0); }

    COND_TMP_UNARY
    (
            !detail::is_nullptr_constructible_g<T> &&
            !detail::is_zero_constructible_g<T> &&
            ENV_STD::is_default_constructible_v < T >
    )
    imp inl cmp op T() const noex { ret T(); }

    ENV_CLANG_SUPPRESS_POP
};

let_cmp nil = nil_t{ };


ENV_TEST_CASE("nil init")
{
    cmp int* i_ptr{nil};
    REQUIRES(i_ptr == nullptr);

    cmp nullptr_t null_ptr{nil};
    REQUIRES(null_ptr == nullptr);

    cmp int i{nil};
    REQUIRES(i == 0);

    cmp test::default_constructible_t default_constructible(nil);
    REQUIRES(default_constructible.i == 0);
}


// nil equality

cmp_fn op==(nonced nil_t lhs, nonced nil_t rhs) noex { ret true; }

cmp_fn op!=(nonced nil_t lhs, nonced nil_t rhs) noex { ret false; }

ENV_TEST_CASE("nil equality")
{
    let eq_res = nil == nil;
    REQUIRES(eq_res);

    let ne_res = nil != nil;
    REQUIRES_FALSE(ne_res);
}


// nillable

COND_CHECK_UNARY(is_nillable, is_imp_convertible_g < nil_t, T >);

COND_CONCEPT(nillable, is_nillable_g<C>);


ENV_TEST_BEGIN

strct nillable_t { };

strct non_nillable_t { non_nillable_t() = delete; };

ENV_TEST_END

ENV_TEST_CASE("nil init")
{
    REQUIRES(is_nillable_g < int * >);
    REQUIRES(is_nillable_g < nullptr_t >);
    REQUIRES(is_nillable_g < int >);
    REQUIRES(is_nillable_g < test::nillable_t >);
    REQUIRES(!is_nillable_g < test::non_nillable_t >);
}


// validatable

ENV_DETAIL_BEGIN

COND_CHECK_UNARY
(
        is_strictly_validatable,
        (ENV_STD::is_same_v < decl(ENV_STD::forward<T>(declvalr<T>()).is_valid()), bool >)
);

ENV_DETAIL_END

COND_CHECK_UNARY
(
        is_validatable,
        detail::is_strictly_validatable_g<T> ||
        (is_equatable_g < T > && is_nillable_g<T>)
);

COND_CONCEPT(validatable, is_validatable_g<C>);


ENV_TEST_BEGIN

strct default_equatable_t
{
    int i;
    cmp_fn op==(default_equatable_t other) const noex { ret i == other.i; }
    cmp_fn op!=(default_equatable_t other) const noex { ret i != other.i; }
};

strct non_validatable_t { };

strct strictly_validatable_t
{
    int i;
    cmp_fn is_valid() const noex { ret i != 0; }
};

ENV_TEST_END

ENV_TEST_CASE("nil validatable")
{
    REQUIRES(is_validatable_g<int>);
    REQUIRES(is_validatable_g<nullptr_t>);
    REQUIRES(is_validatable_g<int*>);
    REQUIRES(is_validatable_g<test::strictly_validatable_t>);
    REQUIRES(is_validatable_g<const test::strictly_validatable_t&>);
    REQUIRES_FALSE(is_validatable_g<test::non_validatable_t>);
    REQUIRES_FALSE(is_validatable_g<test::non_nillable_t>);
}


// equatable

COND_TMP_UNARY(!detail::is_strictly_validatable_g<T>)
cmp_fn op==(equatable_c <nillable_c<T>>&& lhs, nil_t rhs) noex
{
    ret ENV_STD::forward<T>(lhs) == static_cast<unqualified_gt <T>>(rhs);
}

COND_TMP_UNARY(!detail::is_strictly_validatable_g<T>)
cmp_fn op==(nil_t lhs, equatable_c <nillable_c<T>>&& rhs) noex
{
    ret static_cast<unqualified_gt <T>>(lhs) == ENV_STD::forward<T>(rhs);
}

COND_TMP_UNARY(!detail::is_strictly_validatable_g<T>)
cmp_fn op!=(equatable_c <nillable_c<T>>&& lhs, nil_t rhs) noex
{
    ret ENV_STD::forward<T>(lhs) != static_cast<unqualified_gt <T>>(rhs);
}

COND_TMP_UNARY(!detail::is_strictly_validatable_g<T>)
cmp_fn op!=(nil_t lhs, equatable_c <nillable_c<T>>&& rhs) noex
{
    ret static_cast<unqualified_gt <T>>(lhs) != ENV_STD::forward<T>(rhs);
}

ENV_TEST_CASE("nil equatable")
{
    cmp int i = 1;
    const int* ptr = &i;
    cmp int* null_ptr{nullptr};
    cmp test::default_equatable_t equatable_nil(nil), equatable{1};

    SUBCASE("eq")
    {
        SUBCASE("lhs")
        {
            let cmp nullptr_res = nil == nullptr;
            let cmp null_ptr_res = nil == null_ptr;
            let cmp zero_res = nil == 0;
            let cmp n_trivial_res = nil == equatable_nil;

            REQUIRES(nullptr_res);
            REQUIRES(null_ptr_res);
            REQUIRES(zero_res);
            REQUIRES(n_trivial_res);
        }

        SUBCASE("rhs")
        {
            let cmp nullptr_res = nullptr == nil;
            let cmp null_ptr_res = null_ptr == nil;
            let cmp zero_res = 0 == nil;
            let cmp n_trivial_res = equatable_nil == nil;

            REQUIRES(nullptr_res);
            REQUIRES(null_ptr_res);
            REQUIRES(zero_res);
            REQUIRES(n_trivial_res);
        }
    }

    SUBCASE("ne")
    {
        SUBCASE("lhs")
        {
            let ptr_res = nil != ptr;
            nonce(ptr_res);
            let cmp one_res = nil != 1;
            let cmp trivial_res = nil != equatable;

            REQUIRE(ptr_res);
            REQUIRES(one_res);
            REQUIRES(trivial_res);
        }

        SUBCASE("rhs")
        {
            let ptr_res = ptr != nil;
            nonce(ptr_res);
            let cmp one_res = 1 != nil;
            let cmp trivial_res = equatable != nil;

            REQUIRE(ptr_res);
            REQUIRES(one_res);
            REQUIRES(trivial_res);
        }
    }
}


// validity

COND_TMP_UNARY(detail::is_strictly_validatable_g<T>)
cmp_fn op==(T&& lhs, nonced nil_t rhs) noex { ret !ENV_STD::forward<T>(lhs).is_valid(); }

COND_TMP_UNARY(detail::is_strictly_validatable_g<T>)
cmp_fn op==(nonced nil_t lhs, T&& rhs) noex { ret !ENV_STD::forward<T>(rhs).is_valid(); }

COND_TMP_UNARY(detail::is_strictly_validatable_g<T>)
cmp_fn op!=(T&& lhs, nonced nil_t rhs) noex { ret ENV_STD::forward<T>(lhs).is_valid(); }

COND_TMP_UNARY(detail::is_strictly_validatable_g<T>)
cmp_fn op!=(nonced nil_t lhs, T&& rhs) noex { ret ENV_STD::forward<T>(rhs).is_valid(); }

ENV_TEST_CASE("nil validity")
{
    obj cmp test::strictly_validatable_t valid{1}, invalid(nil);

    SUBCASE("eq")
    {
        SUBCASE("lhs")
        {
            let cmp valid_res = valid == nil;
            let cmp invalid_res = invalid == nil;

            REQUIRES_FALSE(valid_res);
            REQUIRES(invalid_res);
        }

        SUBCASE("rhs")
        {
            let cmp valid_res = nil == valid;
            let cmp invalid_res = nil == invalid;

            REQUIRES_FALSE(valid_res);
            REQUIRES(invalid_res);
        }
    }

    SUBCASE("ne")
    {
        SUBCASE("lhs")
        {
            let cmp valid_res = valid != nil;
            let cmp invalid_res = invalid != nil;

            REQUIRES(valid_res);
            REQUIRES_FALSE(invalid_res);
        }

        SUBCASE("rhs")
        {
            let cmp valid_res = nil != valid;
            let cmp invalid_res = nil != invalid;

            REQUIRES(valid_res);
            REQUIRES_FALSE(invalid_res);
        }
    }
}


// is_(valid/invalid)

tmp<name T>
cmp_fn is_valid(validatable_c<T>&& to_check) noex { ret ENV_STD::forward<T>(to_check) != nil; }

tmp<name T>
cmp_fn is_invalid(validatable_c<T>&& to_check) noex { ret ENV_STD::forward<T>(to_check) == nil; }

ENV_TEST_CASE("is_(valid/invalid)")
{
    cmp int i = 1;
    const int* ptr = &i;
    cmp int* null_ptr{nullptr};
    cmp test::default_equatable_t equatable_nil(nil), equatable{1};
    cmp test::strictly_validatable_t valid{1}, invalid(nil);

    REQUIRES(is_valid(i));
    REQUIRES(is_invalid(0));

    REQUIRE(is_valid(ptr));
    REQUIRES(is_invalid(null_ptr));
    REQUIRES(is_invalid(nullptr));

    REQUIRES(is_valid(equatable));
    REQUIRES(is_invalid(equatable_nil));

    REQUIRES(is_valid(valid));
    REQUIRES(is_invalid(invalid));
}


#endif // ENV_NIL_HPP
