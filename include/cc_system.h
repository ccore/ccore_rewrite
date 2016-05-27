#ifndef __CC_SYSTEM_H__
#define __CC_SYSTEM_H__

/* Detect OS versions */
#if defined __linux__
#define CC_OS_LINUX
#elif defined _WIN32
#define CC_OS_WIN32
#elif defined __APPLE__
#define CC_OS_OSX
#else
#error "OS not supported"
#endif

#endif /* __CC_SYSTEM_H__ */
