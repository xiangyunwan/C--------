#include <stdio.h> 
#include <stdlib.h> 

void bin_out(int temp)
{
char bin[128];  
itoa(temp,bin,2); 
printf("%s\n",bin); 
}
void main() 
{ 
unsigned char a;
a=8;
a>>=1;
bin_out(a);
bin_out(1);
system("pause");
}
