include(ENVTargets)
include(ENVFetch)
conan_basic_setup(TARGETS)

env_log("-!- Adding dependencies... -!-")

include(ENVStd)
include(ENVTest)
include(ENVBench)
include(ENVAlloc)
include(ENVDefault)
