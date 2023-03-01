#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this function takes in reads from a suffix file compares them with reads from a prefix file and links

int main(void)
{
  FILE *prefInp;
  FILE *sufInp;
  FILE *opt;
  prefInp = fopen("PREFIX.txt", "r");
  sufInp = fopen("SUFFIX.txt", "r");
  opt = fopen("LINKER.txt", "w");
  
  unsigned int nRead = 50; // we know this from previous function
  unsigned int lenK = 103; // we also know this from previous function
  
  char *holdS = malloc(sizeof(char) * (lenK + 1));
  char *holdP = malloc(sizeof(char) * (lenK + 1));
  char newLineBreak;
  
  // obviously this method is in O(n^2), so not optimal in the slightest
  
  for (int i = 0; i < nRead; i++)
  {
    // this gets all my suffixs I need
    fread(holdS, sizeof(char), lenK, sufInp);
    fread(&newLineBreak, sizeof(char), 1, sufInp); // this is just to get rid of the crafty linebreak
    fprintf(opt, "%i&", i);
    for (int j = 0; j < nRead; j++)
    {
      fread(holdP, sizeof(char), lenK, prefInp);
      fread(&newLineBreak, sizeof(char), 1, prefInp);
      if (strcmp(holdP, holdS) == 0)
      {
        fprintf(opt, "%i&", j);
      }
    }
    fprintf(opt, "\n");
    rewind(prefInp);
  }
  
  free(holdS);
  free(holdP);
  fclose(prefInp);
  fclose(sufInp);
  fclose(opt);
}