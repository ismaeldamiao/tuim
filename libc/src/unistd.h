#ifndef _UNISTD_H
#define _UNISTD_H

#if defined(__linux__)
#include "unistd-linux.h"
#else
#error Unknown kernel.
#endif

#endif
