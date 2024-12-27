.global __aeabi_read_tp
.type   __aeabi_read_tp, STT_FUNC

.text
   .balign 16
   __aeabi_read_tp:
      MRC P15, 0, R0, C13, C0, 2
      BX LR
