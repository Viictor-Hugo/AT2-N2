#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "playlist.h"


void inicializarPlaylist(Playlist* playlist) {
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->current = NULL;
}

void carregarPlaylist(Playlist* playlist, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* artista = strtok(line, ";");
        char* nome = strtok(NULL, "\n");
        if (artista && nome) {
            inserirMusica(playlist, artista, nome);
        }
    }

    fclose(file);
}

void salvarPlaylist(Playlist* playlist, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Musica* current = playlist->head;
    if (current) {
        do {
            fprintf(file, "%s;%s\n", current->artista, current->nome);
            current = current->next;
        } while (current != playlist->head);
    }

    fclose(file);
}

void exibirPlaylist(Playlist* playlist) {
    Musica* current = playlist->head;
    if (current) {
        do {
            printf("Artista: %s, Musica: %s\n", current->artista, current->nome);
            current = current->next;
        } while (current != playlist->head);
    } else {
        printf("Playlist vazia.\n");
    }
}

int compararMusicas(const void* a, const void* b) {
    Musica* musicaA = *(Musica**)a;
    Musica* musicaB = *(Musica**)b;
    return strcmp(musicaA->nome, musicaB->nome);
}

void exibirPlaylistOrdenada(Playlist* playlist) {
    Musica* current = playlist->head;
    if (current) {
        int count = 0;
        do {
            count++;
            current = current->next;
        } while (current != playlist->head);

        Musica** array = malloc(count * sizeof(Musica*));
        current = playlist->head;
        for (int i = 0; i < count; i++) {
            array[i] = current;
            current = current->next;
        }

        qsort(array, count, sizeof(Musica*), compararMusicas);

        for (int i = 0; i < count; i++) {
            printf("Artista: %s, Musica: %s\n", array[i]->artista, array[i]->nome);
        }

        free(array);
    } else {
        printf("Playlist vazia.\n");
    }
}

void inserirMusica(Playlist* playlist, const char* artista, const char* nome) {
    Musica* novaMusica = (Musica*)malloc(sizeof(Musica));
    strcpy(novaMusica->artista, artista);
    strcpy(novaMusica->nome, nome);

    if (playlist->head == NULL) {
        novaMusica->next = novaMusica;
        novaMusica->prev = novaMusica;
        playlist->head = novaMusica;
        playlist->tail = novaMusica;
        playlist->current = novaMusica;
    } else {
        novaMusica->next = playlist->head;
        novaMusica->prev = playlist->tail;
        playlist->tail->next = novaMusica;
        playlist->head->prev = novaMusica;
        playlist->tail = novaMusica;
    }
}

void removerMusica(Playlist* playlist, const char* nome) {
    Musica* musica = buscarMusica(playlist, nome);
    if (musica) {
        if (musica->next == musica) {
            playlist->head = NULL;
            playlist->tail = NULL;
            playlist->current = NULL;
        } else {
            musica->prev->next = musica->next;
            musica->next->prev = musica->prev;
            if (playlist->head == musica) playlist->head = musica->next;
            if (playlist->tail == musica) playlist->tail = musica->prev;
            if (playlist->current == musica) playlist->current = musica->next;
        }
        free(musica);
    } else {
        printf("Musica nao encontrada.\n");
    }
}

Musica* buscarMusica(Playlist* playlist, const char* nome) {
    Musica* current = playlist->head;
    if (current) {
        do {
            if (strcmp(current->nome, nome) == 0) {
                return current;
            }
            current = current->next;
        } while (current != playlist->head);
    }
    return NULL;
}

void avancarMusica(Playlist* playlist) {
    if (playlist->current) {
        playlist->current = playlist->current->next;
        printf("Musica atual: %s - %s\n", playlist->current->artista, playlist->current->nome);
    } else {
        printf("Playlist vazia.\n");
    }
}

void retornarMusica(Playlist* playlist) {
    if (playlist->current) {
        playlist->current = playlist->current->prev;
        printf("Musica atual: %s - %s\n", playlist->current->artista, playlist->current->nome);
    } else {
        printf("Playlist vazia.\n");
    }
}

void liberarPlaylist(Playlist* playlist) {
    if (playlist->head) {
        Musica* current = playlist->head;
        do {
            Musica* temp = current;
            current = current->next;
            free(temp);
        } while (current != playlist->head);
    }
    playlist->head = NULL;
    playlist->tail = NULL;
    playlist->current = NULL;
}

int main() {
    Playlist playlist;
    inicializarPlaylist(&playlist);
    carregarPlaylist(&playlist, "musicas.txt");

    int opcao;
    char artista[100];
    char nome[100];

    do {
        printf("\nMenu:\n");
        printf("1. Exibir playlist\n");
        printf("2. Exibir playlist ordenada pelo nome das musicas\n");
        printf("3. Inserir nova musica\n");
        printf("4. Remover musica\n");
        printf("5. Buscar musica\n");
        printf("6. Avancar para proxima musica\n");
        printf("7. Retornar a musica anterior\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                exibirPlaylist(&playlist);
                break;
            case 2:
                exibirPlaylistOrdenada(&playlist);
                break;
            case 3:
                printf("Digite o nome do artista: ");
                fgets(artista, sizeof(artista), stdin);
                artista[strcspn(artista, "\n")] = '\0';
                printf("Digite o nome da musica: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                inserirMusica(&playlist, artista, nome);
                salvarPlaylist(&playlist, "musicas.txt");
                break;
            case 4:
                printf("Digite o nome da musica a ser removida: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                removerMusica(&playlist, nome);
                salvarPlaylist(&playlist, "musicas.txt");
                break;
            case 5:
                printf("Digite o nome da musica a ser buscada: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';
                Musica* musica = buscarMusica(&playlist, nome);
                if (musica) {
                    printf("Musica encontrada: %s - %s\n", musica->artista, musica->nome);
                } else {
                    printf("Musica nao encontrada.\n");
                }
                break;
            case 6:
                avancarMusica(&playlist);
                break;
            case 7:
                retornarMusica(&playlist);
                break;
            case 0:
                liberarPlaylist(&playlist);
                printf("Saindo...\n");
                break;
            default:
                printf("Opçãao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}
