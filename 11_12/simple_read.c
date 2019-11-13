#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  int num;
  FILE *fptr;

  fptr = fopen("numbers.txt","r");
  if(fptr == NULL) {
    printf("failed to open file\n");
    return 1;
  }

  fscanf(fptr, "%d", &num);

  printf("read %d from file\n", num);

  fclose(fptr);
  return 0;

}
