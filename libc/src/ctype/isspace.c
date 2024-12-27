int isspace(int ch){
   /* FIXME: Check the current locale. */
   if(
      (ch == 0x20) || (ch == 0x0c) || (ch == 0x0a) ||
      (ch == 0x0d) || (ch == 0x09) || (ch == 0x0b)
   ) return 1;
   return 0;
}
