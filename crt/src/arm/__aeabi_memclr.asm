.global __aeabi_memclr
.type   __aeabi_memclr, STT_FUNC

// void __aeabi_memclr(void *dest, size_t n){ __aeabi_memset(dest, n, 0); }

.text
   .balign 16
   __aeabi_memclr:
      MOV R2, #0
      B __aeabi_memset
