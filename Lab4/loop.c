#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 1000

// Structure for holding word count
typedef struct
{
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

char *file_name = "New Text Document.txt";

int main()
{
    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Start timing
    clock_t start = clock();

    WordCount word_counts[MAX_WORD_LENGTH];
    memset(word_counts, 0, MAX_WORD_LENGTH * sizeof(WordCount));

    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", word) != EOF)
    {
        int found = 0;
        for (int i = 0; i < MAX_WORD_LENGTH; i++)
        {
            if (strcmp(word_counts[i].word, word) == 0)
            {
                word_counts[i].count++;
                found = 1;
                break;
            }
        }
        if (!found)
        {
            for (int i = 0; i < MAX_WORD_LENGTH; i++)
            {
                if (word_counts[i].count == 0)
                {
                    strcpy(word_counts[i].word, word);
                    word_counts[i].count = 1;
                    break;
                }
            }
        }
    }

    // End timing
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;



    // Print word count
    printf("Word Count:\n");
    for (int i = 0; i < MAX_WORD_LENGTH; i++)
    {
        if (word_counts[i].count > 0)
        {
            printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
        }
    }
    // Print time taken
    printf("Time taken: %f seconds\n", time_taken);
    fclose(file);
    return 0;
}
