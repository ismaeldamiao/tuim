int main(void){
   return 42;
}

extern void __libc_main(typeof(&main));
void program(void){ __libc_main(main); }
