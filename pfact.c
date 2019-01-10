#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

#include "eratosthenes.h"


int main(int argc, char *argv[]) {
    // Turning off sigpipe
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    if (argc != 2){
    	fprintf(stderr, "Usage:\n\tpfact n\n");
    	exit(1);
    }

    int factor1 = 0;
    int factor2 = 0;
    int n;
    int ret_value;
    int m;

    if ((n = strtol(argv[1],NULL,10)) == 0){
    	fprintf(stderr, "Usage:\n\tpfact n\n");
    	exit(1);
    }
    else if (n <= 0){
      	fprintf(stderr, "Usage:\n\tpfact n\n");
    	exit(1);  	
    }

    int fd[2];
    if (pipe(fd) == -1){
    	exit(2);
    }

    int fds[2];
    int * fds_pointer = fds;

    if (((ret_value = fork()) < 0)){
    	exit(2);
    }
    else if (ret_value == 0){
    	close(fd[1]);
    	if (read(fd[0],&m,sizeof(m)) < 0){
    		exit(2);
    	}
    	else {
    		while (m <= sqrt(n)) {
				if (n % m == 0){
					if (factor1 == 0){
						factor1 = m;
						if ((n / factor1) % factor1 == 0){
							if (factor1 * factor1 == n){
								printf("%d %d %d\n", n, factor1, factor1);
								close(fd[0]);
								exit(0);
							}
							else {
								printf("%d is not the product of two primes\n", n);
								close(fd[0]);
								exit(0);
							}
						}
					}
					else { //where factor 1 is already found
						printf("%d is not the product of two primes\n", n);
						close(fd[0]);
						exit(0);
					}
				}
				if (make_stage(m,fd[0],&fds_pointer) > 0){
					int status2;
					if (wait(&status2) != -1){
						if (WIFEXITED(status2)){
							if(WEXITSTATUS(status2) == -1){
								exit(-1);
							}
							exit(WEXITSTATUS(status2)+1);
						}
					}

				}
				else {
					if(read(fds[0],&m,sizeof(m)) < 0){
						exit(2);
					}
					else {
						dup2(fds[0],fd[0]);
					}
				}
			}

    	} // when m > sqrt(n)

    		if (n == 2 || n == 3){	// hardcording value values 2 and 3 because they are a special case
				printf("%d is prime\n", n);
				close(fd[0]);
				close(fd[1]);
				exit(0);
    		}
			while((read(fds[0],&m,sizeof(m))) > 0){
				if (factor1 * m == n){
					factor2 = m;
					printf("%d %d %d\n", n, factor1, factor2);
					close(fd[0]);
					exit(0);
				}

				if (factor1 == 0){
					if (factor2 == 0){
						printf("%d is prime\n", n);
						close(fd[0]);
						exit(0);
					}
				}
				printf("%d is not the product of two primes\n", n);
				exit(0);
			}

		}
    else {
    	close(fd[0]);
		for (int i = 2;i<=n;i++){
			if (write(fd[1],&i,sizeof(i)) == -1){
				exit(2);
			}
		}
		int status;
		close(fd[1]);
		if (wait(&status) != -1){
			if (WIFEXITED(status)){
				printf("Number of filters = %d\n", WEXITSTATUS(status));
			}
			exit(0);
		}
		else {
			exit(-1);
		}
    }
 	return 0;
}
