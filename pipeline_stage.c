#include "eratosthenes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

pid_t make_stage (int m,int readfd,int **fds){

	int ret_value;

	if (pipe(*fds) == -1){
		exit(-1);
	}

	if(((ret_value = fork()) < 0)) {
		exit(-1);
	}

	if (ret_value == 0){ //closes pipe ends and returns 0
		close((*fds)[1]);
		return 0;
	}
	else { // filters 
		close((*fds)[0]);
		if (filter(m,readfd,(*fds)[1]) == 1){
			exit(-1);
		}
		close ((*fds)[1]);
		return getpid();
	}


}