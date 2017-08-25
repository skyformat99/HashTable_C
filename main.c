#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    const char* fileName = "input2.txt";

	if (argc > 1)
    {
        fileName = argv[1];
    }

	printf("Opening file: %s\n", fileName);
    
    clock_t timer = clock();
    
    HashMap* map = hashMapNew(997);
    
	FILE *theFile = fopen(fileName, "r");
	if (!theFile) 
	{
		printf("Error opening file. Opening input1.txt\n");
		fileName = "input1.txt";
		fopen(fileName, "r");
	}

    // --- Concordance code begins here ---
	//1. Get the next word with getWord.
	char *nW = nextWord(theFile);

	while (nW != 0) {		
		//2. If the word is already in the hash map, then increment its number of occurrences.
		int contains = hashMapContainsKey(map, nW);

		if (contains)
		{
			//get the current value
			int *currVal = hashMapGet(map, nW);
			(*currVal)++;
			hashMapPut(map, nW, *currVal);
		}

		//3. Otherwise, put the word in the hash map with a count of 1.
		else hashMapPut(map, nW, 1);

		//4. Free the word.
		free(nW);

		//get next word
		nW = nextWord(theFile);
	}
	fclose(theFile);
    // --- Concordance code ends here ---
	
	//Print occurences in format from instructions; code adapted from hashMapPrint function
	for (int i = 0; i < map->capacity; i++)
	{
		HashLink* link = map->table[i];
		if (link != NULL)
		{
			while (link != NULL)
			{
				printf(" %s: %d \n", link->key, link->value);
				link = link->next;
			}
		}
	}
	printf("\n");

    hashMapPrint(map);
    
    timer = clock() - timer;
    printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
    printf("Number of links: %d\n", hashMapSize(map));
    printf("Number of buckets: %d\n", hashMapCapacity(map));
    printf("Table load: %f\n", hashMapTableLoad(map));
    
    hashMapDelete(map);
    return 0;
}