#ifndef FWA_CORE_TRAITS_HPP
#define FWA_CORE_TRAITS_HPP

FWA_CORE_TEST_SUITE_BEGIN("traits");

#include "fwa_qualifiers.hpp"
#include "fwa_basic_traits.hpp"
#include "fwa_arithmetic.hpp"
#include "fwa_ratios.hpp"
#include "fwa_members.hpp"
#include "fwa_callable.hpp"

FWA_CORE_TEST_SUITE_END();


#ifdef FWA_STD_REQUIREMENTS

FWA_CORE_TEST_SUITE_BEGIN("std requirements");

#include "standard_requirements/fwa_standard_requirements.hpp"

FWA_CORE_TEST_SUITE_END();

#endif // FWA_STD_REQUIREMENTS


#endif // FWA_CORE_TRAITS_HPP
