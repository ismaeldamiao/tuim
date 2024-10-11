#if defined(__aarch64__)
# include "aarch64.c"
#elif defined(__arm__)
# include "arm.c"
#elif defined(__x86_64__)
# include "x86_64.c"
#elif defined(__i386__)
# include "i386.c"
#elif defined(__i386__)
#endif // defined(__arm__)
