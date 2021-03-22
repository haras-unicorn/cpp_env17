conan install . -pr="clang_release" -if="./builds/release" --update --build=missing
conan install . -pr="clang_debug" -if="./builds/debug" --update --build=missing
conan install . -pr="clang_coverage" -if="./builds/coverage" --update --build=missing
conan install . -pr="gcc_release" -if="./builds/gcc" --update --build=missing
conan install . -pr="clang-cl_release" -if="./builds/clang-cl" --update --build=missing
conan install . -pr="msvc_release" -if="./builds/msvc" --update --build=missing
