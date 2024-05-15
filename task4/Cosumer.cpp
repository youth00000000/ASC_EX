#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define NUM_CHARS 2048

char buffer[BUFFER_SIZE];
int write_idx = 0;
int read_idx = 0;
int count = 0; // to track the number of items in the buffer

pthread_mutex_t lock;
pthread_cond_t not_empty;
pthread_cond_t not_full;

FILE* fp;

void* producer(void* arg) {
    for (int i = 0; i < NUM_CHARS; i++) {
        pthread_mutex_lock(&lock);
        while (count == BUFFER_SIZE) // Buffer full
            pthread_cond_wait(&not_full, &lock);
        
        buffer[write_idx % BUFFER_SIZE] = i % 26 + 'a';
        write_idx++;
        count++;
        
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < NUM_CHARS; i++) {
        pthread_mutex_lock(&lock);
        while (count == 0) // Buffer empty
            pthread_cond_wait(&not_empty, &lock);
        
        char c = buffer[read_idx % BUFFER_SIZE];
        putc(c, fp);
        read_idx++;
        count--;
        
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void check(FILE* fp) {
    rewind(fp); // Ensure we start reading from the beginning of the file
    for (int i = 0; i < NUM_CHARS; i++) {
        char c = fgetc(fp);
        char expected = i % 26 + 'a';
        if (c != expected) {
            printf("Error at byte %d: expected %c, got %c\n", i, expected, c);
        }
    }
    printf("Finish, no error found\n");
}

int main() {
    fp = fopen("tmp.txt", "w+");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    fclose(fp);

    fp = fopen("tmp.txt", "r");
    check(fp);
    fclose(fp);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
