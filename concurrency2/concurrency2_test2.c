/********************************/
/*				*/
/*  Name: Dylan Camus 		*/
/*  Class: CS444		*/
/*  Assignment: Concurrency 2	*/
/*				*/
/********************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_t threads[5];
pthread_mutex_t m;
pthread_cond_t pcond[5];
pthread_mutex_t forks[5];
int state[5]; /*Thinking, hungry, or eating */
const char *philosophers[5]; /* the names of the philosophers */ 
int times_eating[5]; /* each philosophers eating tally */
static unsigned long mt[N]; /* the array for the state vector */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

struct Node {
   	int data;
	struct Node* next;
};

// Two glboal variables to store address of front and rear nodes. 
 struct Node* front = NULL;
 struct Node* rear = NULL;

// To Enqueue an integer
void Enqueue(int x) {
	struct Node* temp =
	   (struct Node*)malloc(sizeof(struct Node));
	temp->data =x; 
	temp->next = NULL;
	if(front == NULL && rear == NULL){
		front = rear = temp;
		return;
	}
	rear->next = temp;
	rear = temp;
}

// To Dequeue an integer.
void Dequeue() {
	struct Node* temp = front;
	if(front == NULL) {
		printf("Queue is Empty\n");
		return;
	}
	if(front == rear) {
		front = rear = NULL;
	}
	else {
		front = front->next;
	}
	free(temp);
}

int Front() {
	if(front == NULL) {
		return -1;
	}
	return front->data;
}

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
	mt[0]= s & 0xffffffffUL;
	for (mti=1; mti<N; mti++) {
		mt[mti] = 
		   (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. 	*/
		/* In the previous versions, MSBs of the seed affect   	*/
		/* only MSBs of the array mt[].                        	*/
		/* 2002/01/09 modified by Makoto Matsumoto             	*/
		mt[mti] &= 0xffffffffUL;
		/* for >32 bit machines	*/
	}
}

unsigned long genrand_int32(void)
{
	unsigned long y;
	static unsigned long mag01[2]={0x0UL, MATRIX_A};
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= N) { /* generate N words at one time */
		int kk;

		if (mti == N+1)   /* if init_genrand() has 	*/
				  /* not been called, 		*/
			init_genrand(5489UL); /* a default initial seed */
					      /*  is used 		*/

		for (kk=0;kk<N-M;kk++) {
   			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (;kk<N-1;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
	}
	
	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

/* Check if the head of the queue's neighbors are eating 		*/
/* If they are not, signal the head of the queue to start eating	*/ 
void check_queue()
{
   	int head;
	if(Front() != -1) {
		head = Front();
		if(state[(head + 1) % 5] != EATING &&
		      state[(head + 4) % 5] != EATING) {
			pthread_cond_signal(&pcond[head]);
		}
	}
}

void think(int x)
{
	int rnd = (genrand_int32() % 20) + 1;
	printf("%s is thinking...\n", philosophers[x]);
	/* sleep for rnd seconds but check queueperiodically */
	sleep(rnd);
	printf("%s thought for %d seconds\n",
	      philosophers[x], rnd);
}

/* Pick up forks only if both forks are available. If finished thinking */
/* but unable to eat due to the adjacent fork being in use, become	*/
/* hungry. When hungry, the philosopher is placed onto the queue.	*/
/* This queue prevents dreadlock and starvation, but results in reduced	*/
/* parallelism. For example, sometimes the situation occurs where	*/
/* only one philosopher is eating even though another could possibly	*/
/* start eating. This happens when the philosopher at the front of the	*/
/* queue is waiting for a fork.	However, it is still possible for more	*/
/* than one philosopher to eat at a time if the one on the front of the	*/
/* queue has available forks 						*/
void get_forks(int x)
{
	//pthread_mutex_lock(&m);
	//if(Front() == -1 && state[(x + 1) % 5] != EATING &&
	//      state[(x + 4) % 5] != EATING) {
	//	state[x] = EATING;
	//	printf("%s picked up forks %d and %d\n",
	//	      philosophers[x], x, (x + 1) % 5);
	//}
	//else {
	//   	state[x] = HUNGRY;
	//	Enqueue(x); /* put this philosopher on the queue */
	//	pthread_cond_wait(&pcond[x], &m);
	//	Dequeue(); /* if I get this far I must be at 	*/
	//		   /* the front of the queue		*/
	//	state[x] = EATING;
	//	printf("%s picked up forks %d and %d\n",
	//	      philosophers[x], x, (x + 1) % 5);
	//	check_queue();
      	//}
	//pthread_mutex_unlock(&m);
	
	pthread_mutex_lock(&forks[x]);
	printf("%s picked up fork %d\n",
	      philosophers[x], x);
	pthread_mutex_lock(&forks[(x + 1) % 5]);
	printf("%s picked up fork %d\n",
	      philosophers[x], (x + 1) % 5);
	state[x] = EATING;
}

void eat(int x)
{
	int rnd = (genrand_int32() % 8) + 2;
	printf("%s is eating...\n",
	      philosophers[x]);
	sleep(rnd);
	printf("%s ate for %d seconds\n",
	      philosophers[x], rnd);
	times_eating[x] = times_eating[x] + 1;
}

void put_forks(int x)
{
	//pthread_mutex_lock(&m);
	//state[x] = THINKING;
	//printf("%s set down forks %d and %d\n",
	//      philosophers[x], x, (x + 1) % 5);
	//check_queue();
	//pthread_mutex_unlock(&m);
	
	pthread_mutex_unlock(&forks[x]);
	printf("%s set down fork %d\n",
	      philosophers[x], x); 
	pthread_mutex_unlock(&forks[(x + 1) % 5]);
	printf("%s set down fork %d\n",
	      philosophers[(x + 1) % 5]);
	state[x] = THINKING;
}


/* The main philosopher loop */
void *table(int x)
{
	while(1) {
		think(x);
		get_forks(x);
		eat(x);
		put_forks(x);
	}
}

int main()
{
	int i;
	int j = 0;
	init_genrand(time(NULL));
	philosophers[0] = "Aristotle";
	philosophers[1] = "Plato";
	philosophers[2] = "Socrates";
	philosophers[3] = "Confucius";
	philosophers[4] = "Friedrich Nietzche";

	//pthread_mutex_init(&m, NULL);
	for(i = 0; i < 5; i++) {
		times_eating[i] = 0;
	}
	for(i = 0; i < 5; i++) {
		pthread_mutex_init(&forks[i], NULL);
	}
	for(i = 0; i < 5; i++) {
		pthread_create(&threads[i], NULL,
		      (void *)table, (void *)i);
	}

	/* Periodically print the number of times each philosopher 	*/
	/* has eaten to make sure that it is fair 			*/
	while(1) {
	   	j++;
		sleep(1);
		if(j % 5 == 0) {
		   	printf("\n====================="
			      "=====================\n");
			for(i = 0; i < 5; i++) {	
				printf("%s has ate %d times\n",
				      philosophers[i], times_eating[i]);
			}
			printf("====================="
			      "=====================\n");
		}
		printf("\n---------------------"
		      "---------------------\n");
		for(i = 0; i < 5; i++) {
			if(state[i] == EATING) {
				printf("%s is currently EATING\n",
				      philosophers[i]);
		   	}
			else if(state[i] == THINKING) {
				printf("%s is currently THINKING\n",
				      philosophers[i]);
			}
			else {
				printf("%s is currently HUNGRY\n",
				      philosophers[i]);
		   	}
		}
		printf("---------------------"
		      "---------------------\n\n");
		sleep(2);
	}
	for(i = 0; i < 5; i++) {
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&m);
	return 0;
}
