int iscntrl(int ch){
   /* FIXME: Check the current locale. */
   if(((ch >= 0x00) && (ch < 0x20)) || (ch == 0x7f)) return 1;
   return 0;
}
