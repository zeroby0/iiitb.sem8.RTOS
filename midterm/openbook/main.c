#include <semaphore.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_STEPS 100

void* new_shared_variable_with_semaphores(size_t len) {
	void* shared_var = mmap(NULL, len, PROT_READ | PROT_WRITE, 
									MAP_ANONYMOUS | MAP_SHARED | MAP_HASSEMAPHORE, -1, 0);

	return shared_var;
}

void* new_shared_variable(size_t len) {
	void* shared_var = mmap(NULL, len, PROT_READ | PROT_WRITE, 
									MAP_ANONYMOUS | MAP_SHARED, -1, 0); // Shared memory

	return shared_var;
}


typedef struct Player {
	int id;
	int current_step;

	sem_t* semaphore_exec;
} Player;

Player* players[4];
int* current_dice_throw;
int* current_player__steps_remaining;

int thread_pids[4];


Player* player_new(int id) {
	Player* player = (Player*) new_shared_variable_with_semaphores(sizeof(Player));

	player->id = id;
	player->current_step = 0;

	char semaphore_name[32];
	sprintf(semaphore_name, "coroutine_semaphore_%d", id);

	if(sem_unlink(semaphore_name) == -1) {
		perror("Sem unlink");
	}

	player->semaphore_exec = sem_open(semaphore_name, O_CREAT | O_EXCL, S_IRUSR|S_IWUSR , 1);
	if(player->semaphore_exec == -1) {
		perror("Error in sem_open");
	}

	int pid = fork();
	if(pid){
		thread_pids[id] = pid;
		return player;
	} else {

		printf("Fork!\n");

		while(1) {
			printf("In fork!\n");
			sem_wait(player->semaphore_exec);
			printf("sem is %d\n", player->semaphore_exec);

			printf("Player %d at %d threw %d\n", id, player->current_step, *current_dice_throw);
			printf("Player %d moves by %d steps.\n", id, *current_dice_throw);

			player->current_step = player->current_step + *current_dice_throw;
			printf("Player %d is now at %d\n", id, player->current_step);

			*current_player__steps_remaining = MAX_STEPS - player->current_step;
			printf("Remaining steps for %d is %d\n", id, *current_player__steps_remaining );

			sem_post(player->semaphore_exec);


			sleep(1);
		}

	}
}


int main() {

	current_dice_throw = (int*)new_shared_variable(sizeof(*current_dice_throw));
	current_player__steps_remaining = (int*)new_shared_variable(sizeof(*current_player__steps_remaining));

	*current_dice_throw = 1;
	*current_player__steps_remaining = 100;



	for(int i = 0; i < 4; i++) {
		players[i] = player_new(i);
	}

	sleep(4);

	for(int i=0;(i < 20) && (*current_player__steps_remaining > 0); i++) {

		printf("%d\n", i);

		*current_dice_throw = rand()%6 + 1; // rand [1->6]
		printf("Dice for player %d is %d\n", i%4, *current_dice_throw);

		sem_post(players[i%4]->semaphore_exec);

		sem_wait(players[i%4]->semaphore_exec);
		printf("Remaining steps for player %d is %d\n", i%4, *current_player__steps_remaining);

		// current_player__steps_remaining will be updated in
		// the coroutine
	}

	for(int i = 0; i < 4; i++) {
		kill(thread_pids[i], SIGKILL);
	}

	return 1;

}