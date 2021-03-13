#ifndef FWA_CLEANUP_HPP
#define FWA_CLEANUP_HPP


#ifdef FWA_UNDEF_OPS
#undef FWA_UNDEF_OPS
#define FWA_UNDEF_OPS 1
#endif // FWA_UNDEF_OPS
#ifdef FWA_UNDEF_STACK
#undef FWA_UNDEF_STACK
#define FWA_UNDEF_STACK 1
#define FWA_UNDEF_OPS 1
#endif // FWA_UNDEF_STACK
#ifdef FWA_UNDEF_META
#undef FWA_UNDEF_META
#define FWA_UNDEF_META 1
#define FWA_UNDEF_STACK 1
#define FWA_UNDEF_OPS 1
#endif // FWA_UNDEF_META
#ifdef FWA_UNDEF_MACROS
#undef FWA_UNDEF_MACROS
#define FWA_UNDEF_MACROS 1
#define FWA_UNDEF_META 1
#define FWA_UNDEF_STACK 1
#define FWA_UNDEF_OPS 1
#endif // FWA_UNDEF_MACROS

#include "ops/fwa_cleanup_ops.hpp"
#include "stack/fwa_cleanup_stack.hpp"
#include "meta/fwa_cleanup_meta.hpp"
#include "macros/fwa_cleanup_macros.hpp"
#include "structure/fwa_cleanup_structure.hpp"


#endif // FWA_CLEANUP_HPP
