env_add_dep(std)
env_target_precompile(
        std_dep
        INTERFACE
        <utility>
        <type_traits>

        <memory>
        <new>
        <cstring>
        <iterator>

        <bitset>

        <any>
        <optional>
        <tuple>

        <string>
        <string_view>

        <stack>
        <deque>
        <queue>

        <list>
        <forward_list>

        <array>
        <vector>

        <set>
        <map>
        <unordered_map>
        <unordered_set>

        <thread>
        <mutex>
        <shared_mutex>
        <atomic>
        <chrono>

        <cmath>
        <functional>
        <algorithm>
        <exception>

        <iostream>
        <filesystem>
)
