#ifndef ENV_CLEANUP_MACROS_HPP
#define ENV_CLEANUP_MACROS_HPP

// global suppress

#ifdef ENV_MESSAGES
ENV_CLANG_SUPPRESS_POP // messages
#endif // ENV_MESSAGES

ENV_CLANG_SUPPRESS_POP // clang-tidy

// undef

#if ENV_UNDEF_MACROS

// tokens

// basic

#undef SKIP
#undef EMPTY
#undef COMMA
#undef OPEN
#undef CLOSE

#undef SINGLE
#undef PACK
#undef ERASE
#undef CONSUME

// strings

#undef CAT_IMPL
#undef CAT

#undef INTER_IMPL
#undef INTER

#undef STRING_IMPL
#undef STRING_VAR
#undef STRING

// spread

#undef SPREAD_IMPL
#undef SPREAD

// compiler

#undef ENV_COMPILER_MAJOR
#undef ENV_COMPILER_MINOR
#undef ENV_COMPILER_PATCH
#undef ENV_COMPILER_VER
#undef ENV_COMPILER_NAME

#undef ENV_MSVC
#undef ENV_CLANG
#undef ENV_GCC

// arch

#undef ENV_ARCH_NAME

#undef ENV_X64
#undef ENV_X86
#undef ENV_ARM

// os

#undef ENV_OS_NAME

#undef ENV_WIN
#undef ENV_WIN32
#undef ENV_WIN64

#undef ENV_APPLE
#undef ENV_MAC
#undef ENV_IOS
#undef ENV_IOS_SIM

#undef ENV_LINUX
#undef ENV_ANDROID

// standard

#undef ENV_CPP

#undef ENV_CPP98
#undef ENV_CPP11
#undef ENV_CPP14
#undef ENV_CPP17
#undef ENV_CPP20
#undef ENV_CPP23

// pragma

#undef ENV_PRAGMA

#undef ENV_MESSAGE

#undef ENV_CLANG_SUPPRESS_PUSH
#undef ENV_CLANG_SUPPRESS_POP

#undef ENV_GCC_SUPPRESS_PUSH
#undef ENV_GCC_SUPPRESS_POP

#undef ENV_MSVC_SUPPRESS_PUSH
#undef ENV_MSVC_SUPPRESS_POP

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

#undef ANON

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

#undef ANON_STRUCT

#undef FORMAT
#undef ACCESS_BEGIN
#undef ACCESS_END
#undef CLASS_SEMI

// application formula body

#undef APP_FML
#undef FML_BODY

#endif // ENV_UNDEF_MACROS

#endif // ENV_CLEANUP_MACROS_HPP
