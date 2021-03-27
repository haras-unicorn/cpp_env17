#ifndef ENV_INTRINSIC_HPP
#define ENV_INTRINSIC_HPP


// https://docs.microsoft.com/en-us/cpp/intrinsics/compiler-intrinsics?view=msvc-160

#if ENV_X86 // intrinsics

// pause
#include <immintrin.h>

#elif ENV_X64 // intrinsics

// pause
#include <immintrin.h>

#elif ENV_ARM // intrinsics

#elif ENV_ARM64 // intrinsics

#else // intrinsics
#error "Unsupported intrinsics."
#endif // intrinsics


#endif // ENV_INTRINSIC_HPP
