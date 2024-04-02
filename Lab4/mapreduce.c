#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAX_WORD_LENGTH 1000
#define NUM_THREADS 4

// Structure for holding word count
typedef struct
{
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

// Function prototypes
void *map(void *arg);
void reduce();

// Shared data
WordCount intermediate_data[NUM_THREADS][MAX_WORD_LENGTH];
WordCount final_result[MAX_WORD_LENGTH];
char *file_name = "New Text Document.txt";

int main()
{
    pthread_t threads[NUM_THREADS];
    int i;

    // Start timing
    clock_t start = clock();

    // Create threads for map phase
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, map, (void *)(intptr_t)i);
    }

    // Wait for all threads to complete
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Reduce phase
    reduce();

    // End timing
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;


    // Print output
    printf("Word Count:\n");
    for (i = 0; i < MAX_WORD_LENGTH; i++)
    {
        if (final_result[i].count > 0)
        {
            printf("%s: %d\n", final_result[i].word, final_result[i].count);
        }
    }

    // Print time taken
    printf("Time taken: %f seconds\n", time_taken);

    return 0;
}

// Map function reads the file and counts words
void *map(void *arg)
{
    int thread_id = (int)(intptr_t)arg;
    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    long chunk_size = file_size / NUM_THREADS;
    long start_position = chunk_size * thread_id;
    long end_position = (thread_id == NUM_THREADS - 1) ? file_size : chunk_size * (thread_id + 1);

    fseek(file, start_position, SEEK_SET);

    char word[MAX_WORD_LENGTH];
    while (ftell(file) < end_position && fscanf(file, "%s", word) != EOF)
    {
        int word_length = strlen(word);
        if (word_length > 0)
        {
            intermediate_data[thread_id][word_length - 1].count++;
            strcpy(intermediate_data[thread_id][word_length - 1].word, word);
        }
    }

    fclose(file);
    pthread_exit(NULL);
}

// Reduce function aggregates word counts from all threads
void reduce()
{
    for (int i = 0; i < MAX_WORD_LENGTH; i++)
    {
        for (int j = 0; j < NUM_THREADS; j++)
        {
            final_result[i].count += intermediate_data[j][i].count;
        }
        strcpy(final_result[i].word, intermediate_data[0][i].word);
    }
}
