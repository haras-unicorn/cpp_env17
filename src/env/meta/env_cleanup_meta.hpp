#ifndef ENV_CLEANUP_META_HPP
#define ENV_CLEANUP_META_HPP

#if ENV_UNDEF_META

// constants

#undef val
#undef val_con

// results

#undef res_name
#undef deduc_res
#undef res_t
#undef res_cast

// templates

// names

#undef dep_name

// skips

#undef SKIP_COND
#undef SKIP_EXPR
#undef SKIP_TYPE

// conversion

#undef EXPR_TYPE
#undef EXPR_COND
#undef TYPE_EXPR
#undef TYPE_COND
#undef COND_EXPR
#undef COND_TYPE

// cond tmp

#undef COND_TMP_OPT
#undef COND_TMP
#undef COND_TMP_UNARY
#undef COND_TMP_BINARY
#undef COND_TMP_TERNARY
#undef COND_TMP_VARIADIC

// expr tmp

#undef EXPR_TMP_OPT
#undef EXPR_TMP
#undef EXPR_TMP_UNARY
#undef EXPR_TMP_BINARY
#undef EXPR_TMP_TERNARY
#undef EXPR_TMP_VARIADIC

// type tmp

#undef TYPE_TMP_OPT
#undef TYPE_TMP
#undef TYPE_TMP_UNARY
#undef TYPE_TMP_BINARY
#undef TYPE_TMP_TERNARY
#undef TYPE_TMP_VARIADIC
#undef TYPE

// expr check

#undef CHECK_OPT
#undef ELABORATE_EXPR_CHECK
#undef EXPR_CHECK
#undef EXPR_CHECK_UNARY
#undef EXPR_CHECK_BINARY
#undef EXPR_CHECK_TERNARY
#undef EXPR_CHECK_VARIADIC

// expr class check

#undef ELABORATE_EXPR_CLASS_CHECK
#undef EXPR_CLASS_CHECK
#undef EXPR_CLASS_CHECK_UNARY
#undef EXPR_CLASS_CHECK_BINARY
#undef EXPR_CLASS_CHECK_TERNARY
#undef EXPR_CLASS_CHECK_VARIADIC

// cond check

#undef COND_CHECK_OPT
#undef ELABORATE_COND_CHECK
#undef COND_CHECK
#undef COND_CHECK_UNARY
#undef COND_CHECK_BINARY
#undef COND_CHECK_TERNARY
#undef COND_CHECK_VARIADIC

// cond class check

#undef ELABORATE_COND_CLASS_CHECK
#undef COND_CLASS_CHECK
#undef COND_CLASS_CHECK_UNARY
#undef COND_CLASS_CHECK_BINARY
#undef COND_CLASS_CHECK_TERNARY
#undef COND_CLASS_CHECK_VARIADIC

// type check

#undef TYPE_CHECK_OPT
#undef ELABORATE_TYPE_CHECK
#undef TYPE_CHECK
#undef TYPE_CHECK_UNARY
#undef TYPE_CHECK_BINARY
#undef TYPE_CHECK_TERNARY
#undef TYPE_CHECK_VARIADIC

// type class check

#undef ELABORATE_TYPE_CLASS_CHECK
#undef TYPE_CLASS_CHECK
#undef TYPE_CLASS_CHECK_UNARY
#undef TYPE_CLASS_CHECK_BINARY
#undef TYPE_CLASS_CHECK_TERNARY
#undef TYPE_CLASS_CHECK_VARIADIC

// extract check

#undef EXTRACT_CHECK_OPT
#undef EXTRACT_CHECK
#undef SIMPLE_EXTRACT_CHECK
#undef EXTRACT_CLASS_CHECK
#undef SIMPLE_EXTRACT_CLASS_CHECK

// on context

#undef ON_COND
#undef ON_EXPR
#undef ON_TYPE

// cond concept

#undef COND_CONCEPT_OPT
#undef ELABORATE_COND_CONCEPT
#undef COND_CONCEPT
#undef COND_CONCEPT_UNARY
#undef COND_CONCEPT_BINARY
#undef COND_CONCEPT_TERNARY
#undef COND_CONCEPT_VARIADIC

// expr concept

#undef EXPR_CONCEPT_OPT
#undef ELABORATE_EXPR_CONCEPT
#undef EXPR_CONCEPT
#undef EXPR_CONCEPT_UNARY
#undef EXPR_CONCEPT_BINARY
#undef EXPR_CONCEPT_TERNARY
#undef EXPR_CONCEPT_VARIADIC

// type concept

#undef TYPE_CONCEPT_OPT
#undef ELABORATE_TYPE_CONCEPT
#undef TYPE_CONCEPT
#undef TYPE_CONCEPT_UNARY
#undef TYPE_CONCEPT_BINARY
#undef TYPE_CONCEPT_TERNARY
#undef TYPE_CONCEPT_VARIADIC

// members

#undef MEMBER_DETECTOR

#undef DATA_DETECTOR
#undef FUNCTION_DETECTOR
#undef OPERATOR_DETECTOR

#undef SDATA_DETECTOR
#undef ALIAS_DETECTOR

// self

// this type

#undef DECL_THIS

// inheritor type

#undef inheritor
#undef inheritor_extract
#undef inheritor_name

#undef DECL_INHERITOR_TYPE

#undef DECL_INHERITOR

// self

#undef DECL_SELF_INHERITOR

#undef DECL_SELF

#undef DECL_SELF_NO_INHERITOR

// bases

#undef BASE_NAME
#undef BASE_CONST_NAME
#undef BASE_MUT_NAME

#undef BASE_PTR_NAME
#undef BASE_CONST_PTR_NAME
#undef BASE_MUT_PTR_NAME

#undef DECL_BASE

// self getters

#undef CRTP_INHERITOR

#undef CRTP

#undef CRTP_BASE

// base getters

#undef _AS_BASE_NAME
#undef AS_BASE_NAME

#undef GET_AS_BASE

#undef DEF_AS_BASE_BODY

#undef DEF_AS_BASE_FML
#undef _DEF_AS_BASE
#undef DEF_AS_BASE

#endif // ENV_UNDEF_META

#endif // ENV_CLEANUP_META_HPP
