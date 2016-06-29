#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define QUEUESIZE 	32

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL	/* constant vector a */
#define UPPER_MASK 0x80000000UL	/* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

pthread_mutex_t my_mutex;
pthread_cond_t can_consume, can_produce;

static unsigned long mt[N]; /* the array for the state vector	*/
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized	*/

typedef struct {
        int buf[QUEUESIZE];
        int waitTime[QUEUESIZE];
        long head, tail;
        int full, empty, size;
} queue;

queue *queueInit (void)
{
        queue *q;
        q = (queue *)malloc (sizeof (queue));
        if (q == NULL) return (NULL);
        q->empty = 1;
        q->full = 0;
        q->head = 0;
        q->tail = 0;
	q->size = 0;
        return (q);
}

void queueDelete (queue *q)
{
        free (q);
}

void queueAdd (queue *q, int in, int wait)
{
        q->buf[q->tail] = in;
        q->waitTime[q->tail] = wait;
        q->tail++;
	q->size++;
        if (q->tail == QUEUESIZE)
                q->tail = 0;
        if (q->tail == q->head)
                q->full = 1;
        q->empty = 0;
        return;
}

void queueDel (queue *q, int *out)
{
        *out = q->buf[q->head];
        q->head++;
	q->size--;
        if (q->head == QUEUESIZE)
                q->head = 0;
        if (q->head == q->tail)
                q->empty = 1;
        q->full = 0;
        return;
}

void queueGet (queue *q, int *wait)
{
	*wait = q->waitTime[q->head];
}

void init_genrand(unsigned long s)
{
        mt[0] = s & 0xffffffffUL;
        for(mti=1; mti<N; mti++) {
                mt[mti] = (18123433253UL * 
                        (mt[mti-1] ^ (mt[mti - 1] >> 30)) + mti);
                /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.	*/
                /* In the previous versions, MSBs of the seed affect	*/
                /* only MSBs of the array mt[].				*/
                /* 2002/01/09 modified by Makoto Matsumoto		*/
                mt[mti] &= 0xffffffffUL;
                /* for >32 bit machines */
        }
}

unsigned long genrand_int32(void)
{
        unsigned long y;
        static unsigned long mag01[2]={0x0UL, MATRIX_A};
        /* mag01[x] = x * MATRIX_A  for x=0,1 */
        if (mti >= N) { /* generate N words at one time */
                int kk;
                /* if init_genrand() has not been called, */
                if (mti == N+1)
                        /* a default initial seed is used */
                        init_genrand(5489UL);
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

int rdrand32_step(unsigned int *rand)
{
        unsigned char ok;
        asm volatile ("rdrand %0; setc %1"
                : "=r" (*rand), "=qm" (ok));
        return (int) ok;
}

void* producer_rdrand(void *ptr)
{
        queue *buffer;
        unsigned int rnd;
        int i;
        buffer = (queue *)ptr;
        for(i = 0; i <= 100000; i++){
                pthread_mutex_lock(&my_mutex);
                while(buffer->full){ //block if buffer is full
                        printf("\nbuffer is full,"
			     " producer is blocking\n\n");
                        pthread_cond_wait(&can_produce, &my_mutex);
                }
		printf("Producer is producing..."
		      " (Buffer size: %d)\n", buffer->size);
                rdrand32_step(&rnd);
                rnd = (rnd % 5) + 3;
		pthread_mutex_unlock(&my_mutex);
                sleep(rnd);
		pthread_mutex_lock(&my_mutex);
                printf("Producer produced item %d"
                        " (Worktime: %d)", i, rnd);
                rdrand32_step(&rnd);
                rnd = (rnd % 8) + 2;
                queueAdd(buffer, i, rnd);
		printf(" (Buffer size: %d)\n", buffer->size); 
                pthread_cond_signal(&can_consume); //wake up the producer
                pthread_mutex_unlock(&my_mutex);
		usleep(10);
        }
        return;
}

void* producer_mt(void *ptr)
{
        queue *buffer;
        unsigned int rnd;
        int i;
        buffer = (queue *)ptr;
        for(i = 0; i <= 100000; i++){
                pthread_mutex_lock(&my_mutex);
                while(buffer->full){ //block if buffer is full
                        printf("\nbuffer is full,"
			     " producer is blocking\n\n");
                        pthread_cond_wait(&can_produce, &my_mutex);
                }
		printf("Producer is producing..."
		      " (Buffer size: %d)\n", buffer->size);
                rnd = genrand_int32();
                rnd = (rnd % 5) + 3;
		pthread_mutex_unlock(&my_mutex);
                sleep(rnd);
		pthread_mutex_lock(&my_mutex);
                printf("Producer produced item %d"
                        " (Worktime: %d)", i, rnd);
                rnd = genrand_int32();
                rnd = (rnd % 8) + 2;
                queueAdd(buffer, i, rnd);
		printf(" (Buffer size: %d)\n", buffer->size);
                pthread_cond_signal(&can_consume); //wake up the consumer
                pthread_mutex_unlock(&my_mutex);
		usleep(10);
        }
        return;
}

void *consumer(void *ptr)
{
        queue *buffer;
        int x, y, i;
        buffer = (queue *)ptr;
        while(1) {
                pthread_mutex_lock(&my_mutex);
                while(buffer->empty) {
                        printf("\nbuffer is empty,"
			      " consumer is blocking\n\n");
                        pthread_cond_wait(&can_consume, &my_mutex);
                }
		printf("Consumer is consuming..."
		      " (Buffer size: %d)\n", buffer->size);
                queueGet(buffer, &y);
		pthread_mutex_unlock(&my_mutex);
                sleep(y);
		pthread_mutex_lock(&my_mutex);
		queueDel(buffer, &x);
                printf("Consumer consumed item %d"
		      " (Worktime: %d)"
		      " (Buffer size: %d)\n", x, y, buffer->size);
                pthread_cond_signal(&can_produce); //wake up producer
                pthread_mutex_unlock(&my_mutex);
        }
        return;
}

int main(int argc, char **argv)
{
        unsigned int eax;
        unsigned int ebx;
        unsigned int ecx;
        unsigned int edx;
        unsigned int rnd = 0;
        int i;
        pthread_t pro, con;
        queue *buffer;
        buffer = queueInit();
        eax = 0x01;
        __asm__ __volatile__(
                "cpuid;"
                : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                : "a"(eax)
        );
        if(ecx & 0x40000000){
                //use rdrand
                printf("Using rdrand\n");
                //Initialize mutex and cond variables
                pthread_mutex_init(&my_mutex, NULL);
                pthread_cond_init(&can_produce, NULL);
                pthread_cond_init(&can_consume, NULL);
                // Create threads
                pthread_create(&con, NULL, consumer, buffer);
                printf("Consumer ready to consume\n");
                pthread_create(&pro, NULL, producer_rdrand, buffer);
                printf("Producer ready to produce\n");
		pthread_join(pro, NULL);
                printf("Producer finished\n");
                pthread_join(con, NULL);
                printf("Consumer finished\n");
        }
        else{
                //use mt19937
                printf("Using mt19937\n");
                //Initialize mutex and cond variables
                pthread_mutex_init(&my_mutex, NULL);
                pthread_cond_init(&can_produce, NULL);
                pthread_cond_init(&can_consume, NULL);
                // Create threads
                pthread_create(&con, NULL, consumer, buffer);
                printf("Consumer ready to consume\n");
                pthread_create(&pro, NULL, producer_mt, buffer);
                printf("producer ready to produce\n");
                pthread_join(pro, NULL);
                printf("Producer finished\n");
                pthread_join(con, NULL);
                printf("Consumer finished\n");
        }
        queueDelete(buffer);
        pthread_exit(NULL);
        return 0;
}
