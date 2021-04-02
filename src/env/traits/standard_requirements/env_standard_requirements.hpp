#ifndef ENV_STANDARD_REQUIREMENTS_HPP
#define ENV_STANDARD_REQUIREMENTS_HPP

// This project is modelling: https://en.cppreference.com/w/cpp/named_req.
// The goal of this project is to be able to statically check the conformance of user-created data structures.

// PSA: use these only for testing.
// Use the standard type traits as much as possible because they are intrinsic for these requirements.
// These will probably drastically increase your compile times.

// PSA 2: don't just format these files with your IDE and/or formatter.
// They use a lot of macros and most IDE's don't do a good job at formatting macro arguments.

// PSA 3: some of the requirements have preconditions that are impossible to check.
// These requirements try to be as close to the the standard as it is reasonable for them to be.
// Example: https://en.cppreference.com/w/cpp/named_req/OutputIterator (see invalidation and assignment).

// PSA 3: some of the requirements just use the requirements from type_traits.
// It is not possible to check for them otherwise.
// Example: https://en.cppreference.com/w/cpp/named_req/StandardLayoutType.

// PSA 4: some of the requirements have a lot of notes that are not specified here.
// There will be a URL for every requirement to the cppreference page of that requirement if you want to check those.
// There will also be comments for a lot of requirements if you just want to check something quickly.
// Example: https://en.cppreference.com/w/cpp/named_req/Allocator (notes at the bottom).

// PSA 5: some of the requirements are either too hard to implement or impossible to implement
// Example: https://en.cppreference.com/w/cpp/named_req/ConstexprIterator.

// PSA 6: some of the requirements are made specifically for certain types in the STL
// These requirements will not be checked, as this doesn't have anything to do with user-created data structures.
// Example: https://en.cppreference.com/w/cpp/named_req/SequenceContainer (see insert, erase and assign).

// PSA 7: some of the requirements are a bit renamed to ease readability
// Example: https://en.cppreference.com/w/cpp/named_req/EmplaceConstructible
//  - is_std_emplace_constructible_into should be called is_std_emplace_constructible

// There are the type properties requirements and some library-wide requirements in this one.
// This is due to the fact that requirements later on are dependent upon these.
#include "env_basic_standard_requirements.hpp"

#include "env_iterator_standard_requirements.hpp"

// the rest of the library-wide requirements
#include "env_library_wide_standard_requirements.hpp"
// this one has a lot of requirements, so it has its own file
#include "env_allocator_requirements.hpp"

#include "env_container_element_requirements.hpp"
// containers have so many requirements that this is needed
#include "env_container_traits_requirements.hpp"
#include "env_container_standard_requirements.hpp"
//
//#include "env_concurrency_requirements.hpp"
//
//#include "env_other_requirements.hpp"

#endif // ENV_STANDARD_REQUIREMENTS_HPP
