long long __aeabi_uwrite8(long long value, void *address){
   unsigned char *dest = address;
   const unsigned char *src = (typeof(src))(&value);
   dest[0] = src[0];
   dest[1] = src[1];
   dest[2] = src[2];
   dest[3] = src[3];
   dest[4] = src[4];
   dest[5] = src[5];
   dest[6] = src[6];
   dest[7] = src[7];
   return value;
}
