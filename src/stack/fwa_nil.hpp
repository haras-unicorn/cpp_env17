#ifndef FWA_CORE_NIL_HPP
#define FWA_CORE_NIL_HPP


// checks

EXPR_CHECK_UNARY(is_zero_constructible, T(0));

EXPR_CHECK_UNARY(is_nullptr_constructible, T(nullptr));


FWA_NAMESPACE_TEST_BEGIN

strct trivial_equatable_t
{
    int i;
    cmp_fn op==(trivial_equatable_t other) const noex { ret i == other.i; }
    cmp_fn op!=(trivial_equatable_t other) const noex { ret i != other.i; }
};

FWA_NAMESPACE_TEST_END

TEST_CASE("nil checks")
{
    REQUIRES(is_nullptr_constructible_g<int*>);
    REQUIRES(is_zero_constructible_g<int*>);

    REQUIRES(!is_nullptr_constructible_g<int>);
    REQUIRES(is_zero_constructible_g<int>);

    REQUIRES(!is_nullptr_constructible_g<test::trivial_equatable_t>);
    REQUIRES(!is_zero_constructible_g<test::trivial_equatable_t>);
    REQUIRES(FWA_STD::is_default_constructible_v < test::trivial_equatable_t >);
}


// nil

strct nil_t
{
    FWA_CLANG_SUPPRESS_PUSH("google-explicit-constructor")

    COND_TMP_UNARY
    (
            is_nullptr_constructible_g<T>
    )
    imp inl cmp op T() const noex { ret T(nullptr); }

    COND_TMP_UNARY
    (
            !is_nullptr_constructible_g<T> &&
            is_zero_constructible_g<T>
    )
    imp inl cmp op T() const noex { ret T(0); }

    COND_TMP_UNARY
    (
            !is_nullptr_constructible_g<T> &&
            !is_zero_constructible_g<T> &&
            FWA_STD::is_default_constructible_v < T >
    )
    imp inl cmp op T() const noex { ret T(); }

    FWA_CLANG_SUPPRESS_POP
};

let_cmp nil = nil_t{ };


COND_CHECK_UNARY(is_nillable, is_imp_convertible_g<nil_t, T>);

COND_CONCEPT(nillable, is_nillable_g<C>);


FWA_CORE_TEST_CASE("nil init")
{
    cmp int* i_ptr{nil};
    REQUIRES(i_ptr == nullptr);

    cmp nullptr_t null_ptr{nil};
    REQUIRES(null_ptr == nullptr);

    cmp int i{nil};
    REQUIRES(i == 0);

    // With equals sign to avoid aggregate initialization to the int i
    cmp test::trivial_equatable_t trivial = nil;
    REQUIRES(trivial.i == 0);
}


// equality

cmp_fn op==(obj nil_t lhs, obj nil_t rhs) noex { ret true; }

cmp_fn op!=(obj nil_t lhs, obj nil_t rhs) noex { ret false; }

tmp<name T>
cmp_fn op==(equatable_c<nillable_c<T>>&& lhs, nil_t rhs) noex
{
    ret FWA_STD::forward<T>(lhs) == static_cast<unqualified_gt<T>>(rhs);
}

tmp<name T>
cmp_fn op==(nil_t lhs, equatable_c<nillable_c<T>>&& rhs) noex
{
    ret static_cast<unqualified_gt<T>>(lhs) == FWA_STD::forward<T>(rhs);
}

tmp<name T>
cmp_fn op!=(equatable_c<nillable_c<T>>&& lhs, nil_t rhs) noex
{
    ret FWA_STD::forward<T>(lhs) != static_cast<unqualified_gt<T>>(rhs);
}

tmp<name T>
cmp_fn op!=(nil_t lhs, equatable_c<nillable_c<T>>&& rhs) noex
{
    ret static_cast<unqualified_gt<T>>(lhs) != FWA_STD::forward<T>(rhs);
}


FWA_CORE_TEST_CASE ("nil equals")
{
    cmp test::trivial_equatable_t n_trivial(nil), trivial{1};

    cmp int i = 1;
    const int* ptr = &i;
    cmp int* null_ptr{nullptr};

    SUBCASE("eq")
    {
        SUBCASE("self")
        {
            // doctest somehow makes this harder to do
            let cmp eq_res = nil == nil;
            let cmp ne_res = nil != nil;

            REQUIRES(eq_res);
            REQUIRES_FALSE(ne_res);
        }

        SUBCASE("lhs")
        {
            let cmp nullptr_res = nil == nullptr;
            let cmp null_ptr_res = nil == null_ptr;
            let cmp zero_res = nil == 0;
            let cmp n_trivial_res = nil == n_trivial;

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
            let cmp n_trivial_res = n_trivial == nil;

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
            let cmp one_res = nil != 1;
            let cmp trivial_res = nil != trivial;

            REQUIRE(ptr_res);
            REQUIRES(one_res);
            REQUIRES(trivial_res);
        }

        SUBCASE("rhs")
        {
            let ptr_res = ptr != nil;
            let cmp one_res = 1 != nil;
            let cmp trivial_res = trivial != nil;

            REQUIRE(ptr_res);
            REQUIRES(one_res);
            REQUIRES(trivial_res);
        }
    }
}


#endif // FWA_CORE_NIL_HPP
