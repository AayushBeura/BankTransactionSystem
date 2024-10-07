#include<stdlib.h>
#include<stdio.h>
#include<time.h>
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void formatIFSC(char *ifsc)
{
    // To capitalize IFSC code (all letters)
    for (int i = 0; ifsc[i] != '\0'; i++)
    {
        if (ifsc[i] >= 'a' && ifsc[i] <= 'z')
        {
            ifsc[i] = ifsc[i] - 'a' + 'A';
        }
    }
}

// Function to format the name
void capitalizeName(char *name) {
    if (name[0] >= 'a' && name[0] <= 'z') {
        name[0] = name[0] - 'a' + 'A';
    }
    for (int i = 1; name[i] != '\0'; i++) {
        if (name[i - 1] == ' ' && name[i] >= 'a' && name[i] <= 'z') {
            name[i] = name[i] - 'a' + 'A';
        }
    }
}

// Function to fetch the current date
void getCurrentDate(char *dateBuffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateBuffer, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

