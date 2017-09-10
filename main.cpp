#include<iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

int main(int argc, char *argv[])
{
  long long int main_memory = atoi(argv[3]);
  main_memory = main_memory*1024*1024;

  FILE *metadata;
  metadata = fopen("metadata.txt", "r");
  if(metadata == NULL)
  {
    std::cout << "Error: Could not open file metadata.txt" << std::endl;
    exit (1);
  }

  int lines = 0;
  for(char c = getc(metadata); c != EOF; c = getc(metadata))
  {
    if (c == '\n')
      lines++;
  }
  fclose(metadata);

  metadata = fopen("metadata.txt", "r");

  return 0;
}
