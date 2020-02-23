#include <iostream>
#include<stdio.h>

int main()
{
  char buff[] = {'0', '0', '57', '111', '9'};

  FILE* file_open = fopen("lin.bin", "ab+");
  FILE* file_write = fopen("lin.bin", "wb");

  fwrite(buff, 1, 5, file_write);
  fread(buff, 1, 5, file_open);

  return 0;
}