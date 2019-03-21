#include <stdio.h> // printf
#include <string.h> // strcpy

#include "coroutine.h"

char message[10] = "\0";
void* print_message_1(void* t_coroutine) {
	Coroutine* coroutine = (Coroutine*) t_coroutine;

	while(1) {
		coroutine_pause(coroutine);
		printf("1: %s\n", message);
	}

	return NULL;
}

void* print_message_2(void* t_coroutine) {
	Coroutine* coroutine = (Coroutine*) t_coroutine;

	while(1) {
		coroutine_pause(coroutine);
		printf("2: %s\n", message);
	}

	return NULL;
}

int main() {
	pthread_mutex_t* main_lock = main_lock_create();

	Coroutine* coroutine1 = coroutine_create(1, &print_message_1, main_lock);
	Coroutine* coroutine2 = coroutine_create(2, &print_message_1, main_lock);
	Coroutine* coroutine3 = coroutine_create(3, &print_message_2, main_lock);
	Coroutine* coroutine4 = coroutine_create(4, &print_message_2, main_lock);

	strcpy(message, "Hello 1");
	coroutine_resume(coroutine1, main_lock);

	strcpy(message, "Hello 2");
	coroutine_resume(coroutine2, main_lock);

	strcpy(message, "Hello 3");
	coroutine_resume(coroutine3, main_lock);

	strcpy(message, "Hello 3");
	coroutine_resume(coroutine3, main_lock);

	strcpy(message, "Hello 1");
	coroutine_resume(coroutine1, main_lock);

	strcpy(message, "Hello 4");
	coroutine_resume(coroutine4, main_lock);

	coroutine_destroy(coroutine1);
	coroutine_destroy(coroutine2);
	coroutine_destroy(coroutine3);
	coroutine_destroy(coroutine4);

	main_lock_destroy(main_lock);

	return 0;
}