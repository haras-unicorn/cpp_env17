#ifndef FWA_CORE_HPP
#define FWA_CORE_HPP


#include "structure/fwa_structure.hpp"

FWA_NAMESPACE_CORE_BEGIN

FWA_CORE_TEST_SUITE_BEGIN("core");

#include "macros/fwa_macros.hpp"
#include "meta/fwa_meta.hpp"
#include "traits/fwa_traits.hpp"
#include "stack/fwa_stack.hpp"
#include "ops/fwa_ops.hpp"
#include "heap/fwa_heap.hpp"
#include "proxies/fwa_proxies.hpp"
#include "iteration/fwa_iteration.hpp"

FWA_CORE_TEST_SUITE_END();

FWA_NAMESPACE_CORE_END


#endif // FWA_CORE_HPP
