// macros

#include <hedley.h>

// std

#include <utility>
#include <type_traits>

#include <memory>
#include <new>
#include <cstring>
#include <iterator>

#include <bitset>

#include <any>
#include <optional>
#include <variant>
#include <tuple>

#include <string>
#include <string_view>

#include <stack>
#include <deque>
#include <queue>

#include <list>
#include <forward_list>

#include <array>
#include <vector>

#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <chrono>

#include <cmath>
#include <functional>
#include <algorithm>
#include <exception>

#include <iostream>
#include <filesystem>


// boost

#include <boost/hana.hpp>


// test

#include <gtest/gtest.h>
#include <gmock/gmock.h>

// bench

#include <benchmark/benchmark.h>

// alloc

#include <mimalloc.h>

// json

#include <nlohmann/json.hpp>


// functional

#include <immer/box.hpp>

#include <immer/array.hpp>
#include <immer/array_transient.hpp>

#include <immer/vector.hpp>
#include <immer/vector_transient.hpp>
#include <immer/flex_vector.hpp>
#include <immer/flex_vector_transient.hpp>

#include <immer/map.hpp>
#include <immer/map_transient.hpp>
#include <immer/set.hpp>
#include <immer/set_transient.hpp>

#include <immer/algorithm.hpp>


#include <zug/compose.hpp>

#include <zug/run.hpp>
#include <zug/into.hpp>
#include <zug/into_vector.hpp>
#include <zug/tuplify.hpp>

#include <zug/sequence.hpp>
#include <zug/any_state.hpp>
#include <zug/with_state.hpp>
#include <zug/skip.hpp>

#include <zug/reduce.hpp>
#include <zug/maybe_reduced.hpp>
#include <zug/reductor.hpp>

#include <zug/transducer/transducer.hpp>

#include <zug/transducer/iter.hpp>
#include <zug/transducer/range.hpp>
#include <zug/transducer/chain.hpp>
#include <zug/transducer/cycle.hpp>
#include <zug/transducer/count.hpp>
#include <zug/transducer/repeat.hpp>
#include <zug/transducer/product.hpp>
#include <zug/transducer/random_sample.hpp>

#include <zug/transducer/cat.hpp>
#include <zug/transducer/interleave.hpp>
#include <zug/transducer/interpose.hpp>
#include <zug/transducer/partition.hpp>
#include <zug/transducer/partition_by.hpp>

#include <zug/transducer/dedupe.hpp>
#include <zug/transducer/distinct.hpp>
#include <zug/transducer/drop.hpp>
#include <zug/transducer/drop_while.hpp>
#include <zug/transducer/take.hpp>
#include <zug/transducer/take_nth.hpp>
#include <zug/transducer/take_while.hpp>
#include <zug/transducer/remove.hpp>
#include <zug/transducer/filter.hpp>

#include <zug/transducer/each.hpp>
#include <zug/transducer/scan.hpp>

#include <zug/transducer/enumerate.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/map_indexed.hpp>
#include <zug/transducer/mapcat.hpp>
#include <zug/transducer/replace.hpp>

#include <zug/transducer/eager.hpp>
#include <zug/transducer/read.hpp>
#include <zug/transducer/readbuf.hpp>
#include <zug/transducer/write.hpp>
#include <zug/transducer/writebuf.hpp>
#include <zug/transducer/sink.hpp>

#include <zug/transducer/zip.hpp>
#include <zug/transducer/unzip.hpp>
