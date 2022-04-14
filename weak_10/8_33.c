#include <stdio.h>
int main() {
unsigned long virtual_address;
scanf("%lu", &virtual_address);
printf("The address %lu contains:\npage number = %lu\noffset = %lu\n",
virtual_address, virtual_address/(4*1024), virtual_address%(4*1024));
return 0;
}

