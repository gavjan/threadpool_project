#ifndef _ERR_
#define _ERR_

#include <stdlib.h>


extern void syserr(int bl, const char* fmt, ...);

extern void fatal(const char* fmt, ...);

#endif
