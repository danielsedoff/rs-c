#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void help(void);

int main(int argc, char *argv[]){

 char c, cc, *filename, *textContent;
 unsigned long minimumWordLength, fsize, textContenttLength, counter, currentChar, copyPosition, wordSize, longestWord;
 double progressPc;
 
 if(argc < 2 || argc > 3){
  help();
  return 0;
 }

 if(argc == 3) { 
  minimumWordLength = (int) strtol(argv[2], NULL, 0);
 }
 
 if(minimumWordLength == 0L) minimumWordLength = 1; // default
 
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
 textContent = (char*)malloc(fsize + 1);

 if(textContent == NULL) {
  printf("Error: cannot allocate %ld bytes of memory.\n", fsize + 1);
  return 0;
 }
 
 fread(textContent, fsize, 1, f);
 fclose(f);
 textContent[fsize] = 0;
 textContenttLength = strlen(textContent);
 printf("\nThe file <%s> is %ld bytes long.\n", filename, textContenttLength);
 printf("If it takes too long, try to change the MINLENGTH parameter.\n");
 
 if(textContenttLength < 2) {
  return 0;
 }
 
 
 for(currentChar = 0; currentChar < textContenttLength - 1; ++currentChar) {
  progressPc = 100. * (double)currentChar / (double)textContenttLength;
  c = textContent[currentChar];
  longestWord = 0;
  for(copyPosition = currentChar + 1; copyPosition < textContenttLength; ++copyPosition) {
   if(c == textContent[copyPosition]){
    wordSize = 1;
    while(currentChar + wordSize < copyPosition && copyPosition + wordSize < textContenttLength && textContent[currentChar + wordSize] == textContent[copyPosition + wordSize]){
     ++wordSize;
    }
    if (wordSize > longestWord) longestWord = wordSize;
   }
  }
  if(longestWord > 1){
   if(longestWord >= minimumWordLength) {
    printf("\n%.2f%% - ", progressPc);
    if (currentChar == 0) {
		counter = 0; 
		--longestWord;
	} else {
		counter = currentChar - 1;
	}
    for(; counter < currentChar + longestWord && counter < textContenttLength; ++counter) {
	 cc = textContent[counter];
	 if(cc == '\n' || cc == '\t') cc = ' ';
	 printf("%c", cc);
    }
   }
   currentChar += longestWord;
  }
 }
 return 0;
}

void help(){
  printf("\nrs: find repeated strings in file and output to stdout. \n");
  printf("Usage:     rs FILE [MINLENGTH]\n\n");
  printf("FILE:      a plain text file no greater than your RAM size.\n");
  printf("MINLENGTH: minimum repeated sequence length.\n");
  printf("Contact:   Daniel Sedoff <danielsedoff@gmail.com>\n\n");
}
