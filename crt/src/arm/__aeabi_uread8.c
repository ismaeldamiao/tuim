long long __aeabi_uread8(void *address){
   int value;
   unsigned char *dest = (typeof(dest))(&value);
   const unsigned char *src = address;
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
