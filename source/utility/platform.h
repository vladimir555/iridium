#ifndef HEADER_PLATFORM_0A9DC04_5F20_4C44_A56B_5834C387CC13
#define HEADER_PLATFORM_0A9DC04_5F20_4C44_A56B_5834C387CC13


#ifdef __linux
#ifndef ANDROID
#define LINUX_PLATFORM
#undef linux
#undef unix
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
//#if _WIN32 || _WIN64
//#if _WIN64
//#define CPU64
//#else
//#define CPU32
//#endif
//#endif
#endif


#ifdef __APPLE_OS__
#define MACOS_PLATFORM
#define PLATFORM_PATH platform/macos/
#endif


#ifdef __APPLE__
#define MACOS_PLATFORM
#define PLATFORM_PATH platform/macos/
#endif


#ifdef __FreeBSD__
#define FREEBSD_PLATFORM
#define PLATFORM_PATH platform/freebsd/
#endif


#if defined(LINUX_PLATFORM) || defined(FREEBSD_PLATFORM) || defined(MACOS_PLATFORM)
#define UNIX_PLATFORM
#endif


#if defined(FREEBSD_PLATFORM) || defined(MACOS_PLATFORM)
#define FREEBSD_LIKE_PLATFORM
#endif


#define PLATFORM_IDENT(x) x
#define PLATFORM_XSTR(x) #x
#define PLATFORM_STR(x) PLATFORM_XSTR(x)
#define PLATFORM_HEADER(file_name) PLATFORM_STR(PLATFORM_IDENT(PLATFORM_PATH)PLATFORM_IDENT(file_name))


#if _WIN32 || _WIN64
   #if _WIN64
     #define PLATFORM_CPU64
  #else
    #define PLATFORM_CPU32
  #endif
#endif


// Check GCC
#if __GNUC__
  #if defined(__x86_64__) || defined(__ppc64__)
    #define PLATFORM_CPU64
  #else
    #define PLATFORM_CPU32
  #endif
#endif


#endif // HEADER_PLATFORM_0A9DC04_5F20_4C44_A56B_5834C387CC13
