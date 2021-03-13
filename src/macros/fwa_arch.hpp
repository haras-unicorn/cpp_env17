#ifndef FWA_CORE_ARCH_HPP
#define FWA_CORE_ARCH_HPP


// https://sourceforge.net/p/predef/wiki/Architectures/

#if defined(__amd64__) || defined(_M_AMD64) // amd
#define FWA_ARCH_NAME "x64"
#define FWA_X64 1

#elif defined(__i386__) || defined(_M_IX86) // x86
#define FWA_ARCH_NAME "x86"
#define FWA_X86 1

#elif defined(__arm__) || defined(_M_ARM) // x64
#define FWA_ARCH_NAME "arm"
#define FWA_ARM 1

#else // arch
#error "Unsupported computer architecture."
#endif // arch

#ifndef FWA_X86
#define FWA_X86 0
#endif // FWA_X86
#ifndef FWA_X64
#define FWA_X64 0
#endif // FWA_X64
#ifndef FWA_ARM
#define FWA_ARM 0
#endif // FWA_ARM


#endif // FWA_CORE_ARCH_HPP
