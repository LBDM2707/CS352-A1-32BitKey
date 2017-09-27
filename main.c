#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
typedef unsigned short u_short; 
typedef unsigned int u_int32; 
typedef unsigned long u_int64; 
void getFname(char* Fname,u_int32** buffer, long* count){
	printf("Enter filename: ");
	scanf("%s",Fname);

	FILE *f = fopen(Fname, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 

	u_int32* temp = (u_int32*) malloc(size); // malloc an array of u_int32
	fread(temp, sizeof(u_int32), size/sizeof(u_int32), f);
	fclose(f);
	*count = size;
	*buffer = temp;
}
char* long_to_binary(unsigned long k){//for testing
    static char c[65];
    c[0] = '\0';

    unsigned long val;
    for (val = 1UL << (sizeof(unsigned long)*8-1); val > 0; val >>= 1) {
        strcat(c, ((k & val) == val) ? "1" : "0");
    }
    return c;
}
u_int64 getCode(u_int32* buffer, int count) {
// count = byte/word count 
	register u_int64 total=0;
	register u_int64 max = powl(2,32);
	printf("max   = %s\n",long_to_binary(max) );
	int bytes = 4;
	u_int64 temp32 = 0;
	long pointer = 0; // pointer for buffer.
	while (count-= 4) {
		printf("========================\n");
		printf("Total = %s\n",long_to_binary(total));
		total += (u_int64)buffer[pointer];
		printf("Added = %s\n",long_to_binary(buffer[pointer]));
		printf("Equal = %s\n",long_to_binary(total));
		// handle overflow
		if (total >= max){
			total = total - max +1;
			printf("after = %s\n", long_to_binary(total));
		}
		//increase pointer 
		pointer++;
	}
	printf("flip  = %s\n", long_to_binary(~total));
	printf("========================\n");
	printf("Total hex = %x\n",total);
	printf("32bit hex = %x\n",~total);
	//convert total to code

	return 0;
} 

int main() {
	/*-
	ask filename from user and put file contents in a buffer and update the count of
	bytes/words.-call getCode()function to calculate code using one's complement.
	*/
	char Fname[128]="";
	long count;
	u_int32* buffer;
	getFname(Fname,&buffer,&count);
	//printf("%s\n",Fname);
	//printf("%s\n",buffer);
	//printf("%ld\n",count);
	u_int64 code = getCode(buffer,count);
	/* use code to validate any user-specified file */
	//getFname(...); 
	//testValidity(code, Fname);
	free(buffer);
}	