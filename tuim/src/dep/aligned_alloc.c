#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 201112L
   #define DO_NOT_HAVE_ALIGNED_ALLOC 1
#elif defined(__ANDROID__)
   #define DO_NOT_HAVE_ALIGNED_ALLOC 1
#else
   #define DO_NOT_HAVE_ALIGNED_ALLOC 0
   int tuim_ignore_this = 0;
#endif

#if DO_NOT_HAVE_ALIGNED_ALLOC
   #if defined(_POSIX_C_SOURCE)
      #include "aligned_alloc-posix.c"
   #elif defined(_WIN32)
      #include "aligned_alloc-win.c"
   #endif
#endif
