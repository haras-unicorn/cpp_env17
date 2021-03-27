#ifndef ENV_ARCH_HPP
#define ENV_ARCH_HPP


// https://sourceforge.net/p/predef/wiki/Architectures/
// https://stackoverflow.com/questions/37244202/detect-arm-64-in-preprocessor

#if defined(__amd64__) || defined(_M_AMD64) // x64
#define ENV_ARCH_NAME "x64"
#define ENV_X64 1

#elif defined(__i386__) || defined(_M_IX86) // x86
#define ENV_ARCH_NAME "x86"
#define ENV_X86 1

#elif defined(__arm__) || defined(_M_ARM) // arm
#define ENV_ARCH_NAME "arm"
#define ENV_ARM 1

#elif defined(__aarch64__) || defined(_M_ARM64) // arm64
#define ENV_ARCH_NAME "arm64"
#define ENV_ARM64 1

#else // arch
#error "Unsupported computer architecture."
#endif // arch


#ifndef ENV_X86
#define ENV_X86 0
#endif // ENV_X86
#ifndef ENV_X64
#define ENV_X64 0
#endif // ENV_X64
#ifndef ENV_ARM
#define ENV_ARM 0
#endif // ENV_ARM
#ifndef ENV_ARM64
#define ENV_ARM64 0
#endif // ENV_ARM64


#endif // ENV_ARCH_HPP
