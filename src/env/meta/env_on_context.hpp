#ifndef ENV_ON_CONTEXT_HPP
#define ENV_ON_CONTEXT_HPP



// dep

#define dep_name name TDep = ENV::void_t

// cond
// NOTE: you can only check for conditions here, so no conversion from type or expr into condition

#define ON_COND(...) COND_TMP((dep_name), (ENV_STD::is_same_v<TDep, ENV::void_t> && (__VA_ARGS__)))

ENV_TEST_BEGIN

tmp<name T>
strct cond_const
{
    ON_COND(ENV_STD::is_const_v < T >) cmp_fn test(int) const noex
    {
        ret true;
    }

    cmp_fn test(...) const noex { ret false; }
};

ENV_TEST_END

ENV_TEST_CASE("cond")
{
    REQUIRES(test::cond_const<const int>{ }.test(0));
    REQUIRES_FALSE(test::cond_const<int>{ }.test(0));
}

// TODO: do this for type
// I believe it can be done somehow, but the thing is that the only reason that the cond works is that
// the expression used to create the error in requires depends on TDep being known whereas if you have an expression in
// decltype or just a type that is computable with the context the compiler immediately tries to compute the expression
// or type

//// expr
//
//#define ON_EXPR(...)
//        strct CAT(_on_expr_, __LINE__) { SFINAE_IN_CLASS_STRUCT(ID, (name TOnExpr), (TOnExpr), __VA_ARGS__); };
//        COND_TMP((dep_name), CAT(_on_expr_, __LINE__)::tmp CAT(ID, _g)<TDep>)
//
//ENV_NAMESPACE_TEST_BEGIN
//
//tmp<name T>
//strct expr_addable
//{
//    ON_EXPR(declval<T>() + declval<T>())
//    cmp_fn test_addable(int) const noex { ret true; }
//
//    cmp_fn test_addable(...) const noex { ret false; }
//};
//
//ENV_NAMESPACE_TEST_END
//
//ENV_TEST_CASE("expr")
//{
//    REQUIRES(test::expr_addable<int>{ }.test_addable(0));
//    REQUIRES_FALSE(test::expr_addable<void>{ }.test_addable(0));
//}

//// type
//
//#define ON_TYPE(...) TYPE_TMP((dep_name), ENV::require_nt<ENV::is_expr_g<TDep>>, __VA_ARGS__)
//
//ENV_NAMESPACE_TEST_BEGIN
//
//EXPR_TMP_UNARY(declval<T>() + declval<T>())
//strct enable_plus_gt {};
//
//tmp<name T>
//strct type_addable
//{
//    ON_TYPE(enable_plus_gt<T>)
//    cmp_fn test_addable(int) const noex { ret true; }
//
//    cmp_fn test_addable(...) const noex { ret false; }
//};
//
//ENV_NAMESPACE_TEST_END
//
//ENV_TEST_CASE("expr")
//{
//    REQUIRES(test::type_addable<int>{ }.test_addable(0));
//    REQUIRES_FALSE(test::type_addable<nullptr_t>{}.test_addable(0));
//}

#endif // ENV_ON_CONTEXT_HPP
