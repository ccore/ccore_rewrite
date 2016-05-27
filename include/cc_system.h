/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

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
