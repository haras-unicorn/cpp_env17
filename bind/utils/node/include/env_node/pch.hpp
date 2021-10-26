#ifndef ENV_BIND_NODE_PCH
#define ENV_BIND_NODE_PCH


#include <env/env.hpp>

#if ENV_NODE_NAN // abstractions
    #include <nan.h>
#elif ENV_NODE_NAPI // abstractions
    #include <napi.h>
#else // abstractions
    #error "No abstractions defined for Node bindings."
#endif


#endif // ENV_BIND_NODE_PCH
