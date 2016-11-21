#ifdef __linux
#ifndef ANDROID
#define LINUX_PLATFORM
#define PLATFORM_PATH platform/linux/
#endif
#endif


#ifdef ANDROID
#define ANDROID_PLATFORM
#define PLATFORM_PATH platform/android/
#endif


#ifdef _WIN32
#define WINDOWS_PLATFORM
#define PLATFORM_PATH platform/windows/
#endif


#ifdef __APPLE_OS__
#define MACOS_PLATFORM
#define PLATFORM_PATH platform/macos/
#endif


#ifdef __FreeBSD__
#define FREEBSD_PLATFORM
#define PLATFORM_PATH platform/freebsd/
#endif


#define PLATFORM_IDENT(x) x
#define PLATFORM_XSTR(x) #x
#define PLATFORM_STR(x) PLATFORM_XSTR(x)
#define PLATFORM_HEADER(y) PLATFORM_STR(PLATFORM_IDENT(PLATFORM_PATH)PLATFORM_IDENT(y))


#if _WIN32 || _WIN64
#if _WIN64
#definr CPU64
#else
#define CPU32
#endif
#endif


