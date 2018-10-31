//Fabian Demiro 02046975
#define _REENTRANT
#define BUFFER_SIZE 15
#define SHMKEY ((key_t) 2741)
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
    char character[BUFFER_SIZE];
} shared_mem;

shared_mem *buffer;
sem_t full, empty, mutex;
char newChar;
FILE *fp;
int counter = 0;

void *thread1();
void *thread2();


int main(void)
{
	int shmid;
  pthread_t tid1[1];     /* process id for thread 1 */
  pthread_t tid2[1];     /* process id for thread 2 */
  pthread_attr_t attr[1];     /* attribute pointer array */

 buffer = (shared_mem*) malloc(sizeof(shared_mem));

  fp = fopen("mytest.dat", "r");

  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);

  char *shmadd;
  shmadd = (char *) 0;


  if((shmid = shmget(SHMKEY, sizeof(char), IPC_CREAT | 0666)) < 0)
  {
        perror("shmget");
        return(1);
  }

  if((buffer = (shared_mem *) shmat(shmid, shmadd, 0)) == (shared_mem *) -1)
  {
        perror ("shmat");
        return (0);
  }

  fflush(stdout);
  pthread_attr_init(&attr[0]);
  pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);

  pthread_create(&tid1[0], &attr[0], thread1, NULL);
  pthread_create(&tid2[0], &attr[0], thread2, NULL);

  pthread_join(tid1[0], NULL);
  pthread_join(tid2[0], NULL);

  if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
   {
        perror ("shmctl");
        exit (-1);
   }

  pthread_exit(NULL);
  sem_destroy(&empty);
  sem_destroy(&full);
  sem_destroy(&mutex);
  fclose(fp);
  printf("End of program");
  exit(0);
}

void *thread1()
{
	while(1)
	{
 		sem_wait(&empty);
    sem_wait(&mutex);


		if(fscanf(fp,"%c", &newChar) != '*')
    {
      buffer->character[counter] = newChar;
      counter++;
      counter = counter % BUFFER_SIZE;
    }
    else
    {
      buffer->character[counter] = '*';
      exit(0);
    }
    
		sem_post(&mutex);
		sem_post(&full);
	}
}

void *thread2()
{
	while(1)
	{
		sem_wait(&full);
		sem_wait(&mutex);

    sleep(1);

    if(buffer->character[counter] != '*')
    {
        printf("Consumer: %c\n", buffer->character[counter]);
	      counter = counter % BUFFER_SIZE;
    }
     else
      exit(0);

		sem_post(&mutex);
		sem_post(&empty);
	}
}
