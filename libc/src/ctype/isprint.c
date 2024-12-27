int isprint(int ch){
   /* FIXME: Check the current locale. */
   if((ch > 0x1f) && (ch < 0x7f)) return 1;
   return 0;
}
