include(FWAProperties)
include(FWAFetch)

fwa_log("-!- Adding dependencies... -!-")

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


fwa_log("-!- Pruning dependencies... -!-")

include(FWAExclude)

fwa_prune_directories(${PROJECT_SOURCE_DIR})
