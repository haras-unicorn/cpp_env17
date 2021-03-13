env_add_dependency(std INTERFACE)

env_target_precompile(std
        INTERFACE
        <utility>
        <type_traits>

        <bitset>

        <memory>
        <iterator>

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
        <atomic>
        <chrono>

        <functional>
        <algorithm>
        <exception>

        <iostream>
        <filesystem>)
