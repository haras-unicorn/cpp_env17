#ifndef ENV_OS_HPP
#define ENV_OS_HPP


// https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) // win
#define ENV_WIN 1
#ifdef _WIN64 // win
#define ENV_WIN64 1
#define ENV_OS_NAME "Windows x64"
#else // win
#define ENV_WIN32 1
#define ENV_OS_NAME "Windows x86"
#endif // win

#elif __APPLE__ // apple
#define ENV_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR // apple
#define ENV_IOS_SIM 1
#define ENV_OS_NAME "iOS Simulator"
#elif TARGET_OS_IPHONE // apple
#define ENV_IOS 1
#define ENV_OS_NAME "iOS"
#elif TARGET_OS_MAC // apple
#define ENV_MAC 1
#define ENV_OS_NAME "Mac"
#else // apple
#error "Unsupported Apple OS."
#endif // apple

#elif __linux__ // linux
#define ENV_LINUX 1
#ifdef __ANDROID__ // linux
#define ENV_OS_NAME "Android"
#define ENV_ANDROID 1
#else // linux
#define ENV_OS_NAME "Linux"
#endif // linux

#else // os
#error "Unsupported OS."
#endif // os

#ifndef ENV_WIN
#define ENV_WIN 0
#endif // ENV_WIN
#ifndef ENV_WIN32
#define ENV_WIN32 0
#endif // ENV_WIN32
#ifndef ENV_WIN64
#define ENV_WIN64 0
#endif // ENV_WIN64

#ifndef ENV_APPLE
#define ENV_APPLE 0
#endif // ENV_APPLE
#ifndef ENV_IOS_SIM
#define ENV_IOS_SIM 0
#endif // ENV_IOS_SIM
#ifndef ENV_IOS
#define ENV_IOS 0
#endif // ENV_IOS
#ifndef ENV_MAC
#define ENV_MAC 0
#endif // ENV_MAC

#ifndef ENV_LINUX
#define ENV_LINUX 0
#endif // ENV_LINUX
#ifndef ENV_ANDROID
#define ENV_ANDROID 0
#endif // ENV_ANDROID


#endif // ENV_OS_HPP
