#ifndef ENV_CLEANUP_MACROS_HPP
#define ENV_CLEANUP_MACROS_HPP

// global warning suppression - please add only when necessary!!

// suppress JetBrains and other inspections added by extensions
FWA_CLANG_SUPPRESS_POP

// anonymous empty structs - I have no idea why I can't just put this in the ANON_STRUCT macro
FWA_CLANG_SUPPRESS_POP
FWA_MSVC_SUPPRESS_POP
FWA_MSVC_SUPPRESS_POP

// doctest sometimes declares variables with two const qualifiers
FWA_CLANG_SUPPRESS_POP
FWA_MSVC_SUPPRESS_POP

// undef

#if FWA_UNDEF_MACROS

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

#undef FWA_COMPILER_MAJOR
#undef FWA_COMPILER_MINOR
#undef FWA_COMPILER_PATCH
#undef FWA_COMPILER_VER
#undef FWA_COMPILER_NAME

#undef FWA_MSVC
#undef FWA_CLANG
#undef FWA_GCC

// arch

#undef FWA_ARCH_NAME

#undef FWA_X64
#undef FWA_X86
#undef FWA_ARM

// os

#undef FWA_OS_NAME

#undef FWA_WIN
#undef FWA_WIN32
#undef FWA_WIN64

#undef FWA_APPLE
#undef FWA_MAC
#undef FWA_IOS
#undef FWA_IOS_SIM

#undef FWA_LINUX
#undef FWA_ANDROID

// standard

#undef FWA_CPP

#undef FWA_CPP98
#undef FWA_CPP11
#undef FWA_CPP14
#undef FWA_CPP17
#undef FWA_CPP20
#undef FWA_CPP23

// pragma

#undef FWA_PRAGMA

#undef FWA_MESSAGE

#undef FWA_CLANG_SUPPRESS_PUSH
#undef FWA_CLANG_SUPPRESS_POP

#undef FWA_GCC_SUPPRESS_PUSH
#undef FWA_GCC_SUPPRESS_POP

#undef FWA_MSVC_SUPPRESS_PUSH
#undef FWA_MSVC_SUPPRESS_POP

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

#undef typ_impl
#undef typ_p
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

#undef ANON_NAME

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

#endif // FWA_UNDEF_MACROS

#endif // ENV_CLEANUP_MACROS_HPP
