#ifndef ENV_STRUCTURE_HPP
#define ENV_STRUCTURE_HPP


#include "env_directives.hpp"


#include "env_tokens.hpp"

#include "env_arch.hpp"
#include "env_compiler.hpp"
#include "env_os.hpp"
#include "env_standard.hpp"
#include "env_pragma.hpp"
#include "env_warnings.hpp"

#undef ENV_SUPPRESS_POP
#undef ENV_SUPPRESS_PUSH
#define ENV_SUPPRESS_PUSH

#include "env_suppress.hpp"

#undef ENV_SUPPRESS_PUSH

#include "env_messages.hpp"


#include "env_namespacing.hpp"

#include "env_bench.hpp"
#include "env_mock.hpp"
#include "env_perf.hpp"
#include "env_test.hpp"

#include "env_features.hpp"
#include "env_glue.hpp" // TODO: remove this ASAP


#endif // ENV_STRUCTURE_HPP
