#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {

  int num;
  FILE *fptr;

  fptr = fopen("numbers.txt", "w");

  if(fptr == NULL) {
    printf("Failed to open, exiting \n");
    return 1;
  }

  /* read from stdin */
  printf("Give me a number: \n");
  scanf("%d", &num);

  /* write to file */
  fprintf(fptr, "%d\n", num);
  fclose(fptr);
  return 0;
  
}
