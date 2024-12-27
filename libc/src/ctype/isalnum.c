int isalnum(int ch){
   /* FIXME: Check the current locale. */
   if((ch > 0x2f) && (ch < 0x3a)) return 1; /* numeric */
   if((ch > 0x40) && (ch < 0x5b)) return 1; /* upper case */
   if((ch > 0x60) && (ch < 0x7b)) return 1; /* lower case */
   return 0;
}
