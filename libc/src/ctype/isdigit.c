int isdigit(int ch){
   /* FIXME: Check the current locale. */
   if((ch > 0x2f) && (ch < 0x3a)) return 1; /* numeric */
   return 0;
}
