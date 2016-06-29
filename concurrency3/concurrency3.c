#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include"mt19937ar.c"
#include<math.h>
#include<semaphore.h>

#define NUM_SEARCHERS 4
#define NUM_INSERTERS 3
#define NUM_DELETERS 2

typedef enum{SEARCH, INSERT, DELETE} op_t;
typedef enum{ACTIVE, DEACTIVE} state_t;

sem_t sem_searcher;
sem_t sem_inserter;
sem_t sem_deleter;

pthread_mutex_t searcher_mutex;
pthread_mutex_t inserter_mutex;
pthread_mutex_t deleter_mutex;

int active_searchers = 0;
state_t inserter_state = DEACTIVE;
state_t deleter_state = DEACTIVE;

int searcher_id[NUM_SEARCHERS];
int inserter_id[NUM_INSERTERS];
int deleter_id[NUM_DELETERS];

struct Node
{
   int data;
   struct Node *next;
};

struct Node* head;

struct Node* GetNewNode(int x) {
   struct Node* newNode
      = (struct Node*)malloc(sizeof(struct Node));
   newNode->data = x;
   newNode->next = NULL;
   return newNode;
}

void InsertAtTail(int x) {
   struct Node* cur = head;
   struct Node* newNode = GetNewNode(x);
   if(head == NULL) {
      head = newNode;
      return;
   }
   while(cur->next != NULL) cur = cur->next; // Go To last Node
   cur->next = newNode;
}

int DeleteVal(int x)
{
   struct Node* cur = head;
   struct Node* temp;
   if(head == NULL)
   {
      return -1;
   }
   if(head->data == x) {
      if(head->next != NULL)
      {
	 temp = head->next;
	 free(head);
	 head = temp;
      }
      else
      {
	 free(head);
	 head = NULL;
      }
      return 0;
   }
   while(cur->next != NULL)
   {
      temp = cur;
      cur = cur->next;
      if(cur->data == x)
      {
	 temp->next = cur->next;
	 free(cur);
	 return 0;
      }
   }
   return -1;
}

int SearchVal(int x)
{
   struct Node* cur = head;
   if(head == NULL)
   {
      return -1;
   }
   while(cur->next != NULL)
   {
      if(cur->data == x)
      {
	 return 0;
      }
      cur = cur->next;
   }
   return -1;
}

// This function is called after an operation is completed to check
// if another operation that is currently waiting is safe to run
int controller(op_t operation)
{
   int i;

   switch(operation)
   {
      case SEARCH:
	 if(deleter_state == DEACTIVE)
	 {
	    sem_post(&sem_searcher);
	    return 1;
	 }
	 break;
      case INSERT:
	 if(deleter_state == DEACTIVE && inserter_state == DEACTIVE)
	 {
	    sem_post(&sem_inserter);
	    return 1;
	 }
	 break;
      case DELETE:
	 if(inserter_state == DEACTIVE && active_searchers == 0)
	 {
	    sem_post(&sem_deleter);
	    return 1;
	 }
	 break;
   }
   return 0;
}

void* searcher(void* n)
{
   int *id = (int*)n;
   int found;
   int rnd;
   int check;

   while(1)
   {
      sleep(1);
      if(deleter_state == ACTIVE)
      {
	 printf("Searcher %d must wait for active deleters"
	       " to finish\n", *id);
      }
      // decrements the semaphore or waits if semaphore is 0 
      sem_wait(&sem_searcher);
      printf("Searcher %d ready to search\n", *id);
      active_searchers++;
      rnd = genrand_int32() % 20;
      printf("Searcher %d searching for %d in list\n", *id, rnd);
      found = SearchVal(rnd);
      if(found == 0)
      { 
	 printf("Searcher %d found %d in list\n", *id, rnd);
      }
      else
      {
	 printf("Searcher %d searched entire list"
	       " and did not find %d\n", *id, rnd);
      }
      active_searchers--;
      // Check if it is safe to delete
      check = controller(DELETE);
      // Otherwise continue searching
      if(check == 0)
      {
	 controller(SEARCH);
      }
   }
   return NULL;
}

void* inserter(void* n)
{
   int *id = (int*)n;
   int rnd;
   int check;

   while(1)
   {
      sleep(1);
      if(deleter_state == ACTIVE || inserter_state == ACTIVE)
      {
	 printf("Inserter %d must wait for active deleters"
	       " and inserters to finish\n", *id);
      }
      sem_wait(&sem_inserter);
      inserter_state = ACTIVE;
      printf("Inserter %d ready to insert\n", *id);	
      rnd = genrand_int32() % 20;
      printf("Inserter %d inserting %d to the end of the list\n",
	    *id, rnd);
      InsertAtTail(rnd);
      printf("Inserter %d inserted %d to the end of the list\n",
	    *id, rnd);
      inserter_state = DEACTIVE;
      // Check if it is safe to delete
      check = controller(DELETE);
      // Check if it is safe to insert
      if(check == 0)
      { 
	 check = controller(INSERT);
      }
   }
   return NULL;
}

void* deleter(void* n)
{
   int *id = (int*)n;
   int deleted;
   int rnd;
   
   while(1)
   {
      sleep(1);
      if(active_searchers > 0 || inserter_state == ACTIVE)
      {
	 printf("Deleter %d must wait for active inserters"
	       " and searchers to finish\n", *id);
      }
      sem_wait(&sem_deleter);
      deleter_state = ACTIVE;
      printf("Deleter %d ready to delete\n", *id);	
      rnd = genrand_int32() % 20;
      printf("Deleter %d deleting %d from the list\n",
	    *id, rnd);
      deleted = DeleteVal(rnd);
      if(deleted == 0)
      {
	 printf("Deleter %d deleted %d from the list\n",
	       *id, rnd);
      }
      else
      {
	 printf("Deleter %d did not find %d in the list\n",
	       *id, rnd);
      }
      deleter_state = DEACTIVE;
      // Check if it is safe to insert
      controller(INSERT);
      // Check if it is safe to search
      controller(SEARCH);
   }
   return NULL;
}

int main()
{
   int i;
   head = NULL;

   pthread_t searcher_p[NUM_SEARCHERS];
   pthread_t inserter_p[NUM_INSERTERS];
   pthread_t deleter_p[NUM_DELETERS];

   sem_init(&sem_searcher, 0, 4);
   sem_init(&sem_inserter, 0, 3);
   sem_init(&sem_deleter, 0, 2);

   for(i = 0; i < NUM_SEARCHERS; i++)
   {
      searcher_id[i] = i;
      pthread_create(&searcher_p[i], NULL, searcher,
	    (void *)&searcher_id[i]);
   }

   for(i = 0; i < NUM_INSERTERS; i++)
   {
      inserter_id[i] = i;
      pthread_create(&inserter_p[i], NULL, inserter,
	    (void*)&inserter_id[i]);
   }

   for(i = 0; i < NUM_DELETERS; i++)
   {
      deleter_id[i] = i;
      pthread_create(&deleter_p[i], NULL, deleter,
	    (void*)&deleter_id[i]);
   }

   for(i = 0; i < NUM_SEARCHERS; i++)
   {
      pthread_join(searcher_p[i], NULL);
   }

   for(i = 0; i < NUM_INSERTERS; i++)
   {
      pthread_join(inserter_p[i], NULL);
   }

   for(i = 0; i < NUM_DELETERS; i++)
   {
      pthread_join(deleter_p[i], NULL);
   }
}
