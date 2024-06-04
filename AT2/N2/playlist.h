#ifndef PLAYLIST_H
#define PLAYLIST_H

typedef struct Musica {
    char artista[100];
    char nome[100];
    struct Musica* prev;
    struct Musica* next;
} Musica;

typedef struct Playlist {
    Musica* head;
    Musica* tail;
    Musica* current;
} Playlist;


void inicializarPlaylist(Playlist* playlist);
void carregarPlaylist(Playlist* playlist, const char* filename);
void salvarPlaylist(Playlist* playlist, const char* filename);
void exibirPlaylist(Playlist* playlist);
void exibirPlaylistOrdenada(Playlist* playlist);
void inserirMusica(Playlist* playlist, const char* artista, const char* nome);
void removerMusica(Playlist* playlist, const char* nome);
Musica* buscarMusica(Playlist* playlist, const char* nome);
void avancarMusica(Playlist* playlist);
void retornarMusica(Playlist* playlist);
void liberarPlaylist(Playlist* playlist);

#endif 
