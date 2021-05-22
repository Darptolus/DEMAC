/**
 * @file DECARD_tools.hpp
 * @author Diego Roa
 * @date 1 Apr 2021
 * @brief Different helper functions and macros for error handling and other commonly used operations
 * @todo Add copyright
 *
 * Includes the definition of the Runtime
 */

#ifndef __DECARD_TOOLS__
#define __DECARD_TOOLS__
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef VERBOSE_MODE
#define VERBOSE_MODE -1
#endif

// Macro for output of information, warning and error messages
#ifdef FNAME 
  #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
  #define __FILENAME__ ""
#endif
#if VERBOSE_MODE >= 0
  #define DECARD_WARNING(level, message, ...) { \
    if(VERBOSE_MODE >= level) {\
      printf("<[D_W]: %s:%i> " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }
  #define DECARD_WARNING_IF(level, condition, message, ...) { \
    if(VERBOSE_MODE >= level && condition) { \
      printf("<[D_W]: %s:%i> " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }
  
  #define DECARD_ERROR(level, message, ...) { \
    if(VERBOSE_MODE >= level) {\
      fprintf(stderr, "<[D_E]: %s:%i> " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }
  #define DECARD_ERROR_IF(level, condition, message, ...) { \
    if(VERBOSE_MODE >= level && condition) { \
      fprintf(stderr, "<[D_E]: %s:%i> " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }
  
  #define DECARD_INFOMSG(level, message, ...) { \
    if(VERBOSE_MODE >= level) {\
      printf("<[D_I]: %s:%i> " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }
  #define DECARD_INFOMSG_IF(level, condition, message, ...) { \
    if(VERBOSE_MODE >= level && condition) { \
      printf("<[D_I]: %s:%i> " message "\n", __FILENAME__, __LINE__, ##__VA_ARGS__); \
    } \
  }
#else
  #define DECARD_WARNING(level, message, ...) {}
  #define DECARD_WARNING_IF(level, message, ...) {}
  #define DECARD_ERROR(level, message, ...) {}
  #define DECARD_ERROR_IF(level, message, ...) {}
  #define DECARD_INFOMSG(level, message, ...) {}
  #define DECARD_INFOMSG_IF(level, message, ...) {}
#endif // END IF VERBOSE_MODE


#endif // __DECARD_TOOLS__
