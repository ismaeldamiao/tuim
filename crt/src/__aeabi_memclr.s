.global __aeabi_memclr
.text

__aeabi_memclr:
   MOV R2, #0
   B __aeabi_memset
