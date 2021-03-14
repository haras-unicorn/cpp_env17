#ifndef ENV_CLEANUP_MACROS_HPP
#define ENV_CLEANUP_MACROS_HPP

#if ENV_UNDEF_MACROS

// attributes

#undef TYPE_ATTRIBUTES
#undef OBJECT_ATTRIBUTES

#undef RETURN_ATTRIBUTES
#undef NO_RETURN_ATTRIBUTES

#undef TAG
#undef UNIQUE
#undef UTAG

// keywords

#undef tmp
#undef name
#undef sass
#undef sass_msg

#undef cmp
#undef inl
#undef noex
#undef if_cmp

#undef op
#undef deduc
#undef decl
#undef ret

#undef scast
#undef ccast
#undef dcast
#undef rcast
#undef unused

// types

#undef typ_p
#undef typ_a
#undef typ

#undef strct_p
#undef strct

#undef cls_p
#undef cls

#undef enm_p
#undef enm

// objects

#undef obj_p
#undef obj
#undef cmp_obj_p
#undef cmp_obj

#undef temp_p
#undef temp
#undef mut_p
#undef mut
#undef let_p
#undef let

#undef let_cmp_p
#undef let_cmp

// functions

#undef imp_p
#undef imp
#undef cmp_imp_p
#undef cmp_imp
#undef con_p
#undef con
#undef cmp_con_p
#undef cmp_con

#undef fun_p
#undef fun
#undef cmp_fn_p
#undef cmp_fn

#undef callb_p
#undef callb
#undef cmp_callb_p
#undef cmp_callb

// scope

#undef UID

#undef SCOPE_IMPL
#undef SCOPE

#undef IFFY_IMPL
#undef IFFY

#undef IF
#undef CMP_IF

#undef ON
#undef CMP_ON

#undef TERN
#undef CMP_TERN

#undef ELVIS
#undef CMP_ELVIS

// body

// structs

#undef ACCESS_BEGIN
#undef ACCESS_END

// application formula body

#undef APP_FML
#undef FML_BODY

#endif // ENV_UNDEF_MACROS

#endif // ENV_CLEANUP_MACROS_HPP
