#ifndef FWA_CORE_OS_HPP
#define FWA_CORE_OS_HPP

// https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) // win
#define FWA_WIN 1
#ifdef _WIN64 // win
#define FWA_WIN64 1
#define FWA_OS_NAME "Windows x64"
#else // win
#define FWA_WIN32 1
#define FWA_OS_NAME "Windows x86"
#endif // win

#elif __APPLE__ // apple
#define FWA_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR // apple
#define FWA_IOS_SIM 1
#define FWA_OS_NAME "iOS Simulator"
#elif TARGET_OS_IPHONE // apple
#define FWA_IOS 1
#define FWA_OS_NAME "iOS"
#elif TARGET_OS_MAC // apple
#define FWA_MAC 1
#define FWA_OS_NAME "Mac"
#else // apple
#error "Unsupported Apple OS."
#endif // apple

#elif __linux__ // linux
#define FWA_LINUX 1
#ifdef __ANDROID__ // linux
#define FWA_OS_NAME "Android"
#define FWA_ANDROID 1
#else // linux
#define FWA_OS_NAME "Linux"
#endif // linux

#else // os
#error "Unsupported OS."
#endif // os


#ifndef FWA_WIN
#define FWA_WIN 0
#endif // FWA_WIN
#ifndef FWA_WIN32
#define FWA_WIN32 0
#endif // FWA_WIN32
#ifndef FWA_WIN64
#define FWA_WIN64 0
#endif // FWA_WIN64

#ifndef FWA_APPLE
#define FWA_APPLE 0
#endif // FWA_APPLE
#ifndef FWA_IOS_SIM
#define FWA_IOS_SIM 0
#endif // FWA_IOS_SIM
#ifndef FWA_IOS
#define FWA_IOS 0
#endif // FWA_IOS
#ifndef FWA_MAC
#define FWA_MAC 0
#endif // FWA_MAC

#ifndef FWA_LINUX
#define FWA_LINUX 0
#endif // FWA_LINUX
#ifndef FWA_ANDROID
#define FWA_ANDROID 0
#endif // FWA_ANDROID


#endif // FWA_CORE_OS_HPP
