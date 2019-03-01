#include <stdio.h>

int main(){

int frequency[26];
int ch;
FILE* txt_file = fopen ("gdb.txt", "rt");
FILE * output = fopen("output.txt","a");
 
/* init the freq table: */
for (ch = 0; ch < 26; ch++)
    frequency[ch] = 0;
 
while (1) {
    ch = fgetc(txt_file);
    if (ch == EOF) break; /* end of file or read error.  EOF is typically -1 */
 
    /* assuming ASCII; "letters" means "a to z" */
    if ('a' <= ch && ch <= 'z')      /* lower case */
        frequency[ch-'a']++;
    
	}
	for (ch = 0; ch < 26; ch++)
		fprintf(output,"%c %d\n\r",ch+'a',frequency[ch]);

	fclose(txt_file);
	fclose(output);
}