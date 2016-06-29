#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#include "mt19937ar.c"

#define NUMSMOKERS 6

sem_t agentLock;
sem_t pusherLock;
sem_t smokerSem[3];
sem_t pusherSem[3];
char* smokerType[3] = {"tobacco and matches",
"paper and matches", "tobacco and paper"};
int tableItems[3] = {0, 0, 0};


// The smokers wait until the items they need are available on the table.
// Once they are, the smoker will spend some time making a cigerette.
// When the smoker finishes making the cigerette, they will signal the
// agent to put new item's on the table and then smoke the cigerette.
void* smoker(void* arg)
{
	int id = *((int*) arg);
	int type = id % 3;
	int i;

	for(i = 0; i < 5; i++)
	{
		printf("Smoker %d waiting for %s\n",
			id, smokerType[type]);

		// wait for items on table
		sem_wait(&smokerSem[type]);

		// make cigarette
		printf("Smoker %d making a cigarette\n", id);
		usleep(genrand_int32() % 3000000);
		sem_post(&agentLock);

		// smoke
		printf("Smoker %d now smoking\n", id);
		usleep(genrand_int32() % 9000000);
	}

	return NULL;
}

// The pushers are signalled by the agent to put items on the table
// Each pusher pushes a specific item. Once the table is full
// (2 items on table), the pusher will signal the smoker to smoke.
void* pusher(void* arg)
{
	int id = *((int*) arg);
	int i;
	int j;

	for(i = 0; i < 20; i++)
	{
		// Wait for the pushers turn
		sem_wait(&pusherSem[id]);
		sem_wait(&pusherLock);

		// check if the right items are on the table
		if(tableItems[(id + 1) % 3])
		{
			tableItems[(id + 1) % 3] = 0;
			for(j = 0; j < NUMSMOKERS / 3; j++)
			{
				sem_post(&smokerSem[(id + 2) % 3]);
			}
		}
		else if(tableItems[(id + 2) % 3])
		{
			tableItems[(id + 2) % 3] = 0;
			for(j = 0; j < NUMSMOKERS / 3; j++)
			{
				sem_post(&smokerSem[(id + 1) % 3]);
			}
		}
		else
		{
			tableItems[id] = 1;
		}

		sem_post(&pusherLock);
	}

	return NULL;
}

// The agent occasionally wakes up and signals the pushers
// to push their items.
void* agent(void* arg)
{
	int id = *((int*) arg);
	int i;

	for(i = 0; i < 10; i++)
	{
		usleep(genrand_int32() % 12000000);

		// wait on agent lock
		sem_wait(&agentLock);

		// signal pushers to push items
		sem_post(&pusherSem[id]);
		sem_post(&pusherSem[(id + 1) % 3]);

		printf("Agent %d giving out %s\n",
			id, smokerType[(id + 2) % 3]);
	}

	return NULL;
}

int main()
{
	int i;
	init_genrand(time(NULL));

	sem_init(&agentLock, 0, 1);
	sem_init(&pusherLock, 0, 1);

	int smokers[NUMSMOKERS];
	int pushers[3];
	int agents[3];
	pthread_t smokerThreads[NUMSMOKERS];
	pthread_t pusherThreads[3];
	pthread_t agentThreads[3];

	for(i = 0; i < 3; i++)
	{
		sem_init(&smokerSem[i], 0, 0);
		sem_init(&pusherSem[i], 0, 0);
	}

	for(i = 0; i < NUMSMOKERS; i++)
	{
		smokers[i] = i;

		if(pthread_create(&smokerThreads[i], NULL, smoker, &smokers[i]) == EAGAIN)
		{
			perror("Not enough resources to create new thread");
			return 0;
		}
	}

	for(i = 0; i < 3; i++)
	{
		pushers[i] = i;

		if(pthread_create(&pusherThreads[i], NULL, pusher, &pushers[i]) == EAGAIN)
		{
			perror("Not enough resources to create new thread");
			return 0;
		}
	}

	for(i = 0; i < 3; i++)
	{
		agents[i] = i;

		if(pthread_create(&agentThreads[i], NULL, agent, &agents[i]) == EAGAIN)
		{
			perror("Not enough resources to create new thread");
			return 0;
		}
	}

	for(i = 0; i < 6; i++)
	{
		pthread_join(smokerThreads[i], NULL);
	}

	printf("All done!\n");

	return 0;
}
