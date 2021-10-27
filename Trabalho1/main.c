#include "catalog.h"
#include "game.h"
#include "list.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INEXISTENT_KEY (-32000)

/*INFORMAÇÕES:
    NOME: Ryan Souza Sá Teles
    nUSP: 12822062
    Turma: BSI [021]
    Instituto: Instituto de Ciências Matemáticas e de Computação
    Disciplina: Estrutura de Dados I
    Professor: Leonardo Tórtoro Pereira
*/

void producers_found(LIST *catalog, char **found_games, char *producer, int *search_number);
void years_found(LIST *catalog, char **found_games, int year, int *search_number);
void print_catalog(LIST *catalog);
void move_right(LIST *catalog, int index, int moves);
void move_left(LIST *catalog, int index, int moves);
int *create_duplicate_games(int registers);
void found_duplicates(LIST *catalog, int *duplicate_games);
void free_duplicate_games(int **duplicate_games);
void move_keys_to_right(LIST *catalog, int key_to_move);

int main() {
    LIST *catalog;
    FILE *arq1;

    arq1 = fopen("3.csv", "r");
    if (arq1 == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    catalog = create_list();

    char line[200];
    int key = 0;
    while (fgets(line, sizeof(line), arq1)) {

        while (1) {
            GAME *game;
            char *game_name = strtok(line, ";");
            // TRATAR ENTRADA - caracteres: ´╗┐
            while (*game_name < 'A')
                game_name++;

            int game_year = atoi(strtok(NULL, ";"));

            char *game_producer = strtok(NULL, ";");
            game_producer[strlen(game_producer) - 1] = '\0';

            // Registrar na estrutura game
            game = register_game(game_name, game_producer, game_year, key);
            key++;

            // Registrar na lista catalogo
            list_insert(catalog, game);

            break;
        }
    }

    int size_search = 100;
    int number_of_register = list_size(catalog);
    while (1) {
        char command[2];
        scanf("%s", command);
        getchar();

        if (select_command(command) == remove_duplicados) {
            int *duplicate_games = create_duplicate_games(number_of_register);

            // busca duplicados e salva a chave deles no vetor
            found_duplicates(catalog, duplicate_games);

            // exclui todos games do catalogo enquanto o vetor tem chaves
            int index = 0;
            while (duplicate_games[index] != INEXISTENT_KEY) {
                int key_to_remove = duplicate_games[index];
                if (!list_remove_item(catalog, key_to_remove))
                    perror("Error to remove item");
                int key_to_move = key_to_remove;
                move_keys_to_right(catalog, key_to_move);
                index++;
            }
            // altera tamanho do vetor criado
            decrease_list_size(catalog, index);

            // limpa vetor
            free_duplicate_games(&duplicate_games);

        } else if (select_command(command) == produtora) {
            // ler produtora
            char producer[256];
            scanf(" %[^(\r|\n)]*c", producer);
            getchar();

            // Criar vetor de buscas
            char **found_games;
            found_games = create_found_games_catalog(size_search);
            int search_number = 0;

            // buscar games da produtora
            producers_found(catalog, found_games, producer, &search_number);

            // imprimir games da produtora
            print_found_games(found_games, search_number);

            // liberar vetor
            free_found_games_catalog(found_games, size_search);

        } else if (select_command(command) == ano) {
            // ler ano
            int year;
            scanf("%d", &year);

            // Criar vetor de buscas
            char **found_games;
            found_games = create_found_games_catalog(size_search);
            int search_number = 0;

            // buscar games deste ano
            years_found(catalog, found_games, year, &search_number);

            // imprimir games da produtora
            print_found_games(found_games, search_number);

            // liberar vetor
            free_found_games_catalog(found_games, size_search);

        } else if (select_command(command) == imprime) {
            // imprimir todos os jogos
            print_catalog(catalog);
        } else if (select_command(command) == posicao) {
            // ler posição
            int position;
            scanf("%d", &position);

            // buscar o jogo que está salvo na posição X
            GAME *game = sequential_search(catalog, position);

            // imprimir jogo da posiçaõ
            print_game_name(game);

        } else if (select_command(command) == mover_direita) {
            // ler posição
            int index;
            scanf("%d", &index);
            // ler quantas casas mover
            int moves;
            scanf("%d", &moves);
            // buscar posição no catalogo

            if (index + moves < number_of_register) {
                move_right(catalog, index, moves);
            } else {
                printf("Error, out of range");
            }
        } else if (select_command(command) == mover_esquerda) {
            // ler posição
            int index;
            scanf("%d", &index);
            // ler quantas casas mover
            int moves;
            scanf("%d", &moves);
            // buscar posição no catalogo

            if (index - moves > 0) {
                move_left(catalog, index, moves);
            } else {
                printf("Error, out of range");
            }
        } else if (select_command(command) == encerrar) {
            // liberar espaço
            list_erase(&catalog);
            fclose(arq1);
            // break
            break;
        }
    }
    return EXIT_SUCCESS;
}

void producers_found(LIST *catalog, char **found_games, char *producer, int *search_number) {
    int records = list_size(catalog);
    for (int i = 0; i < records; i++) {
        char *search = search_producer(sequential_search(catalog, i), producer);
        if (search != FALSE) {
            strcpy(found_games[*search_number], search);
            *search_number += 1;
        }
    }
}

void years_found(LIST *catalog, char **found_games, int year, int *search_number) {
    int records = list_size(catalog);
    for (int i = 0; i < records; i++) {
        char *search = search_year(sequential_search(catalog, i), year);
        if (search != FALSE) {
            strcpy(found_games[*search_number], search);
            *search_number += 1;
        }
    }
}

void print_catalog(LIST *catalog) {
    int records = list_size(catalog);
    for (int i = 0; i < records; i++) {
        print_game_name(sequential_search(catalog, i));
    }
}

void move_right(LIST *catalog, int index, int moves) {
    for (int i = 0; i < moves; i++) {
        GAME *game1 = sequential_search(catalog, index);
        GAME *game2 = sequential_search(catalog, index + 1);
        // executar movimentação
        if (swap_games(game1, game2)) {
            index++;
        } else {
            printf("Error");
            break;
        }
    }
}

void move_left(LIST *catalog, int index, int moves) {
    for (int i = 0; i < moves; i++) {
        GAME *game1 = sequential_search(catalog, index);
        GAME *game2 = sequential_search(catalog, index - 1);
        // executar movimentação
        if (swap_games(game1, game2)) {
            index++;
        } else {
            printf("Error");
            break;
        }
    }
}

int *create_duplicate_games(int registers) {
    int *duplicate_games;

    duplicate_games = (int *)malloc(sizeof(int) * registers);

    if (duplicate_games == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; i < registers; i++)
        duplicate_games[i] = INEXISTENT_KEY;

    return duplicate_games;
}

void found_duplicates(LIST *catalog, int *duplicate_games) {
    int records = list_size(catalog);

    int index = 0;

    for (int i = 0; i < records; i++) {
        GAME *duplicate_game = sequential_search(catalog, i);
        for (int j = i + 1; j < records; j++) {
            GAME *possible_duplicate_game = sequential_search(catalog, j);

            if (compare_games(duplicate_game, possible_duplicate_game)) {
                int key = get_key(possible_duplicate_game);
                boolean can_add = TRUE;
                for (int k = 0; k < records; k++) {
                    if (duplicate_games[k] == key) {
                        can_add = FALSE;
                        break;
                    }
                }
                if (can_add) {
                    duplicate_games[index] = get_key(possible_duplicate_game);
                    index++;
                }
            }
        }
    }
}

void free_duplicate_games(int **duplicate_games) {
    free(*duplicate_games);
}

void move_keys_to_right(LIST *catalog, int key_to_move) {
    int size_catalog = list_size(catalog);

    for (int i = key_to_move + 1; i < size_catalog; i++) {
        GAME *game = sequential_search(catalog, i);
        decrease_key_value(game);
    }
}