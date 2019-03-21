#include "coroutine.h"


// You'll find the following pattern in this code
//
// lock(main)
// 	some code
// lock(main)
// unlock(main)
//
// This kind of code assumes that the main is unlocked
// initially. We lock it the first time, and when we
// lock it the second time, the thread is blocked.
// When it's unlocked from somewhere else, the thread can
// run again. Then we unlock it so that our assumption
// remains true in the next run of the function.
//
// The coroutine unlocks the main here.
// 
// That code is indented so you can be aware
// when the pattern is being used.

Coroutine* coroutine_create(int id, void* (*function)(void*), pthread_mutex_t* main_lock) {

	// You can always assume that the main thread
	// is unlocked when this function is called.
	// This function should only be called from the main thread.
	// We're gonna leave the main thread unlock when we leave.
	// Just the way we found it.
	// Coroutine thread will be locked though.

	// Try to get a lock. As main thread is unlocked,
	// This function will not block.
	if(pthread_mutex_lock(main_lock) != 0) {
		perror("ERR: coroutine_create() -> pthread_mutex_lock()\n\
				Cannot lock main mutex");
		exit(EXIT_FAILURE);
	}




		Coroutine* coroutine = (Coroutine*) malloc(sizeof(Coroutine));

		coroutine->id = id;

		coroutine->main_lock = main_lock;

		pthread_mutex_t* thread_lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
		if(thread_lock == NULL) {
			perror("ERR: coroutine_create() -> thread_lock = malloc()\n\
					Error creating thread_lock");
			exit(EXIT_FAILURE);
		}

		coroutine->thread_lock = thread_lock;

		if(pthread_mutex_init(coroutine->thread_lock, NULL) != 0) {
			perror("ERR: coroutine_create() -> pthread_mutex_init()\n\
					Cannot initiate thread mutex");
			exit(EXIT_FAILURE);
		}

		// Lock the coroutine thread.
		// This will not block as we just
		// created the mutex and it is unlocked by default.
		if(pthread_mutex_lock(coroutine->thread_lock) != 0) {
			perror("ERR: coroutine_create() -> pthread_mutex_lock()\n\
					Cannot lock thread mutex");
			exit(EXIT_FAILURE);
		}

		
		pthread_t* thread = (pthread_t*) malloc(sizeof(pthread_t));
		if(thread == NULL) {
			perror("ERR: coroutine_create() -> thread = malloc()\n\
					Error creating thread");
			exit(EXIT_FAILURE);
		}

		coroutine->thread = thread;
		
		if(pthread_create(coroutine->thread, NULL, function, coroutine) != 0) {
			perror("ERR: coroutine_create() -> pthread_create()\n\
					Cannot create coroutine thread");
			exit(EXIT_FAILURE);
		}





	// This is to make sure parent thread doesn't continue
	// before coroutine is created. Main thread resumes when
	// coroutine encounters a coroutine_pause
	if(pthread_mutex_lock(main_lock) != 0) {
		perror("ERR: coroutine_create() -> pthread_mutex_lock()\n\
				Cannot lock main mutex");
		exit(EXIT_FAILURE);
	}

	// Now main thread can continue.
	// Let's unlock it to leave things
	// the way we found them.
	if(pthread_mutex_unlock(coroutine->main_lock) != 0) {
		perror("coroutine_create() -> pthread_mutex_unlock()\n\
				Error unlocking main in creation");
		// exit(EXIT_FAILURE);
	}

	return coroutine;
}

void coroutine_destroy(Coroutine* coroutine) {


	if( pthread_mutex_destroy(coroutine->thread_lock) != 0) {
		perror("coroutine_destroy() -> pthread_mutex_destroy()\n\
				Error while destroying coroutine");
		// exit(EXIT_FAILURE);		
	}

	if( pthread_cancel(*(coroutine->thread)) != 0) {
		perror("coroutine_destroy() -> pthread_cancel()\n\
				Error while destroying coroutine");
		// exit(EXIT_FAILURE);
	}

	free(coroutine->thread);
	free(coroutine->thread_lock);
	free(coroutine);
}

void coroutine_resume(Coroutine* coroutine, pthread_mutex_t* main_lock) {
	// You can always assume that when the function is called
	// main thread is unlocked and
	// coroutine thread is locked

	// so we're locking an unlocked main thread
	// and this doesnt block
	if(pthread_mutex_lock(main_lock) != 0) {
		perror("ERR: coroutine_resume() -> pthread_mutex_lock()\n\
				Cannot lock main mutex");
		exit(EXIT_FAILURE);
	}

		// Now we unlock the coroutine thread
		// it was blocked, but now it runs
		// this call is also not blocking to main thread
		// fprintf(stderr, "coroutine_resume() unlocking coroutine thread %d\n", coroutine->id);
		if(pthread_mutex_unlock(coroutine->thread_lock) != 0) {
			perror("coroutine_resume() -> pthread_mutex_unlock()\n\
					Error unlocking coroutine thread to execute");
			// exit(EXIT_FAILURE);
		}

	// as we locked the main thread at the start of this function
	// this will block until main thread is unlocked by coroutine
	// when it calls coroutine_pause
	// fprintf(stderr, "coroutine_resume() locking main thread %d\n", coroutine->id);
	if(pthread_mutex_lock(coroutine->main_lock) != 0) {
		perror("coroutine_resume() -> pthread_mutex_lock()\n\
				Error locking main thread to resume coroutine");
		// exit(EXIT_FAILURE);
	}

	// Now unlock the mutex we locked in the last step.
	// Good, we're leaving main mutex and coroutine mutex
	// the way we found them.
	if(pthread_mutex_unlock(coroutine->main_lock) != 0) {
		perror("coroutine_create() -> pthread_mutex_unlock()\n\
				Error unlocking main thread");
		// exit(EXIT_FAILURE);
	}
}

void coroutine_pause(Coroutine* coroutine) {
	// Never call this function from the main thread.

	// You can always assume that both main thread and
	// coroutine thread are locked when this is called.

	// Unlock the main thread. It was locked when coroutine_resume
	// was called in the main thread.
	// fprintf(stderr, "coroutine_pause() unlocking main thread %d\n", coroutine->id);
	if(pthread_mutex_unlock(coroutine->main_lock) != 0) {
		perror("coroutine_pause() -> pthread_mutex_unlock()\n\
				Error unlocking main thread to pause coroutine");
		// exit(EXIT_FAILURE);
	}

	// Lock the coroutine thread.
	// This will block the coroutine as
	// coroutine is locked already.
	// It will be unlocked when couroutine_resume is called again.
	// fprintf(stderr, "coroutine_pause() locking coroutine thread %d\n", coroutine->id);
	if(pthread_mutex_lock(coroutine->thread_lock) != 0) {
		perror("coroutine_pause() -> pthread_mutex_lock()\n\
				Error locking coroutine thread to pause");
		// exit(EXIT_FAILURE);
	}
}

pthread_mutex_t* main_lock_create() {
	pthread_mutex_t* main_lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));

	if(main_lock == NULL) {
		perror("ERR: main() -> malloc()\n\
			Error creating main_lock");
		exit(EXIT_FAILURE);
	}

	if(pthread_mutex_init(main_lock, NULL) != 0) {
		perror("ERR: main() -> pthread_mutex_init()\n\
				Cannot initiate main mutex");
		exit(EXIT_FAILURE);
	}

	return main_lock;
}

void main_lock_destroy(pthread_mutex_t* main_lock) {
	if( pthread_mutex_destroy(main_lock) != 0) {
		perror("coroutine_destroy() -> pthread_mutex_destroy()\n\
				Error while destroying coroutine");
		// exit(EXIT_FAILURE);		
	}

	free(main_lock);
}
