int puts(char*);

int main(void){
   if(puts("Ola Mundo!") >= 0)
      return 0;
   else
      return 127;
}
