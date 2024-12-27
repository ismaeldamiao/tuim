int isalpha(int ch){
   /* FIXME: Check the current locale. */
   if((ch > 0x40) && (ch < 0x5b)) return 1; /* upper case */
   if((ch > 0x60) && (ch < 0x7a)) return 1; /* lower case */
   return 0;
}
