//Fabian Demiro U#02046975
//Username: fabiandemiro   pw: Fabi@12345678
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/msg.h>
#include <sys/sem.h>

//Semaphore key
#define SHMKEY ((key_t) 5698)
#define SEMKEY ((key_t) 4654)
#define NSEMS 1 //Number of semaphores created

int sem_id;
static struct sembuf OP = {0,-1,0};
static struct sembuf OV = {0,1,0};
struct sembuf *P =&OP;
struct sembuf *V = &OV;

typedef union {
	int val;
	struct semid_ds *buf;
	ushort *array;
} semunion;

int POP()
{
	int status;
	status = semop(sem_id, P, 1);
	return status;
}

int VOP()
{
	int status;
	status = semop(sem_id, V, 1);
	return status;
}
 
typedef struct
{
	int value;
} shared_mem;

void Process1(); //These are the functions that will increment the counters
void Process2();
void Process3();
void Process4();

shared_mem *total;

int main(void)
{

        int shmid, pid1,pid2, pid3,pid4, status, value, value1, semnum = 0;
	semunion semctl_arg;
	semctl_arg.val = 1;
        char *shmadd;
        shmadd = (char *) 0;

	//Create semaphores
	sem_id = semget(SEMKEY, NSEMS, IPC_CREAT | 0666);
	if(sem_id < 0) printf("Error in creating the semaphore.\n");

	//Initialize semaphore
	value1 = semctl(sem_id, semnum, SETVAL, semctl_arg);
	value = semctl(sem_id, semnum, GETVAL, semctl_arg);
	if(value < 1) printf("Error detected in SETVAL.\n");
	
        if ((shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0)
        {
                perror ("shmget");
                exit (1);
        }

        if ((total = (shared_mem *) shmat (shmid, shmadd, 0)) == (shared_mem *) -1)
        {
                perror ("shmat");
                exit (0);
        }

        total->value = 0; //Initialize the values to 0

        if((pid1 = fork()) == 0) //Fork the first time and make sure it's the child
        {
                Process1();
        }

        if(pid1 != 0 && ((pid2 = fork()) == 0)) //Fork the second time and make sure pid1 is not the child
        {
                Process2();
        }

        if(pid1 != 0 && pid2 != 0 && ((pid3 = fork()) == 0)) //Fork for the third process and make sure pid1 and pid2 are not $
        {
                Process3();
        }

        if(pid1 != 0 && pid2 != 0 && pid3 != 0 && ((pid4 = fork()) == 0)) //Perform the 4th fork and make sure pid1, pid2, and$
        {
                Process4();
        }

        if(pid1 != 0 && pid2 != 0 && pid3 != 0 && pid4 != 0) //Print out the 4 processes and their ID
        {
                printf("Child with ID: %d has just exited\n",waitpid(pid1, NULL, 0));
                printf("Child with ID: %d has just exited\n",waitpid(pid2, NULL, 0));
                printf("Child with ID: %d has just exited\n",waitpid(pid3, NULL, 0));
                printf("Child with ID: %d has just exited\n",waitpid(pid4, NULL, 0));
                printf("End of Program\n");
	}

        if(shmdt(total) == -1)
        {
                perror("shmdt");
                exit(-1);
        }

        shmctl(shmid, IPC_RMID, NULL); //Release shared memory

	//Deallocate semaphore
        semctl_arg.val = 0;
        status = semctl(sem_id, 0, IPC_RMID, semctl_arg);
        if(status < 0) printf("Error in removing the semaphore.\n");

        exit(0);
}

//Call POP() and VOP() in all functions to restrict access and allow processes to finish one at a time
void Process1()
{
        int x;
        for(x = 0;x < 100000; x++) //Increment the value 100000 as stated in the requirements for the first child
        {
		POP();
                total->value += 1;
		VOP();
        }

        printf("From Process 1: counter = %d\n", total->value);
        exit(0);
}

void Process2()
{
        int x;
        for(x = 0; x < 200000; x++) //Increment the value by 200000 as stated in the requirement for the second child
        {
		POP();
                total->value += 1;
		VOP();
        }

        printf("From Process 2: counter = %d\n", total->value);
        exit(0);
}

void Process3()
{
        int x;
        for(x = 0; x < 300000; x++) //Increment the value by 300000 as stated in the requirements for the third child
        {
		POP();
                total->value += 1;
		VOP();
        }
        printf("From Process 3: counter = %d\n", total->value);
        exit(0);
}

void Process4()
{
        int x;
        for(x = 0; x < 500000; x++)
        {
		POP();
                total->value += 1;
		VOP();
        }
        printf("From Process 4: counter = %d\n", total->value); //Increment the value by 500000 as estimated by the requiremen$
        exit(0);
}

