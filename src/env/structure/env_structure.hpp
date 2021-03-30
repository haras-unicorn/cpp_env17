#ifndef ENV_STRUCTURE_HPP
#define ENV_STRUCTURE_HPP


#include "env_directives.hpp"


#include "env_tokens.hpp"

#include "env_arch.hpp"
#include "env_intrinsic.hpp"
#include "env_compiler.hpp"
#include "env_os.hpp"
#include "env_standard.hpp"
#include "env_pragma.hpp"
#include "env_messages.hpp"
#include "env_warnings.hpp"


#undef ENV_SUPPRESS_POP
#undef ENV_SUPPRESS_PUSH
#define ENV_SUPPRESS_PUSH

#include "env_suppress.hpp"

#undef ENV_SUPPRESS_PUSH


ENV_MESSAGE(Env Compiler - ENV_COMPILER_NAME, ENV_COMPILER_VER)
ENV_MESSAGE(Env Arch - ENV_ARCH_NAME)
ENV_MESSAGE(Env OS - ENV_OS_NAME)
ENV_MESSAGE(Env Standard - C++ ENV_CPP)


#include "env_namespacing.hpp"

#include "env_bench.hpp"
#include "env_mock.hpp"
#include "env_test.hpp"
#include "env_hana.hpp"

#include "env_features.hpp"
#include "env_glue.hpp" // TODO: remove this ASAP


#endif // ENV_STRUCTURE_HPP
