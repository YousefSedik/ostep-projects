#include <stdio.h>
#include <stdlib.h>
const int BUFFER_SIZE = 100; 
int main(int argc, char * argv[]){
  if (argc < 2){
    return 0;
  }
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < argc - 1; i++){
    FILE *fp = fopen(argv[i + 1], "r");
    if (fp == NULL) {
      printf("wcat: cannot open file\n");
      exit(1);
    }    
    while (fgets(buffer, BUFFER_SIZE, fp)){
      printf("%s", buffer);
    }
    fclose(fp);
  }
  return 0;
}
