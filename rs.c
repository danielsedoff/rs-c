#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void help(void);

int main(int argc, char *argv[]){

 char c, cc, *filename, *tx;
 unsigned long min_word, fsize, txLen, ctr, currChar, copyCh, wrd, longestWd;
 double progressPc;
 
 if(argc < 2 || argc > 3){
  help();
  return 0;
 }

 if(argc == 3) { 
  min_word = (int) strtol(argv[2], NULL, 0);
 }
 
 if(min_word == 0L) min_word = 1; // default
 
 filename = argv[1];
 if(access(filename, F_OK) == -1) {
  printf("\nSorry, file <%s> does not exist.\n", filename);
  help();
  return 0;
 }
 
 FILE *f = fopen(filename, "rb");
 fseek(f, 0, SEEK_END);
 fsize = ftell(f);
 fseek(f, 0, SEEK_SET);
 c = '0';
 tx = (char*)malloc(fsize + 1); 

 if(tx == NULL) {
  printf("Sorry, cannot allocate %ld bytes of memory.\n", fsize + 1);
  return 0;
 }
 
 fread(tx, fsize, 1, f);
 fclose(f);
 tx[fsize] = 0;
 txLen = strlen(tx);
 printf("\nThe file <%s> is %ld bytes long.\n", filename, txLen);
 printf("If it takes too long, try to change the MINLENGTH parameter.\n");
 
 
 if(txLen < 2) {
  return 0;
 }
 
 
 for(currChar = 0; currChar < txLen - 1; ++currChar) {
  progressPc = 100. * (double)currChar / (double)txLen;
  c = tx[currChar];
  longestWd = 0;
  for(copyCh = currChar + 1; copyCh < txLen; ++copyCh) {
   if(c == tx[copyCh]){
    wrd = 1;
    while(currChar + wrd < copyCh && copyCh + wrd < txLen && tx[currChar+wrd]==tx[copyCh+wrd]){
     ++wrd;
    }
    if (wrd > longestWd) longestWd = wrd;
   }
  }
  if(longestWd > 1){
   if(longestWd >= min_word) {
    printf("\n%.2f%% - ", progressPc);
    if (currChar == 0) {
		ctr = 0; 
		--longestWd;
	} else {
		ctr = currChar - 1;
	}
    for(; ctr < currChar + longestWd && ctr < txLen; ++ctr) {
	 cc = tx[ctr];
	 if(cc == '\n' || cc == '\t') cc = ' ';
	 printf("%c", cc);
    }
   }
   currChar += longestWd;
  }
 }
 return 0;
}

void help(){
  printf("\nrs: find repeated strings in fileand output to stdout. \n");
  printf("USAGE: rs FILE [MINLENGTH]\n\n");
  printf("FILE should be a plain text file no greater than your RAM size.\n");
  printf("[MINLENGTH] is the minimum repeated sequence length.\n");
  printf("Author contact: Daniel Sedoff <danielsedoff@gmail.com>\n\n");
}
