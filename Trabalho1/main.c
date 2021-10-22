#include "catalog.h"
#include "game.h"
#include "list.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*INFORMAÇÕES:
    NOME: Ryan Souza Sá Teles
    nUSP: 12822062
    Turma: BSI [021]
    Instituto: Instituto de Ciências Matemáticas e de Computação
    Disciplina: Estrutura de Dados I
    Professor: Leonardo Tórtoro Pereira
*/

int main() {
    LIST *catalog;
    FILE *arq1;

    arq1 = fopen("CSV.csv", "r");
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
            key++;
            game = register_game(game_name, game_producer, game_year, key);

            // Registrar na lista catalogo
            list_insert(catalog, game);

            break;
        }
    }

    int size_search = 100;
    int number_of_register = key;
    while (1) {
        char command[2];
        scanf(" %s", command);
        getchar();

        if (select_command(command) == produtora) {
            // ler produtora
            char producer[256];
            scanf(" %[^(\r|\n)]*c", producer);
            getchar();

            // Criar vetor de buscas
            char **found_games;
            found_games = create_found_games_catalog(size_search);
            int search_number = 0;

            // buscar games da produtora
            for (int i = 1; i <= number_of_register; i++) {
                char *search = search_producer(sequential_search(catalog, i), producer);
                if (search != FALSE) {
                    strcpy(found_games[search_number], search);
                    search_number++;
                }
            }

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
            for (int i = 1; i <= number_of_register; i++) {
                char *search = search_year(sequential_search(catalog, i), year);
                if (search != FALSE) {
                    strcpy(found_games[search_number], search);
                    search_number++;
                }
            }

            // imprimir games da produtora
            print_found_games(found_games, search_number);

            // liberar vetor
            free_found_games_catalog(found_games, size_search);

        } else if (select_command(command) == imprime) {
            // imprimir todos os jogos
            for (int i = 1; i <= number_of_register; i++) {
                print_game_name(sequential_search(catalog, i));
            }
        } else if (select_command(command) == posicao) {
            // ler posição
            int position;
            scanf("%d", &position);

            // buscar o jogo que está salvo na posição X
            GAME *game = sequential_search(catalog, position + 1);

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
            
            if (abs(moves - index) <= (key - index)) {
                for (int i = 0; i < moves; i++) {
                    GAME *game1 = sequential_search(catalog, index+1);
                    GAME *game2 = sequential_search(catalog, index + 2);

                    if (swap_games(game1, game2)) {
                        index++;
                    } else {
                        printf("Error");
                        break;
                    }
                }
            }
            // executar movimentação
        } else if (select_command(command) == mover_esquerda) {
            // ler posição

            // ler quantas casas mover

            // buscar posição no catalogo

            // executar movimentação
        } else if (select_command(command) == encerrar) {
            // liberar espaço
            // break
            break;
        }
    }

    return EXIT_SUCCESS;
}