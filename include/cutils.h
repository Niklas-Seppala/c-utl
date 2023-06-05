#if !defined(CTUL_H)
#define CTUL_H
#include "stdio.h"
#include "stdlib.h"

#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif

#define UNUSED_PARAMETER(x) (void)(x)
#define NOT_IMPLEMENTED { printf("Not implemented %s:%d %s()\n", __FILE__, __LINE__, __FUNCTION_NAME__); exit(EXIT_FAILURE); }
#endif


// Empty macro implementation.
#define UNDEFINED

#endif // CTUL_H
