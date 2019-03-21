#ifndef COROUTINE_H
#define COROUTINE_H

#include <pthread.h> // all pthread_*
#include <stdlib.h> // malloc, free
#include <stdio.h> // perror
#include <string.h> // strcpy

typedef struct Coroutine {
	int id; // For debugging purposes
	pthread_t* thread;
	pthread_mutex_t* thread_lock;
	pthread_mutex_t* main_lock;

	// Payload incase you want to add some data
	void* payload1;
	void* payload2;
	void* payload3;
	void* payload4;
} Coroutine;

Coroutine* coroutine_create(int id, void* (*function)(void*), pthread_mutex_t* main_lock);
void coroutine_destroy(Coroutine* coroutine);
void coroutine_resume(Coroutine* coroutine, pthread_mutex_t* main_lock);
void coroutine_pause(Coroutine* coroutine);
pthread_mutex_t* main_lock_create();
void main_lock_destroy(pthread_mutex_t* main_lock);

#endif