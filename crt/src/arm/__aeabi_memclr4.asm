.global __aeabi_memclr4
.type   __aeabi_memclr4, STT_FUNC

// void __aeabi_memclr4(void *dest, size_t n){ __aeabi_memset4(dest, n, 0); }

.text
   .balign 16
   __aeabi_memclr4:
      MOV R2, #0
      B __aeabi_memset4
