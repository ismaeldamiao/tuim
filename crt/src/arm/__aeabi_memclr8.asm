.global __aeabi_memclr8
.type   __aeabi_memclr8, STT_FUNC

// void __aeabi_memclr8(void *dest, size_t n){ __aeabi_memset8(dest, n, 0); }

.text
   .balign 16
   __aeabi_memclr8:
      MOV R2, #0
      B __aeabi_memset8
