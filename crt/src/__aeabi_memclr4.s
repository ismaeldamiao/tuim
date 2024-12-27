.global __aeabi_memclr4
.text

__aeabi_memclr4:
   MOV R2, #0
   B __aeabi_memset4
