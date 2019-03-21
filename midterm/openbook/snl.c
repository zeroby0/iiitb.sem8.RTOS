#include "coroutine.h"

#define number_of_players 4

typedef struct Player {
	int id;
	int current_position;

	Coroutine* coroutine;	
} Player;

int final_destination = 100;

int ladder_foot[] = {10, 30, 50, 70};
int ladder_head[] = {20, 40, 60, 80};

int snake_mouth[] = {15, 35, 55, 75};
int snake_tail[]  = {05, 25, 45, 65};

void* play(void* t_coroutine) {
	Coroutine* coroutine = (Coroutine*) t_coroutine;

	while(1) {
		coroutine_pause(coroutine);

		Player* player = (Player*) coroutine->payload1;

		int throw = rand()%6 + 1;

		printf("Player %d at position %d threw %d.\n", player->id, player->current_position, throw);
		printf("The new location is %d.\n", (player->current_position + throw));

		player->current_position = player->current_position + throw;

		for(int i = 0; i < sizeof(ladder_foot)/sizeof(int); i++) {
			if(player->current_position == ladder_foot[i]) {
				printf("But, there's a ladder at %d.\n", player->current_position);
				player->current_position = ladder_head[i];
				printf("The final position for player %d is %d.\n", player->id, player->current_position);
			}

			if(player->current_position == snake_mouth[i]) {
				printf("But, there's a snake at %d.\n", player->current_position);
				player->current_position = snake_tail[i];
				printf("The final position for player %d is %d.\n", player->id, player->current_position);
			}
		}
	}

	return NULL;
}

Player* player_create(int id, pthread_mutex_t* main_lock) {
	Player* player = (Player*) malloc(sizeof(Player));

	player->id = id;
	player->current_position = 0;

	player->coroutine = coroutine_create(id, &play, main_lock);

	(player->coroutine)->payload1 = player;

	return player;
}

void player_destroy(Player* player) {
	coroutine_destroy(player->coroutine);
	free(player);
}

int main() {
	time_t t;
	srand((unsigned) time(&t));

	pthread_mutex_t*  main_lock = main_lock_create();
	
	Player* players[number_of_players];

	for(int i = 0; i < number_of_players; i++) {
		players[i] = player_create(i, main_lock);
	}

	for(int i = 0; i < 4000; i++) {
		if(players[i%4]->current_position >= final_destination) {continue;}
		coroutine_resume(players[i%4]->coroutine, main_lock);
	}

	for(int i = 0; i < number_of_players; i++) {
		printf("Player %d at %d\n", players[i]->id, players[i]->current_position);
	}

	main_lock_destroy(main_lock);

}