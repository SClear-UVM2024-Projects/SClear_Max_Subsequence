//
// Created by Sam Clear on 3/1/23.
//

#include <pthread.h>
#include "printf.h"
#include "threads.sclear.h"
#include "stdlib.h"
#include "string.h"

#define BUFLEN 10100
#define NUM_THREADS 4
#define INITIAL_MAXIMUMS 0;
#define LINE_LEN 256

int main(int argc, char *argv[]) {
    // Define the temporary parameters for the function
    int rc = 0, numChars, segmentSize, iterator, iteratorTwo;
    int currentStartIndex, currentMaxSequenceLength = 0, currentMaxSequencePosition = 0;

    // MUST BE INITIALIZED WITH EACH FILE, shows the number of characters in the pi file!
    numChars = 10000;
    char buffer[BUFLEN];

    // Can also be converted to "= string/char[]" for testing within an IDE
    char *fileName = "../pi10000.asc";

    // Initialize the array for each thread's info
    ThreadInfo tData[NUM_THREADS];
    pthread_t tids[NUM_THREADS];

    // Read in all of the values from the file
    rc = readFile(fileName, &numChars, buffer);

    // Create bounds for each of the threads in the thread list
    segmentSize = numChars / NUM_THREADS;
    currentStartIndex = 0;
    for (iterator = 0; iterator < NUM_THREADS; iterator++) {
        tData[iterator].start = currentStartIndex;
        tData[iterator].end = currentStartIndex + segmentSize - 1;
        if (iterator == 3) {
            tData[iterator].end = currentStartIndex + segmentSize;
        }
        currentStartIndex = currentStartIndex + segmentSize;
        tData[iterator].A = (buffer);
    }

    // Create each of the child threads to complete the program
    for (iterator = 0; iterator < NUM_THREADS; iterator++) {
        pthread_create(&tids[iterator], NULL, findMaxSumSeq, &tData[iterator]);
    }

    // Wait for each thread to terminate, preparing to evaluate each value
    for (iterator = 0; iterator < NUM_THREADS; iterator++) {
        pthread_join(tids[iterator], NULL);
    }

    // Find out what is the longest sequence of all four threads
    for (iterator = 0; iterator < NUM_THREADS; iterator++) {
        if (tData[iterator].max > currentMaxSequenceLength) {
            currentMaxSequenceLength = tData[iterator].max;
            currentMaxSequencePosition = tData[iterator].bestpos;
        }
    }

    // Finally, print out the maximum sequence which began at which position, and conclude the function
    printf("Maximum sequence length found to be %d at position %d \nin the file %s", currentMaxSequenceLength, currentMaxSequencePosition, fileName);

    if (rc != 0) {
        return 8;
    }
    return 0;
}

int readFile(char *filename, int *numChars, char *buffer) {

    // Open the file if possible, returning an error if an invalid file name was entered!
    FILE *fp = fopen(filename, "r"); // filename is the char* passed to your readFile() function

    // Initialize the first line read from the file
    char line[LINE_LEN];
    char *chp = fgets(line, LINE_LEN, fp);

    // Initialize temporary variables
    int len, iterator, endOfLine = 0, currentIndex = 0;

    // Check if the file is empty
    if (chp == NULL) {
        printf("file is empty\n");
        fclose(fp);
        return 4;
    }

    // Continue reading lines until there is no line to read in the program
    while (chp != NULL) {

        // Remove any new-line characters that may clog up reading the characters in the program
        len = strlen(line);
        if (line[len-1] == '\n') {
            line[len-1] = '\0';
            len = len - 1;
        }

        // Create an indicator variable for when the end of the line
        // has been reached, and an iterator.
        iterator = 0, endOfLine = 0;

        // Copy each character from the file that is between 0-9 in
        // ASCII values into the buffer
        while (endOfLine == 0) {
            if (line[iterator] == '\0') {
                endOfLine = 1;
            } else if (line[iterator] >= 48 && line[iterator] <= 57) {
                buffer[currentIndex] = line[iterator];
                currentIndex++;
            }
            iterator++;
        }

        // Get the next line
        chp = fgets(line, LINE_LEN, fp);
    }

    // return 4 if anything goes wrong, return 0 otherwise!
    return 0;
}

void *findMaxSumSeq(void *param) {

    // Initializing thread variables
    ThreadInfo *data;
    int maxSequenceLength, maxSequenceLengthFound, maxSequencePosition, iterator, iteratorTwo, tempSum = 0, tempSequenceLength = 0, currValue;

    data = (ThreadInfo *) param;
    printf("Next thread, checking range %d to %d\n",
           data->start, data->end);

    // Initializing starting values for maximum sequence sum and length
    maxSequencePosition = INITIAL_MAXIMUMS;
    maxSequenceLength = INITIAL_MAXIMUMS;
    maxSequenceLengthFound = INITIAL_MAXIMUMS;

    // Iterate through each item in the list for the longest subsequence
    for (iterator = data->start; iterator <= data->end; ++iterator) {

        // Record the current value to be evaluated for previous-nums-max-sequence
        currValue = data->A[iterator] - 48;
        maxSequenceLength = data->A[iterator] - 48;
        iteratorTwo = iterator - 1;
        tempSum = 0, tempSequenceLength = 1;

        // For each iteration check the following:
        // - Is the value of the tempSum < currValue
        // - Same for tempSequenceLength
        // - Is the iterator within the bounds of the program?
        for (iteratorTwo; tempSum <= currValue && tempSequenceLength <= currValue && iteratorTwo >= 0; --iteratorTwo) {
            tempSum += data->A[iteratorTwo] - 48;
            tempSequenceLength += 1;
            if (tempSum == currValue && tempSequenceLength > maxSequenceLengthFound) {
                maxSequencePosition = iterator;
                maxSequenceLengthFound = tempSequenceLength;
            }
        }
    }

    data->max = maxSequenceLengthFound;
    data->bestpos = maxSequencePosition;
    printf("Maximum Length String for this thread is %d, found at position %d\n", data->max, data->bestpos);
    pthread_exit(NULL);
}