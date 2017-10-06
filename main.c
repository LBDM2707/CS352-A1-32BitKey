#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
typedef unsigned short u_short; 
typedef unsigned int u_int32; 
typedef unsigned long u_int64; 

void readFileToBuffer(char* Fname,u_int32** buffer, long* count){
	FILE *f = fopen(Fname, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 
	u_int32* temp = (u_int32*) malloc(size); // malloc an array of u_int32
	char bytes[4] = "";
	char byte;
	int i =0;
	int pointer = 0;
	while ((byte = fgetc(f)) != EOF){
	  bytes[3-i] = byte;
	  if (i==3){
	    temp[pointer] = *(u_int32 *) bytes;
	    pointer++;
	    memset(bytes,0,4);
	  }
	  i = (i+1)%4;
	}
	if (i!=0){
	  temp[pointer] = *(u_int32 *) bytes;
	}
        fclose(f);
	*count = size;
	*buffer = temp;
}

void getFname(char* Fname,u_int32** buffer, long* count){
	printf("Enter filename: ");
	scanf("%s",Fname);
	readFileToBuffer(Fname, buffer, count);	
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
u_int32 getCode(u_int32* buffer, int count) {
	u_int64 total=0;
	u_int64 rmmask = powl(2, 32)-1;
	u_int64 ofmask = powl(2, 32);
	u_int64 temp32 = 0;
	long pointer = 0;
	while (buffer[pointer] > 0) {
		//printf("========================\n");
		//printf("Total = %s\n",long_to_binary(total));
		//printf("Total = %x\n",total);
		total += (u_int64)(buffer[pointer]);
		//printf("Added = %s\n",long_to_binary((u_int64)buffer[pointer]));
		//printf("Added = %x\n",(u_int64)buffer[pointer]);
		//printf("Equal = %s\n",long_to_binary(total));
		//printf("Equal = %x\n",total);
		// handle overflow
		if ((total & ofmask) != 0){
			total = (total & rmmask) +1;
			//printf("Oflow = %s\n", long_to_binary(total));
			//printf("Oflow = %x\n", total);
		}
		//increase pointer 
		pointer++;
	}
	
/*	printf("flip  = %s\n", long_to_binary(~total));
*printf("========================\n");
	printf("Total hex = %x\n",total);
	printf("32bit hex = %x\n",~total);*/
	//convert total to code
	
	//printf("64 bit = %x\n", total);
	u_int32 sum = ~((u_int32)total);	
	return sum;
}  

void testValidity(char* Fname, u_int32 code) {
	long count;
	u_int32* buffer;	
	readFileToBuffer(Fname,&buffer,&count);
	u_int64 total=0;
	u_int64 rmmask = powl(2, 32)-1;
	u_int64 ofmask = powl(2, 32);
	long pointer = 0;
	while ((u_int64)buffer[pointer] > 0) {
		//printf("========================\n");
		//printf("Total = %x\n",total);
		total += (u_int64)(buffer[pointer]);
		//printf("Added = %s\n",long_to_binary((u_int64)buffer[pointer]));
		//printf("Added = %x\n",(u_int64)buffer[pointer]);
		//printf("Equal = %s\n",long_to_binary(total));
		//printf("Equal = %x\n",total);
		// handle overflow
		if ((total & ofmask) != 0){
			total = (total & rmmask) +1;
			//printf("Oflow = %s\n", long_to_binary(total));
			//printf("Oflow = %x\n", total);
		}
		//increase pointer 
		pointer++;
	}
	free(buffer);
	u_int32 sum = (int)total;
	//printf("32 bit = %x\n", sum);
	if (sum + code == rmmask) {
		printf("Validation: Success!\n");
	}
	else {
		printf("Validation: Fail\n");
	}
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
	u_int32 code = getCode(buffer,count);
	printf("Calculated code: %x (in hex)\n", code);
	free(buffer);
	int k=0;
	char tmp;
	while (k==0){
		printf("Enter filename for validation with %x: ",code);
		scanf("%s",Fname);
		testValidity(Fname,code);
		printf("Again? ");
		scanf("%c",&tmp);
		if (tmp == 'N') {
			k=1;
		}
	}
	/* use code to validate any user-specified file */
	//getFname(...); 
	//testValidity(code, Fname);
	
}	
