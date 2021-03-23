Write-Output "`n`n *** Building Release... *** `n`n"
conan build . -bf="./builds/release"

Write-Output "`n`n *** Building Debug... *** `n`n"
conan build . -bf="./builds/debug"

Write-Output "`n`n *** Building Coverage... *** `n`n"
conan build . -bf="./builds/coverage"

Write-Output "`n`n *** Building GCC... *** `n`n"
conan build . -bf="./builds/gcc"

Write-Output "`n`n *** Building ClangCL... *** `n`n"
conan build . -bf="./builds/clang-cl"

Write-Output "`n`n *** Building MSVC... *** `n`n"
conan build . -bf="./builds/msvc"
