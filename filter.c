#include <unistd.h>
#include <stdio.h>

int filter(int m,int readfd,int writefd){
	int value;
	while(read(readfd,&value,sizeof(value)) > 0){
		if (m % value != 0){
			if(write(writefd,&value,sizeof(value)) == -1){
				return 1;
			}
		}
	}
	return 0;
}