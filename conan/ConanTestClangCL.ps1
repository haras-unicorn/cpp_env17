conan install . -pr="clang-cl_release" -if="./builds/clang-cl" --update --build=missing
conan build . -bf="./builds/clang-cl"
