Write-Output "`n`n *** Installing Release... *** `n`n"
	conan install . -pr="clang_release" -if="./builds/release"  --build=missing

Write-Output "`n`n *** Installing Debug... *** `n`n"
conan install . -pr="clang_debug" -if="./builds/debug"  --build=missing

Write-Output "`n`n *** Installing Coverage... *** `n`n"
conan install . -pr="clang_coverage" -if="./builds/coverage" --build=missing

Write-Output "`n`n *** Installing GCC... *** `n`n"
conan install . -pr="gcc_release" -if="./builds/gcc" --build=missing

Write-Output "`n`n *** Installing ClangCL... *** `n`n"
conan install . -pr="clang-cl_release" -if="./builds/clang-cl" --build=missing

Write-Output "`n`n *** Installing MSVC... *** `n`n"
conan install . -pr="msvc_release" -if="./builds/msvc" --build=missing
