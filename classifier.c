#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this part goes in hand with a part of prunning low probability counts
// this functions main goal is to quantify each read by the maximum post and prefix of each read, given that they comply
int main(void)
{
  FILE *inp = fopen("TEST.fasta", "r");
  FILE *reads;
  FILE *pres;
  FILE *suffs;
  
  reads = fopen("READ.txt", "w");
  pres = fopen("PREFIX.txt", "w");
  suffs = fopen("SUFFIX.txt", "w");
  
  // still need a newline at the end of the file regardless
  
  unsigned long min = 0xffffffffffffff; 
  char hold;
  unsigned long s;
  unsigned long e;
  unsigned long nRead = 0;
  
  while (!feof(inp)) // this part finds the minimum read, ie the maximum to have full read coverage
  {
    fscanf(inp, ">%*[^\n]\n");
    s = ftell(inp);
    fscanf(inp, "%*[^\n]\n");
    e = ftell(inp);
    if ((e - s - 1) < min)
    {
      min = e - s - 1;
    }
    nRead++;
  }
  
  printf("Number of reads is: %lu\n", nRead);
  printf("Minimum read Length is: %lu\n", min);
  rewind(inp); // and close by resetting the cursor on the input file

  // lots of redundancies in this code
  // free some memory to find the read lengths
  unsigned int *lengths = malloc(sizeof(unsigned int) * nRead);
  for (int i = 0; i < nRead; i++)
  {
    fscanf(inp, ">%*[^\n]\n");
    s = ftell(inp);
    fscanf(inp, "%*[^\n]\n");
    e = ftell(inp);
    lengths[i] = e - s - 1;
  }
  
  rewind(inp);
  
  
  while (!feof(inp)) // redundant while looping but I will do it anyways
  {
    fscanf(inp, ">%*[^\n]\n");
    fscanf(inp, "%c", &hold);
    if (hold == 10 && !feof(inp))
    {
      // if equals newline print an & into the file to buffer
      // this is a workaround that might be expensive...
      fprintf(reads, "&");
    }
    if (!feof(inp))
    {
      fprintf(reads, "%c", hold); 
    }
  }
  
  fclose(inp);
  fclose(reads);
  
  // now try to extract the prefixes from each of the files, this is the easy part, the suffix is more difficult
  reads = fopen("READ.txt", "r");
  char holdTwo;
  while (!feof(reads))
  {
    for (int i = 0; i < min; i++)
    {
      fscanf(reads, "%c", &holdTwo);
      fprintf(pres, "%c", holdTwo);
    }
    fscanf(reads, "%*[^\n]\n");
    fprintf(pres, "\n");
  }
  
  rewind(reads);
  char hold3;
  
  for (int i = 0; i < nRead; i++)
  {
    // these are where each should start to print into the 
    fseek(reads, lengths[i] - min, SEEK_CUR);
    for (int j = 0; j < min; j++)
    {
      fscanf(reads, "%c", &hold3);
      fprintf(suffs, "%c", hold3);
    }
    fscanf(reads, "%*[^\n]\n");
    fprintf(suffs, "\n");
  }
  
  
  fclose(suffs);
  fclose(reads);
  fclose(pres);
  free(lengths);
  return(0);
}