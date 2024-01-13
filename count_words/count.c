#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_UNIQUES 6000
#define MAX_LINES 200


typedef struct {
    char* word;
    int count;
} count;

char line[MAX_UNIQUES];
struct count* words;
int num = 0;


static int cmp_count(const void* p1, const void* p2) {
    int c1 = ((count*)p1) -> count;
    int c2 = ((count*)p2) -> count;
    if (c1 == c2) return 0;
    if (c1 < c2) return 1;
    return -1;
}

void get_words(const char* filename) {
    FILE *textfile;
    char str[MAX_UNIQUES];
    const char* s = " ";
    char* token;

    textfile = fopen(filename, "r");
    if (textfile == NULL)
        exit(1);
    while (fgets(str, MAX_UNIQUES, textfile)) {
        // printf("%s\n", str);
        token = strtok(str, s);

        while ( token != NULL) {
            // words[num]
            // todo;
        }
        
    }

    fclose(textfile);
}

int main(int argc, char** argv) {
    if (argc != 2)
        exit(1);

    const char* filename = argv[1];

    printf("%s\n", filename);

    words = (struct count*) malloc(MAX_UNIQUES * sizeof(count));

    get_words(filename);

    free(words);
    return 0;
}