#ifndef ENV_HPP
#define ENV_HPP

#include "structure/env_structure.hpp"

FWA_NAMESPACE_CORE_BEGIN

ENV_TEST_SUITE_BEGIN("core");

#include "heap/env_heap.hpp"
#include "iteration/env_iteration.hpp"
#include "macros/env_macros.hpp"
#include "meta/env_meta.hpp"
#include "ops/env_ops.hpp"
#include "proxies/env_proxies.hpp"
#include "stack/env_stack.hpp"
#include "traits/env_traits.hpp"

ENV_TEST_SUITE_END();

FWA_NAMESPACE_CORE_END

#endif // ENV_HPP
