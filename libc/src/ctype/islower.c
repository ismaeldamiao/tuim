int islower(int ch){
   /* FIXME: Check the current locale. */
   if((ch > 0x60) && (ch < 0x7a)) return 1; /* lower case */
   return 0;
}
