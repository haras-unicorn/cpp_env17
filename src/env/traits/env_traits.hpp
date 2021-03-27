#ifndef ENV_TRAITS_HPP
#define ENV_TRAITS_HPP


ENV_TEST_SUITE_BEGIN("traits");

#include "env_instantiation.hpp"
#include "env_qualifiers.hpp"
#include "env_members.hpp"
#include "env_behaviour.hpp"
#include "env_indirection.hpp"
#include "env_life.hpp"

#include "env_callable.hpp"
#include "env_elements.hpp"
#include "env_arithmetic.hpp"
#include "env_ratios.hpp"
#include "env_atomic.hpp"

#include "env_typeclasses.hpp"

ENV_TEST_SUITE_END();


#ifdef ENV_STD_REQUIREMENTS

ENV_TEST_SUITE_BEGIN("std requirements");

#include "standard_requirements/env_standard_requirements.hpp"

ENV_TEST_SUITE_END();

#endif // ENV_STD_REQUIREMENTS


#endif // ENV_TRAITS_HPP
