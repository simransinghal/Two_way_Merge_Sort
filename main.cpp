#include<iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>

struct attribute{
  char name[100];
  int size;
};

struct attribute *attr_ptr;
int arguments = 0;

void NewSort(char **words, int col_arg[], int start, int end, int asc)
{
  for(int i = 0; i < end; i++)
  {
    for(int j = i + 1; j < end; j++)
    {
      for(int z = 0; z < arguments - 5; z++)
      {
      int substring_ind = 0;
      char subString1[strlen(words[i])], subString2[strlen(words[j])];
      for(int k = 0; k < col_arg[z]; k++)
      {
        substring_ind = substring_ind + attr_ptr[k].size + 2;
      }

      strncpy(subString1, words[i] + substring_ind, attr_ptr[col_arg[z]].size);
      subString1[substring_ind + attr_ptr[col_arg[z]].size] = '\0';

      strncpy(subString2, words[j] + substring_ind, attr_ptr[col_arg[z]].size);
      subString2[substring_ind + attr_ptr[col_arg[z]].size] = '\0';

      if ((strcmp (subString1, subString2) > 0 && asc == 1) || (strcmp (subString1, subString2) < 0 && asc == 0))
      {
              char *temp;
              temp =  words[i];
              words[i] = words[j];
              words[j] = temp;
              break;
      }
      else if((strcmp (subString1, subString2) < 0 && asc == 1) || (strcmp (subString1, subString2) > 0 && asc == 0))
        break;
    }
  }
  }
}

int main(int argc, char *argv[])
{
  arguments = argc;

  long long int main_memory = atoi(argv[3]);
  main_memory = main_memory*1024*1024;

  FILE *metadata;
  metadata = fopen("metadata.txt", "r");
  if(metadata == NULL)
  {
    std::cout << "Error: Could not open file metadata.txt" << std::endl;
    exit (1);
  }

  int metadata_lines = 0;
  for(char c = getc(metadata); c != EOF; c = getc(metadata))
  {
    if (c == '\n')
      metadata_lines++;
  }
  fclose(metadata);

  int asc = 0;
  std::string a = "asc";
  if(argv[4] == a)
    asc = 1;

  //struct attribute attr_ptr[metadata_lines];
  //for(int i = 0; i < metadata_lines; i++)
  attr_ptr = (struct attribute *)malloc(sizeof(struct attribute) * metadata_lines);

  metadata = fopen("metadata.txt", "r");
  char each_line[256];
  int i = 0;
  while (fgets(each_line, sizeof(each_line), metadata))
  {
    char *pt;
    pt = strtok (each_line,",");
    strcpy(attr_ptr[i].name, pt);
    pt = strtok (NULL, ",");
    attr_ptr[i].size = atoi(pt);
    i = i + 1;
  }
  fclose(metadata);

  long long int record_size = 0;
  for(i = 0; i < metadata_lines; i++)
    record_size = record_size + attr_ptr[i].size;

  long long int no_Load_Records = (main_memory)/record_size;

  int col_arg[argc - 5];
  for(i = 5; i < argc; i++)
  {
    for(int j = 0; j < metadata_lines; j++)
    {
      if(strcmp(argv[i], attr_ptr[j].name) == 0)
      {
        col_arg[i - 5] = j;
        break;
      }
    }
  }


int BUFSIZE = record_size + 4*metadata_lines;

FILE *f = fopen(argv[1], "r");
char w[BUFSIZE];
int ct_lines_input = 0;

while (fgets(w, BUFSIZE, f)) {
  ct_lines_input++;
}
fclose(f);

char **words = (char**)malloc(no_Load_Records * sizeof(char*));
FILE *input = fopen(argv[1], "r");

if(input == NULL)
{
  std::cout << "Error: Could not open file input.txt" << std::endl;
  exit (2);
}

i = 0;
words[i] = (char *)malloc(BUFSIZE);
int ct_load_input = 0;
while (fgets(words[i], BUFSIZE, input)) {
  words[i][strlen(words[i]) - 1] = '\0';

  if (i + 1 == no_Load_Records || ct_load_input + 1 == ct_lines_input)
  {
    NewSort(words, col_arg, 0, i + 1, asc);
    for(int j = 0; j < i + 1  ; j++)
      std::cout << words[j] << std::endl;
    break;
  }
  i = i + 1;
  ct_load_input++;
  words[i] = (char *)malloc(BUFSIZE);
 }
fclose(input);

  return 0;
}
