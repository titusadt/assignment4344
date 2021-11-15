#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include<assert.h>
#include<stdbool.h>
#include<string.h>


// Size of the buffers
#define SIZE 1000
//#define
#define NUM_ITEMS 50
#define STOP_SYMBOL "STOP\n" 

// Buffer 1, shared resource between input thread and square-root thread
char buffer_1[SIZE];// Number of items in the buffer
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;// Initialize the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;
int count_1 = 0;// Index where the input thread will put the next item


//Buffer 2, shared resource between square root thread and output thread
char buffer_2[SIZE];// Number of items in the buffer
int count_2 = 0;// Index where the input thread will put the next item
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;// Initialize the condition variable for buffer 1
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

//bu
/*Buffer3 */
char buffer_3[SIZE];// Number of items in the buffer
int count_3 = 0;// Index where the input thread will put the next item
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;// Initialize the condition variable for buffer 1
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;


/*
Get input from the user.
This function doesn't perform any error checking.
*/
char* get_user_input(){
  char* command = NULL;
  size_t len = 0;
  getline(&command, &len, stdin);
  return command;
}


/*
Get the next item from buffer 1
*/
char* get_buff_1(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
  char item[SIZE];
  strcpy(item, buffer_1);
  // Increment the index from which the item will be picked up
  
  count_1--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
  // Return the item
  return item;
}

/*
 Put an item in buff_1
*/
void put_buff_1(char* item){
  
  pthread_mutex_lock(&mutex_1); // Lock the mutex before putting the item in the buffer
  
  for(int i =0; i <=strlen(item); i++){
    buffer_1[i] = item[i];// Put the item in the buffer
  }
  count_1++;
  pthread_cond_signal(&full_1);  // Signal to the consumer that the buffer is no longer empty
  pthread_mutex_unlock(&mutex_1);  // Unlock the mutex
}


/*
 Function that the input thread will run.
 Get input from the user.
 Put the item in the buffer shared with the square_root thread.
*/
void *get_input()
{
  char* item;
  //item = get_user_input();
  for (int i = 0; i < NUM_ITEMS; i++)
  {
    // Get the user input
    item = get_user_input();
    if(strcmp(item, STOP_SYMBOL)==0){
      put_buff_1(item);
      return NULL;
    }
    printf("THIS IS THE INPUT FILE \n")
    //printf(" %s", item);
    put_buff_1(item);
    free(item);
  }
  //printf(" %s\n", item);
  return NULL;
}




/*
Get the next item from buffer 2
*/
char* get_buff_2(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_2);
  while (count_2 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_2, &mutex_2);
  char item[SIZE];
  strcpy(item, buffer_2);
  
  count_2--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
  // Return the item
  return item;
}

/*
 Put an item in buff_2
*/
void put_buff_2(char* item){
  
  pthread_mutex_lock(&mutex_2); // Lock the mutex before putting the item in the buffer
  
  strcpy(buffer_2, item);
 
  count_2++;
  pthread_cond_signal(&full_2);  // Signal to the consumer that the buffer is no longer empty
  pthread_mutex_unlock(&mutex_2);  // Unlock the mutex
}

/*****
replacig lie separator with space
******/
char* LinetoSpace()
{
  char* item;
  for(int i = 0; i < NUM_ITEMS; i++){
    item = get_buff_1();
    if(strcmp(item, STOP_SYMBOL)){
      put_buff_2(item);
      break;
    }
  
    for(int j = 0; j<= strlen(item); j++){
      if(item[i] == '\n'){
        item[i] = ' ';
      }
        
    }
  printf(" %s", item);
  put_buff_2(item);
  free(item);
  }
  return NULL; 
}

//create void* and that is what I will put in the thread




/*
PUT AN ITEM IN BUFFER 3
*/
void put_buff_3(char* item){
  
  pthread_mutex_lock(&mutex_3); // Lock the mutex before putting the item in*the buffer
  
  strcpy(buffer_3, item);
  // Increment the index where the next item will be put.
  //prod_idx_1 = prod_idx_1 + 1; 
  count_3++;
  pthread_cond_signal(&full_3);  // Signal to the consumer that the buffer is no longer empty
  pthread_mutex_unlock(&mutex_3);  // Unlock the mutex
}

/**
REPLACING DOUBLE + SIGN (++)
*/
char* replacePlus(){
  char* item;
  for(int b = 0; b < NUM_ITEMS; b++){
    item = get_buff_2();
    if(strcmp(item, STOP_SYMBOL)){
      put_buff_3(item);
      break;
    }
  
    for(int i = 0; i < (strlen(item)-1); i++){
      if(item[i] == '+' && item[i+1] == '+'){
        item[i] = '^';
        //make a new buffer so when I change it the buffer im reading from doesnt get impacted
        //update the changed buffer
        for(int j =0; j<strlen(item); j++){
          item[j] = item[j+1];
        }
      }
    }
    printf("this is the item %s \n", item);
    put_buff_3(item);
  }
  return NULL;
}

/*
get_buffer 3
*/
char* get_buff_3(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_3);
  while (count_3 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_3, &mutex_3);
  char item[SIZE];
  strcpy(item, buffer_3);
  // Increment the index from which the item will be picked up
  //si
  count_3--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
  // Return the item
  return item;
}

/*
eighty characters thing

*/
char* writeOutput(){
  char* item;
  for(int i =0; i >= 80; i++){
    printf("\n");
    fflush(stdout);
    write(1, item, 80);
    write(1, "\n", 2);
    int s = 79;
    for(int j =0; j < strlen(item); j++){
      if(item[s++] != '\0'){
        item[i] = item[s];
      }
      else
        break;
    }
  }
}

/*
output buffer thing, could also probably be one function
*/




int main()
{
  memset(buffer_1, '\0', sizeof(buffer_1));
  memset(buffer_2, '\0', sizeof(buffer_2));
  memset(buffer_3, '\0', sizeof(buffer_3));
  pthread_t input_t, linePros_t, plus_t, output_t;
  // Create the threads
  pthread_create(&input_t, NULL, get_input, NULL);
  pthread_create(&linePros_t, NULL, LinetoSpace, NULL);
  pthread_create(&output_t, NULL, writeOutput, NULL);
  pthread_create(&plus_t, NULL, replacePlus, NULL);
  
  // Wait for the threads to terminate
  pthread_join(input_t, NULL);
  pthread_join(linePros_t, NULL);
  pthread_join(output_t, NULL);
  pthread_join(plus_t, NULL);
  
  return 0;
}