int __aeabi_uread4(void *address){
   int value;
   register unsigned char *dest = (typeof(dest))(&value);
   register const unsigned char *src = address;
   dest[0] = src[0];
   dest[1] = src[1];
   dest[2] = src[2];
   dest[3] = src[3];
   return value;
}
