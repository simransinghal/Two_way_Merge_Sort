#include<iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <fstream>

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

void add_to_OutputFile(char **words, char filename[] ,int n) {

  std::ofstream myfile(filename);
  if (myfile.is_open())
  {
    int i = 0;

    for(i; i < n; i++)
      myfile << words[i] << '\n';

    myfile.close();
  }

}

int small_substring(char *wfirst, char *wsecond)
{
  for(int z = 0; z < arguments - 5; z++)
    {
      int substring_ind = 0;
      char subString1[strlen(wfirst)], subString2[strlen(wsecond)];
      for(int k = 0; k < col_arg[z]; k++)
      {
        substring_ind = substring_ind + attr_ptr[k].size + 2;
      }

      strncpy(subString1, wfirst + substring_ind, attr_ptr[col_arg[z]].size);
      subString1[substring_ind + attr_ptr[col_arg[z]].size] = '\0';

      strncpy(subString2, wsecond + substring_ind, attr_ptr[col_arg[z]].size);
      subString2[substring_ind + attr_ptr[col_arg[z]].size] = '\0';

      if ((strcmp (subString1, subString2) > 0 && asc == 1) || (strcmp (subString1, subString2) < 0 && asc == 0))
      {
          return 2;
          break;
      }
      else if((strcmp (subString1, subString2) < 0 && asc == 1) || (strcmp (subString1, subString2) > 0 && asc == 0))
        {
          return 1;
          break;
        }
    }
}


void merge_files(int ct_files, char out_file[], int BUFSIZE)
{
  std::ofstream myfile(out_file);

  FILE *f[ct_files];
  int i = 0;
  for(i; i < ct_files; i++)
  {
    char filename[20];
    sprintf(filename, "%s%d%s", "out", i, ".txt");
    FILE *f[i] = fopen(filename, "r");
  }

while(1)
{

  FILE *first, *second;
  first = f[0];
  wfirst = (char *)malloc(BUFSIZE);
  wsecond = (char *)malloc(BUFSIZE);

  int first_i = 0;

  for(i = 1; i < ct_files; i++)
  {
    if (fgets(wfirst, BUFSIZE, first) == NULL)
      break;

    second = f[i];
    if (fgets(wsecond, BUFSIZE, second) == NULL)
      break;

    int flag = small_substring(wfirst, wsecond);

    if(flag == 2)
    {
      first = second;
      first_i = i;
    }
  }

  f[first_i] = first;
  myfile << wfirst << '\n';
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

FILE *input = fopen(argv[1], "r");
if(input == NULL)
{
  std::cout << "Error: Could not open file input.txt" << std::endl;
  exit (2);
}

i = 0;
char **words = (char**)malloc(no_Load_Records * sizeof(char*));
words[i] = (char *)malloc(BUFSIZE);
int ct_load_input = 0;
int ct_output_files = 0;

while (fgets(words[i], BUFSIZE, input)) {
  words[i][strlen(words[i]) - 1] = '\0';

  if (i + 1 == no_Load_Records || ct_load_input + 1 == ct_lines_input)
  {
    NewSort(words, col_arg, 0, i + 1, asc);
    char out_filename[20];
    sprintf(out_filename, "%s%d%s", "out", ct_output_files, ".txt");
    ct_output_files++;
    add_to_OutputFile(words, out_filename, i + 1);
  }

  if(ct_load_input + 1 == ct_lines_input)
  {
    merge_files(ct_output_files, arg[2], BUFSIZE);
    fclose(input);
    exit (1);
  }

  if(i + 1 == no_Load_Records)
  {
    int j = 0;
    for(j; j < i + 1; j++)
      free(words[j]);

    i = -1;
  }

  i = i + 1;
  ct_load_input++;
  words[i] = (char *)malloc(BUFSIZE);
 }
fclose(input);

  return 0;
}
