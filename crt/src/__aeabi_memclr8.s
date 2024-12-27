.global __aeabi_memclr8
.text

__aeabi_memclr8:
   MOV R2, #0
   B __aeabi_memset8
