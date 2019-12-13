#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    // FIXME: implement
    int val = 0;
    char* word = nextWord(file);
    
    while (word != 0) {
        hashMapPut(map, word, val);
        free(word);
        word = nextWord(file);
        val++;
    }
}

/**
 Levenshtein Distance Implementaion
 */
int minimum(int a, int b) {
    if (a > b) {
        return b;
    }
    return a;
}

int minimumThree(int a, int b, int c) {
    return minimum(a, minimum(b, c));
}

int levenshtein(const char *s, const char *t) {
    int len_s = strlen(s), len_t = strlen(t);
    int matrix[len_s + 1][len_t + 1];
    
    matrix[0][0] = 0;
    
    for (int i = 1; i <= len_s; i++) {
        matrix[i][0] = i;
    }
    
    for (int j = 1; j <= len_t; j++) {
        matrix[0][j] = j;
    }
    
    for (int j = 1; j <= len_t; j++) {
        for (int i = 1; i <= len_s; i++) {
            int cost = 0;
            if (s[i - 1] != t[j - 1]) {
                cost = 1;
            }
            
            matrix[i][j] = minimumThree(matrix[i - 1][j] + 1,
                                        matrix[i][j - 1] + 1,
                                        matrix[i - 1][j - 1] + cost);
        }
    }
    
    return matrix[len_s][len_t];
}

/**
 * Checks the spelling of the word provded by the user. If the word is spelled incorrectly,
 * print the 5 closest words as determined by a metric like the Levenshtein distance.
 * Otherwise, indicate that the provded word is spelled correctly. Use dictionary.txt to
 * create the dictionary.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    HashMap* map = hashMapNew(1000);

    FILE* file = fopen("/Users/kyeongnam/Google Drive/OSU/Fall_2019/CS261_DS/Assignments/assign_5/assign5_part2_spellChecker/assign5_part2_spellChecker/dictionary.txt", "r");
//    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputBuffer[256];
    while (1)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);

        // Implement the spell checker code here..
        if (strcmp(inputBuffer, "quit") == 0)
        {
            break;
        }
        
        //To lower letters
        for (int i = 0; i < strlen(inputBuffer); i++) {
            inputBuffer[i] = tolower(inputBuffer[i]);
        }
        
        int *isInMap = hashMapGet(map, inputBuffer);
        
        //If input is in map
        if (isInMap) {
            printf("The inputted word \"%s\" is spelled correctly\n", inputBuffer);
        } else {
            printf("The inputted word \"%s\" is spelled incorrectly\n", inputBuffer);
            printf("Did you mean ");

            int difference = 1;
            int cnt = 5;
            while (cnt != 0) {
                for (int i = 0; i < map->capacity; i++) {
                    HashLink *cur = map->table[i];
                    while (cur != 0) {
                        if (levenshtein(cur->key, inputBuffer) == difference) {
                            printf("\"%s\" ", cur->key);
                            cnt--;
                        }
                        cur = cur->next;
                    }
                    if (cnt == 0) {
                        break;
                    }
                }
                difference++;
            }
            printf("?\n");
        }
    }
    
    hashMapDelete(map);
    return 0;
}
