include(ENVTargets)
include(ENVFetch)

env_log("-!- Adding dependencies... -!-")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

include(ENVStd)
include(ENVTest)
include(ENVBench)
include(ENVAlloc)
include(ENVDefault)
