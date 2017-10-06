#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <tgmath.h>
typedef unsigned short u_short; 
typedef unsigned int u_int32; 
typedef unsigned long int u_int64; 
void getFname(char* Fname,u_int32** buffer, long* count){
	scanf("%s",Fname);

	FILE *f = fopen(Fname, "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET); 

	u_int32* temp = (u_int32*) malloc(size);
	fread(temp, sizeof(u_int32), size/sizeof(u_int32), f);
	fclose(f);
	*count = size;
	*buffer = temp;
}
/*char* long_to_binary(u_int64 k){//for testing
    char* c = (char*) malloc(65 * sizeof(char));
    c[64] = '\0';
    u_int64 val;
	int i;
	for (i = 0; i < 64; i++) {
		val = powl(2, (64 - i) - 1);
		if (k & val) {
			c[i] = '1';
		}
		else {
			c[i] = '0';
		}
	}
    return c;
}*/
u_int32 getCode(u_int32* buffer, int count) {
	u_int64 total=0;
	u_int64 rmmask = powl(2, 32)-1;
	u_int64 ofmask = powl(2, 32);
	int bytes = 4;
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
	u_int32 sum = total;
	//printf("32 bit = %x\n", sum);
	return ~sum;
} 


void testValidity(FILE *f, u_int32 code) {
	fseek(f, 0, SEEK_END);
	long count = ftell(f);
	fseek(f, 0, SEEK_SET); 
	
	u_int32* buffer = (u_int32*) malloc(count);
	fread(buffer, sizeof(u_int32), count/sizeof(u_int32), f);
	fclose(f);
	u_int64 total=0;
	u_int64 rmmask = powl(2, 32)-1;
	u_int64 ofmask = powl(2, 32);
	int bytes = 4;
	u_int64 temp32 = 0;
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
	
	u_int32 sum = total;
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
	printf("Enter filename: ");
	getFname(Fname,&buffer,&count);
	//printf("%s\n",Fname);
	//printf("%s\n",buffer);
	//printf("%ld\n",count);
	u_int32 code = getCode(buffer,count);
	printf("Calculated code: %x\n", code);
	/* use code to validate any user-specified file */
	printf("Enter filename for validation with %x:", code);
	scanf("%s",Fname);
	FILE *f = fopen(Fname, "rb");
	testValidity(f, code);
	
	
	free(buffer);
}

//9f243670
