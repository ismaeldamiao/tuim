int isupper(int ch){
   /* FIXME: Check the current locale. */
   if((ch > 0x40) && (ch < 0x5b)) return 1; /* upper case */
   return 0;
}
