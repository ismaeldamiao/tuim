int isblank(int ch){
   /* FIXME: Check the current locale. */
   if((ch == 0x20) || (ch == 0x09)) return 1; /* space and tab */
   return 0;
}
