#include <stdio.h>
#include <stdlib.h>
typedef unsigned short u_short; 
typedef unsigned int u_int32; 
typedef unsigned long u_int64; 
void getFname(char* Fname,char** buffer){
	printf("Enter filename: ");
	scanf("%s",Fname);

	FILE *f = fopen(Fname, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 

	char* temp = (char*) malloc(size + 1);
	fread(temp, size, 1, f);
	fclose(f);
	*buffer=temp;
	printf("%s\n",buffer);
}
/*
u_int64 getCode(<type?>*buffer, int count) {
// count = byte/word count 
	register u_int64 total=0;
	while (count--) {

	}
	return XXXX;
} */
int main() {

	/*-
	ask filename from user and put file contents in a buffer and update the count of
	bytes/words.-call getCode()function to calculate code using one's complement.
	*/
	char Fname[128]="";
	char* buffer;
	getFname(Fname,&buffer);
	printf("%s\n",buffer);
	//code = getCode(buffer,count);
	/* use code to validate any user-specified file */
	//getFname(...); 
	//testValidity(code, Fname);
}	