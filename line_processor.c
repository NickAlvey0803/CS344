// Nick Alvey - Assignment 4
// Framework of this Assignment completed with the Example Code provided in Assignment 4

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h> // must link with -lm
#include <string.h>
#include <fcntl.h>

/*
A program with a pipeline of 3 threads that interact with each other as producers and consumers.
- Input thread is the first thread in the pipeline. It gets input from the user and puts it in a buffer it shares with the next thread in the pipeline.
- Square root thread is the second thread in the pipeline. It consumes items from the buffer it shares with the input thread. It computes the square root of this item. It puts the computed value in a buffer it shares with the next thread in the pipeline. Thus this thread implements both consumer and producer functionalities.
- Output thread is the third thread in the pipeline. It consumes items from the buffer it shares with the square root thread and prints the items.

*/

// Size of the buffers
#define SIZE 1000

char charholder[1000];
int flag = 0;

// Number of items that will be produced. This number is less than the size of the buffer. Hence, we can model the buffer as being unbounded.
//#define NUM_ITEMS 0

// Buffer 1, shared resource between input thread and line separator thread
char buffer_1[SIZE];
// Number of items in the buffer
int count_1 = 0;
// Index where the input thread will put the next item
int prod_idx_1 = 0;
// Index where the line separator thread will pick up the next item
int con_idx_1 = 0;
// Initialize the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;


// Buffer 2, shared resource between line separator thread and the plus sign thread
char buffer_2[SIZE];
// Number of items in the buffer
int count_2 = 0;
// Index where the line separator thread will put the next item
int prod_idx_2 = 0;
// Index where the plus sign thread will pick up the next item
int con_idx_2 = 0;
// Initialize the mutex for buffer 2
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

// Buffer 3, shared resource between plus sign thread and the output thread
char buffer_3[SIZE];
// Number of items in the buffer
int count_3 = 0;
// Index where the plus sign thread will put the next item
int prod_idx_3 = 0;
// Index where the output thread will pick up the next item
int con_idx_3 = 0;
// Initialize the mutex for buffer 3
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 3
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;

// found on unix.com forumns
void redir(char* fl_nm)
{
  freopen(fl_nm,"w+",stdout);
}

// Search and replace function from here: https://www.geeksforgeeks.org/c-program-replace-word-text-another-given-word/ (as used and cited in HW3 as well)

char* replaceWord(const char* s, const char* oldW,
                  const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
  
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
  
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
  
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
  
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
  
    result[i] = '\0';

    //printf("%d", result);

    return result;
}

// end of Geeks for Geeks function

/*
Get input from the user.
This function doesn't perform any error checking.
*/
char *get_user_input(){
  
  char *value;
  printf("Enter a string to put in: ");

  return value;
}

/*
 Put an item in buff_1
*/
void put_buff_1(char *item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_1);
  // Put the item in the buffer
  strcpy(&buffer_1[prod_idx_1], item);
  // Increment the index where the next item will be put.
  prod_idx_1 = prod_idx_1 + strlen(item);
  count_1 = count_1 + strlen(item);
  
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_1);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
}

/*
 Function that the input thread will run.
 Get input from the user.
 Put the item in the buffer shared with the square_root thread.
*/
void *get_input(void *args)
{
    char *test;
    test = (char*)args;
    put_buff_1(test);

    return NULL;
}

/*
Get the next item from buffer 1
*/
char *get_buff_1(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0) {
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
  }
  char *item;
  item = &buffer_1[con_idx_1];
  // Increment the index from which the item will be picked up
  con_idx_1 = con_idx_1 + strlen(item);
  count_1 = count_1 - strlen(item);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
  // Return the item
  return item;
}

/*
 Put an item in buff_2
*/
void put_buff_2(char *item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_2);
  // Put the item in the buffer
  strcpy(&buffer_2[prod_idx_2], item);
  // Increment the index where the next item will be put.
  prod_idx_2 = prod_idx_2 + strlen(item);
  count_2 = count_2 + strlen(item);
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_2);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
}

/*
 Function that the square root thread will run. 
 Consume an item from the buffer shared with the input thread.
 Compute the square root of the item.
 Produce an item in the buffer shared with the output thread.

*/

void *line_separate(void *args)
{
    //char str[] = "xxforxx xx for xx";
    char c[] = "\n";
    char d[] = " ";
    char *item;

    item = get_buff_1();

    char* result = NULL;

    result = replaceWord(item, c, d);
    put_buff_2(result);

    // int item = 0;
    // double square_root;
    // for (int i = 0; i < NUM_ITEMS; i++)
    // {
    //   item = get_buff_1();
    //   square_root = sqrt(item);
    //   put_buff_2(square_root);
    // }
    return NULL;
}

/*
Get the next item from buffer 2
*/
char *get_buff_2(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_2);
  while (count_2 == 0) {
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_2, &mutex_2);
  }
  char *item;
  item = &buffer_2[con_idx_2];
  
  // Increment the index from which the item will be picked up
  con_idx_2 = con_idx_2 + strlen(item);
  count_2 = count_2 - strlen(item);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
  // Return the item
  return item;
}

void put_buff_3(char *item){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_3);
  // Put the item in the buffer
  strcpy(&buffer_3[prod_idx_3], item);
  // Increment the index where the next item will be put.
  prod_idx_3 = prod_idx_3 + strlen(item);
  count_3 = count_3 + strlen(item);
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_3);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
}

void *convert_plus(void *args)
{
    //char str[] = "xxforxx xx for xx";
    char c[] = "++";
    char d[] = "^";
    char *item;

    item = get_buff_2();

    char* result = NULL;

    result = replaceWord(item, c, d);
    put_buff_3(result);

    // int item = 0;
    // double square_root;
    // for (int i = 0; i < NUM_ITEMS; i++)
    // {
    //   item = get_buff_1();
    //   square_root = sqrt(item);
    //   put_buff_2(square_root);
    // }
    return NULL;
}

char *get_buff_3(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_3);
  while (count_3 == 0) {
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_3, &mutex_3);
  }
  char *item;
  item = &buffer_3[con_idx_3];
  
  // Increment the index from which the item will be picked up
  con_idx_3 = con_idx_3 + strlen(item);
  count_3 = count_3 - strlen(item);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
  // Return the item
  return item;
}


/*
 Function that the output thread will run. 
 Consume an item from the buffer shared with the square root thread.
 Print the item.
*/
void *write_output(void *args)
{   
    int NUM_ITEMS = *((int *)args);
    char *item;
    item = get_buff_3();    
    if (flag == 0) {
        strcpy(charholder,item);
        flag = 1;
        //printf("The whole thing is: %s\n\n\n", charholder);

        if (strlen(charholder) < 80) {
            //pass
        }
        else {
             printf("\nOutput: \n");
             while (strlen(charholder) > 80) {
                 for (int i = 0; i < 80; i++) {
                    printf("%c", charholder[0]);
                    memmove(&charholder[0], &charholder[0 + 1], strlen(charholder)-0);
                }
                printf("\n");
            }
        }
    } else {
        for (int i = 0; i < strlen(item); i++) {
            charholder[strlen(charholder) + i] = item[i];
        }
        int value = 0;
        if (strlen(charholder) < 80) {
            //pass
        }
        else {
             printf("\nOutput: \n");
             while (strlen(charholder) > 80) {
                 for (int i = 0; i < 80; i++) {
                    printf("%c", charholder[0]);
                    memmove(&charholder[0], &charholder[0 + 1], strlen(charholder)-0);
                }
                printf("\n");
            }
            
        }
    }
    //printf("\nOutput: %s, length of %lu\n", charholder, strlen(charholder));
    return NULL;
}

int main(int argc, char **argv)
{   
    if (argc < 2) {
        srand(time(0));
        pthread_t input_t, line_separator_t, square_root_t, output_t;

        char newval[1000];
        
        printf("Enter a line to process: ");
        scanf("%s", newval);
        int num = strlen(newval) / 80;
        int *NUMBER_ITEMS = &num;

        // Create the threads
        pthread_create(&input_t, NULL, get_input, &newval);
        pthread_create(&line_separator_t, NULL, line_separate, NULL);
        pthread_create(&square_root_t, NULL, convert_plus, NULL);
        pthread_create(&output_t, NULL, write_output, NUMBER_ITEMS);
        // Wait for the threads to terminate
        pthread_join(input_t, NULL);
        pthread_join(line_separator_t, NULL);
        pthread_join(square_root_t, NULL);
        pthread_join(output_t, NULL);

    } else if (argc < 3) {
        if (strstr(argv[1], "in") != NULL) {

            int NUM_ITEMS;
            char buff[1000];
            FILE *f = fopen(argv[1], "r");
            while (fgets(buff, 1000, f)) {
                NUM_ITEMS++;
            }
            int fclose( FILE *f);
            FILE *fp = fopen(argv[1], "r");

            int *NUMBER_ITEMS = &NUM_ITEMS;

            while (fgets(buff, 255, fp)) {
                if (strcmp(buff,"STOP\n") == 0) {
                    int fclose( FILE *fp);
                    return EXIT_SUCCESS;
                }
                else {
                    srand(time(0));
                    pthread_t input_t, line_separator_t, square_root_t, output_t;
                    // Create the threads
                    pthread_create(&input_t, NULL, get_input, &buff);
                    pthread_create(&line_separator_t, NULL, line_separate, NULL);
                    pthread_create(&square_root_t, NULL, convert_plus, NULL);
                    pthread_create(&output_t, NULL, write_output, NUMBER_ITEMS);
                    // Wait for the threads to terminate
                    pthread_join(input_t, NULL);
                    pthread_join(line_separator_t, NULL);
                    pthread_join(square_root_t, NULL);
                    pthread_join(output_t, NULL);
                }
                
            }
            int fclose( FILE *fp);

        } else if (strstr(argv[1], "out") != NULL) {

            srand(time(0));
            pthread_t input_t, line_separator_t, square_root_t, output_t;

            char newval[1000];
            
            printf("Enter a line to process: ");

            redir((char*)argv[1]);

            scanf("%s", newval);
            int num = strlen(newval) / 80;
            int *NUMBER_ITEMS = &num;

            // Create the threads
            pthread_create(&input_t, NULL, get_input, &newval);
            pthread_create(&line_separator_t, NULL, line_separate, NULL);
            pthread_create(&square_root_t, NULL, convert_plus, NULL);
            pthread_create(&output_t, NULL, write_output, NUMBER_ITEMS);
            // Wait for the threads to terminate
            pthread_join(input_t, NULL);
            pthread_join(line_separator_t, NULL);
            pthread_join(square_root_t, NULL);
            pthread_join(output_t, NULL);
        }
    } else {
        if (strstr(argv[1], "in") != NULL) {

            redir((char*)argv[2]);

            int NUM_ITEMS;
            char buff[1000];
            FILE *f = fopen(argv[1], "r");
            while (fgets(buff, 1000, f)) {
                NUM_ITEMS++;
            }
            int fclose( FILE *f);
            FILE *fp = fopen(argv[1], "r");

            int *NUMBER_ITEMS = &NUM_ITEMS;

            while (fgets(buff, 255, fp)) {
                if (strcmp(buff,"STOP\n") == 0) {
                    int fclose( FILE *fp);
                    return EXIT_SUCCESS;
                }
                else {
                    srand(time(0));
                    pthread_t input_t, line_separator_t, square_root_t, output_t;
                    // Create the threads
                    pthread_create(&input_t, NULL, get_input, &buff);
                    pthread_create(&line_separator_t, NULL, line_separate, NULL);
                    pthread_create(&square_root_t, NULL, convert_plus, NULL);
                    pthread_create(&output_t, NULL, write_output, NUMBER_ITEMS);
                    // Wait for the threads to terminate
                    pthread_join(input_t, NULL);
                    pthread_join(line_separator_t, NULL);
                    pthread_join(square_root_t, NULL);
                    pthread_join(output_t, NULL);
                }
                
            }
            int fclose( FILE *fp);

        } else if (strstr(argv[2], "in") != NULL) {

            redir((char*)argv[1]);

            int NUM_ITEMS;
            char buff[1000];
            FILE *f = fopen(argv[2], "r");
            while (fgets(buff, 1000, f)) {
                NUM_ITEMS++;
            }
            int fclose( FILE *f);
            FILE *fp = fopen(argv[2], "r");

            int *NUMBER_ITEMS = &NUM_ITEMS;

            while (fgets(buff, 255, fp)) {
                if (strcmp(buff,"STOP\n") == 0) {
                    int fclose( FILE *fp);
                    return EXIT_SUCCESS;
                }
                else {
                    srand(time(0));
                    pthread_t input_t, line_separator_t, square_root_t, output_t;
                    // Create the threads
                    pthread_create(&input_t, NULL, get_input, &buff);
                    pthread_create(&line_separator_t, NULL, line_separate, NULL);
                    pthread_create(&square_root_t, NULL, convert_plus, NULL);
                    pthread_create(&output_t, NULL, write_output, NUMBER_ITEMS);
                    // Wait for the threads to terminate
                    pthread_join(input_t, NULL);
                    pthread_join(line_separator_t, NULL);
                    pthread_join(square_root_t, NULL);
                    pthread_join(output_t, NULL);
                }
                
            }
            int fclose( FILE *fp);
        }
    }
    
    return EXIT_SUCCESS;
}