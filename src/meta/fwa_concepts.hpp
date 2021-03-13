#ifndef FWA_CORE_CONCEPTS_HPP
#define FWA_CORE_CONCEPTS_HPP


// TODO: add more tests

// cond

#define COND_CONCEPT_OPT(_name, _tmp, ...) \
            COND_TMP((name QC SPREAD(_tmp), name C = FWA_CORE::unqualified_gt<QC>), \
                     FWA_STD::is_same_v<C, FWA_CORE::unqualified_gt<QC>> && (__VA_ARGS__)) \
            typ(CAT(_name, _c)) = QC; \
             \
            tmp<name C SPREAD(_tmp)> \
            typ(CAT(_name, _r)) = FWA_CORE::require_ngt<__VA_ARGS__, C>

#define ELABORATE_COND_CONCEPT(_name, _tmp, ...) COND_CONCEPT_OPT(_name, (, SPREAD(_tmp)), __VA_ARGS__)

#define COND_CONCEPT(_name, ...) COND_CONCEPT_OPT(_name, (), __VA_ARGS__)
#define COND_CONCEPT_UNARY(_name, ...) ELABORATE_COND_CONCEPT(_name, (name T), __VA_ARGS__)
#define COND_CONCEPT_BINARY(_name, ...) ELABORATE_COND_CONCEPT(_name, (name TLhs, name TRhs), __VA_ARGS__)
#define COND_CONCEPT_TERNARY(_name, ...) ELABORATE_COND_CONCEPT(_name, (name T1, name T2, name T3), __VA_ARGS__)

FWA_NAMESPACE_TEST_BEGIN

COND_CONCEPT(number, FWA_STD::is_arithmetic_v < C >);

tmp<name TLhs, name TRhs>
cmp_fn add(number_c<TLhs>&& lhs, number_c<TRhs>&& rhs) noex
{
    ret FWA_STD::forward<TLhs>(lhs) + FWA_STD::forward<TRhs>(rhs);
}

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("require concept")
{
    REQUIRES(test::add(1, 2) == 3);
    REQUIRES(test::add(1.2, 3) == 4.2);
}


// expr

#define EXPR_CONCEPT_OPT(_name, _tmp, ...) \
            EXPR_TMP((name QC SPREAD(_tmp), name C = FWA_CORE::unqualified_gt<QC>), \
                     COND_EXPR(FWA_STD::is_same_v<C, FWA_CORE::unqualified_gt<QC>>), __VA_ARGS__) \
            typ(CAT(_name, _c)) = QC; \
             \
            tmp<name C SPREAD(_tmp)> \
            typ(CAT(_name, _r)) = decl(__VA_ARGS__, FWA_CORE::success, TYPE_EXPR(C))

#define ELABORATE_EXPR_CONCEPT(_name, _tmp, ...) EXPR_CONCEPT_OPT(_name, (, SPREAD(_tmp)), __VA_ARGS__)

#define EXPR_CONCEPT(_name, ...) EXPR_CONCEPT_OPT(_name, (), __VA_ARGS__)
#define EXPR_CONCEPT_UNARY(_name, ...) ELABORATE_EXPR_CONCEPT(_name, (name T), __VA_ARGS__)
#define EXPR_CONCEPT_BINARY(_name, ...) ELABORATE_EXPR_CONCEPT(_name, (name TLhs, name TRhs), __VA_ARGS__)
#define EXPR_CONCEPT_TERNARY(_name, ...) ELABORATE_EXPR_CONCEPT(_name, (name T1, name T2, name T3), __VA_ARGS__)

FWA_NAMESPACE_TEST_BEGIN

EXPR_CONCEPT(addable, declvalr<number_r<C>>() + declvalr<C>());

tmp<name T>
cmp_fn add(addable_c<T>&& lhs, addable_c<T>&& rhs) noex
{
    ret FWA_STD::forward<T>(lhs) + FWA_STD::forward<T>(rhs);
}

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("sfinae concept")
{
    REQUIRES(test::add(1, 2) == 3);
    REQUIRES(test::add(4.0, 3.0) == 7);
}


// type

#define TYPE_CONCEPT_OPT(_name, _tmp, ...) \
            TYPE_TMP((name QC SPREAD(_tmp), name C = FWA_CORE::unqualified_gt<QC>), \
                     COND_TYPE(FWA_STD::is_same_v<C, FWA_CORE::unqualified_gt<QC>>), __VA_ARGS__) \
            typ(CAT(_name, _c)) = QC; \
             \
            tmp<name C SPREAD(_tmp)> \
            typ(CAT(_name, _r)) = name FWA_CORE::variadic_vt<__VA_ARGS__, C>::last_t

#define ELABORATE_TYPE_CONCEPT(_name, _tmp, ...) TYPE_CONCEPT_OPT(_name, (, SPREAD(_tmp)), __VA_ARGS__)

#define TYPE_CONCEPT(_name, ...) TYPE_CONCEPT_OPT(_name, (), __VA_ARGS__)
#define TYPE_CONCEPT_UNARY(_name, ...) ELABORATE_TYPE_CONCEPT(_name, (name T), __VA_ARGS__)
#define TYPE_CONCEPT_BINARY(_name, ...) ELABORATE_TYPE_CONCEPT(_name, (name TLhs, name TRhs), __VA_ARGS__)
#define TYPE_CONCEPT_TERNARY(_name, ...) ELABORATE_TYPE_CONCEPT(_name, (name T1, name T2, name T3), __VA_ARGS__)

FWA_NAMESPACE_TEST_BEGIN

TYPE_CONCEPT(subtractable, enable_minus_gt < C >);

tmp<name TLhs, name TRhs>
cmp_fn subtract(subtractable_c<TLhs>&& lhs, subtractable_c<TRhs>&& rhs) noex
{
    ret FWA_STD::forward<TLhs>(lhs) - FWA_STD::forward<TRhs>(rhs);
}

FWA_NAMESPACE_TEST_END

FWA_CORE_TEST_CASE("sfinae concept")
{
    REQUIRES(test::subtract(1, 2) == -1);
    REQUIRES(test::subtract(4.0, 3.0) == 1);
}


#endif // FWA_CORE_CONCEPTS_HPP
