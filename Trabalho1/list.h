#ifndef LIST_H
#define LIST_H
#include "util.h"
#include "game.h"
#define ERRO_LIST (-300)

typedef struct list LIST;

LIST *create_list();
int list_insert(LIST *list, GAME *item);
boolean list_inserir_posicao(LIST *list, int posicao, void *item);
boolean list_erase(LIST **list);
boolean list_remove(LIST *list);
boolean list_remover_item(LIST *list, int key);
int list_size(const LIST *list);
boolean list_empty(const LIST *list);
boolean list_full(const LIST *list);
GAME *sequential_search(const LIST *list, int key);
void print_list(const LIST *list);

#endif // LIST_H
