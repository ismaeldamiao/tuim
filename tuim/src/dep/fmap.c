#if defined(_POSIX_C_SOURCE)
   #include "fmap-posix.c"
#elif defined(_WIN32)
   #include "fmap-win.c"
#endif
