//Fabian Demiro 02046975
#define _REENTRANT
#define BUFFER_SIZE 15
#define SHMKEY ((key_t) 6358)
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

typedef struct { //Shared memory struct as defined by the project requirements with an array of buffer size
    char character[BUFFER_SIZE];
    int value;
} shared_mem;

shared_mem *buffer;
sem_t full, empty, mutex;
char newChar;
FILE *fp;
int counter1 = 0;
int counter2 = 0;
void *thread1();
void *thread2();


int main(void)
{
	int shmid;
  pthread_t tid1[1];     /* process id for thread 1 */
  pthread_t tid2[1];     /* process id for thread 2 */
  pthread_attr_t attr[1];     /* attribute pointer array */

  buffer = (shared_mem*) malloc(sizeof(shared_mem));
  buffer->value = 0;
  fp = fopen("mytest.dat", "r"); //Open mytest.dat and read it
  
  sem_init(&empty, 0, BUFFER_SIZE); //Initialize the 3 semaphores
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
  
  pthread_create(&tid1[0], &attr[0], thread1, NULL); //Create pthreads
  pthread_create(&tid2[0], &attr[0], thread2, NULL);

  pthread_join(tid1[0], NULL); //Wait for threads to finish
  pthread_join(tid2[0], NULL);

  pthread_exit(NULL);
  sem_destroy(&empty);
  sem_destroy(&full);
  sem_destroy(&mutex);
  
  if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
  {
    perror ("shmctl");
    exit (-1);
  }
  
  fclose(fp);
  
  exit(0);
}

void *thread1() //Producer thread 
{
  int t = 0;
  
	do
	{
 		sem_wait(&empty); //Wait
    sem_wait(&mutex);
    
		
    if(fscanf(fp,"%c", &newChar) != EOF && !t) //Read from file and save to newChar
    {
      buffer->character[counter1++ % BUFFER_SIZE] = newChar; //Save the character to the shared buffer so the consumer can access it too
      printf("Producer: %c\n", newChar);
    }     
    else
    {
        buffer->character[counter1++ % BUFFER_SIZE] = '*'; //If EOF is reached, declare an '*' so the consumer can read it and decide it needs to stop running
        t = 1;
    }
    
		sem_post(&mutex); //Signal
		sem_post(&full);
	} while(!t);
}

void *thread2()
{
  int t = 0;
  
	do
	{
		sem_wait(&full); //Wait
		sem_wait(&mutex);
	
    sleep(1); //Sleep for 1 second as request by instructions
    if(buffer->character[counter2 % BUFFER_SIZE] != '*' && (!t)) 
    {
      printf("Consumer: %c\n", buffer->character[counter2 % BUFFER_SIZE]); //Print the character to console
      buffer->value++;
      counter2++;
    } 
    else 
    {
      t = 1;
    } 
     
		sem_post(&mutex); //Signal
		sem_post(&empty);
	} while(!t);
}
