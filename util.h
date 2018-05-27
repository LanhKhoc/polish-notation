#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

char* toLowerCase(char *str) {
    char *tmp = calloc (strlen(str), sizeof(char));
    for (int i=0; i<strlen(str); i++) {
        tmp[i] = '\0';
    }

    for (int i=0; str[i]; i++) {
        tmp[i] = tolower(str[i]);
    }

    return tmp;
}

char *trim(char *s) {
    while (isspace((unsigned char) *s))
        s++;
    if (*s) {
        char *p = s;
        while (*p)
            p++;
        while (isspace((unsigned char) *(--p)));
        p[1] = '\0';
    }
    return s;
}

char** split(char* str, const char a_delim) {
    char** result    = 0;
    char *a_str = calloc(strlen(str), sizeof (char));
    strcpy(a_str, str);
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result) {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void consoleLog(char *str) {
    int i;
    printf("\n");
    for (i=0; str[i]; i++) {
        putchar(str[i]);
    }
}

#endif // UTIL_H_INCLUDED
