#if !defined(_POSIX_C_SOURCE)
   #define DO_NOT_HAVE_MPROTECT 1
#else
   #define DO_NOT_HAVE_MPROTECT 0
   int tuim_ignore_this = 0;
#endif

#if DO_NOT_HAVE_MPROTECT
   #if defined(_WIN32)
      #include "aligned_alloc-win.c"
   #endif
#endif
