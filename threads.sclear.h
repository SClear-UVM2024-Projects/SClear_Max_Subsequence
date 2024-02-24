//
// Created by Sam Clear on 3/1/23.
//

#ifndef ASSIGNMENT3_THREADS_SCLEAR_H
#define ASSIGNMENT3_THREADS_SCLEAR_H

int readFile(char *filename, int *numChars, char *buffer);
void *findMaxSumSeq(void *param);

typedef struct {
    char *A; // the digits themselves--this is a pointer to your big buffer
    int start; // first position that thread should look at
    int end; // last position that this thread should look at
    int bestpos; // ending position of the best sequence found by this thread
    int max; // length of the best sequence found by this thread
} ThreadInfo;

// here’s where you’ll create the threads that do the actual searching

#endif //ASSIGNMENT3_THREADS_SCLEAR_H
