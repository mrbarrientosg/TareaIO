//
//  debug.h
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef debug_log_h
#define debug_log_h

#include <stdio.h>
#include <stdlib.h>

#define LOG 0

#if LOG >= 1
#define DEBUG_PRINT(...) do { printf(__VA_ARGS__); } while (0)
#else
#define DEBUG_PRINT(...) do { } while (0)
#endif


#define FATAL_ERROR(message) \
if (1) {\
printf("Fatal Error: %s\nfile: %s\nfunction: %s\nline: %d\n", message, __FILE__, __PRETTY_FUNCTION__, __LINE__); \
abort(); \
}

#define ASSERT(x, message) \
if (!x) { \
printf("assertion failed: %s\nfile: %s\nfunction: %s\nline: %d\n", message, __FILE__, __PRETTY_FUNCTION__, __LINE__); \
abort(); \
}


#endif /* debug_log_h */
