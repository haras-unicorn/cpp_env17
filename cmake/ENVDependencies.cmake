include(ENVProperties)
include(ENVFetch)

env_log("-!- Adding dependencies... -!-")

include(ENVStd)
include(ENVBoost)
include(ENVDoctest)

include(ENVMimalloc)
include(ENVAsio)
include(ENVPortAudio)
include(ENVJuce)

include(ENVJson)
include(ENVNode)

include(ENVDefault)


env_log("-!- Pruning dependencies... -!-")

include(ENVExclude)

env_prune_directories(${PROJECT_SOURCE_DIR})
