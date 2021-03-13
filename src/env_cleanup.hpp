#ifndef ENV_CLEANUP_HPP
#define ENV_CLEANUP_HPP

#ifdef ENV_UNDEF_OPS
#undef ENV_UNDEF_OPS
#define ENV_UNDEF_OPS 1
#endif // ENV_UNDEF_OPS
#ifdef ENV_UNDEF_STACK
#undef ENV_UNDEF_STACK
#define ENV_UNDEF_STACK 1
#define ENV_UNDEF_OPS 1
#endif // ENV_UNDEF_STACK
#ifdef ENV_UNDEF_META
#undef ENV_UNDEF_META
#define ENV_UNDEF_META 1
#define ENV_UNDEF_STACK 1
#define ENV_UNDEF_OPS 1
#endif // ENV_UNDEF_META
#ifdef ENV_UNDEF_MACROS
#undef ENV_UNDEF_MACROS
#define ENV_UNDEF_MACROS 1
#define ENV_UNDEF_META 1
#define ENV_UNDEF_STACK 1
#define ENV_UNDEF_OPS 1
#endif // ENV_UNDEF_MACROS

#include "macros/env_cleanup_macros.hpp"
#include "meta/env_cleanup_meta.hpp"
#include "ops/env_cleanup_ops.hpp"
#include "stack/env_cleanup_stack.hpp"
#include "structure/env_cleanup_structure.hpp"

#endif // ENV_CLEANUP_HPP
