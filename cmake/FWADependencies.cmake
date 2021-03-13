include(FWAProperties)
include(FWAFetch)

env_log("-!- Adding dependencies... -!-")

include(FWAStd)
include(FWABoost)
include(FWADoctest)

include(FWAMimalloc)
include(FWAAsio)
include(FWAPortAudio)
include(FWAJuce)

include(FWAJson)
include(FWANode)

include(FWADefault)


env_log("-!- Pruning dependencies... -!-")

include(FWAExclude)

env_prune_directories(${PROJECT_SOURCE_DIR})
