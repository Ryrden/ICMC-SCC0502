#ifndef JOGO_H
#define JOGO_H
#include "util.h"

typedef struct game_st GAME;

GAME *register_game(char *name, char *producer, int year, int key);

int get_key(const GAME *game);

GAME *create_empty_item();

boolean swap_games(GAME *game1, GAME *game2);

char *search_producer(GAME *game, char *game_producer);

char *search_year(GAME *game, int game_year);

void print_game_name(GAME *game);

int set_key(GAME *item, int key);

#endif // JOGO_H