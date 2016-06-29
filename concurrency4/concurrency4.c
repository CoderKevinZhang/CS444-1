#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "mt19937ar.c"

#define NUMSEATS 20
#define NUMCUSTOMERS 30

sem_t barberSem;
sem_t customerSem;
pthread_mutex_t extraSeats;
int numExtraSeats = NUMSEATS;

void *customer(void* arg) { 
     int id = *((int *)arg);
     while(1)
     {
     	pthread_mutex_lock(&extraSeats);

     	if(numExtraSeats > 0)
     	{
     		usleep(genrand_int32() % 500000);
     		printf("customer %d entering barbershop\n", id);
     		numExtraSeats--;
     		sem_post(&customerSem);
     		pthread_mutex_unlock(&extraSeats);
     		sem_wait(&barberSem);
     		printf("cutting customer %d's hair\n", id);
     		usleep(1500000);
     		printf("customer %d haircut finished\n", id);
     		printf("customer %d leaving for now...\n", id);
     		usleep(9000000);
     	}
     	else
     	{
     		pthread_mutex_unlock(&extraSeats);
     		printf("customer %d leaving because waiting room is full\n", id);
     		usleep(9000000);
     	}
     }
} 

void *barber(void* arg) 
{
	while(1)
	{
		printf("barber waiting for customer...\n");
		sem_wait(&customerSem);
		pthread_mutex_lock(&extraSeats);
		numExtraSeats++;
		sem_post(&barberSem);
		pthread_mutex_unlock(&extraSeats);
		printf("barber cutting hair\n");
		usleep(1500000);
	}
}

int main(int argc, char *argv[]) 
{
	int i;
	init_genrand(time(NULL));
	int ids[NUMCUSTOMERS];
	pthread_t barberThread;
	pthread_t customerThreads[NUMCUSTOMERS];

	pthread_mutex_init(&extraSeats, NULL);
	sem_init(&barberSem, 0, 0);
	sem_init(&customerSem, 0, 0);

	pthread_create(&barberThread, NULL, barber, NULL);

	for(i = 0; i < NUMCUSTOMERS; i++)
	{
		ids[i] = i;
		pthread_create(&customerThreads[i], NULL, customer, &ids[i]);
	}

	for(i = 0; i < NUMCUSTOMERS; i++)
	{
		pthread_join(customerThreads[i], NULL);
	}

	return 0;
}
