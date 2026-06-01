#pragma once
#include <stdio.h>
#include <stdlib.h>

#define RETURN_CHECK_0(ptr) \
     if((ptr) == NULL) \
        {return 0;}

#define CHECK_MSG_RETURN_0(ptr, msg) \
    if ((ptr) == NULL) { \
        printf("%s\n", msg); \
        return 0; \
    }

#define FREE_CLOSE_FILE_RETURN_0(ptr, fp) \
    if (ptr != NULL) { \
        free(ptr); \
        ptr = NULL;\
    } \
    if (fp != NULL) { \
        fclose(fp); \
    } \
    return 0;

#define CLOSE_RETURN_0(fp) \
    if (fp != NULL) { \
        fclose(fp); \
    } \
    return 0;