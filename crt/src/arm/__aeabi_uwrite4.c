int __aeabi_uwrite4(int value, void *address){
   unsigned char *dest = address;
   const unsigned char *src = (typeof(src))(&value);
   dest[0] = src[0];
   dest[1] = src[1];
   dest[2] = src[2];
   dest[3] = src[3];
   return value;
}
