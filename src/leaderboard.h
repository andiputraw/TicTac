#ifndef _LEADERBOARD_H_
#define _LEADERBOARD_H_

#include "player.h"
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
// #include <inttypes.h>

#define LEADERBOARD_PATH "leaderboard.tictac"
#define HISTORY_PATH "history.tictac"
#define TWO_MB 1024 * 1024 * 2

typedef struct {
    // File taruh leaderboard
    FILE *leaderboard;
    char *leaderboard_buffer;

    // file taruh history
    FILE *history;
    char *history_buffer;
} Leaderboard;

/**
 * History Spec
 * Value ditaruh dalam bentuk unsigned char
 * 
 * Offset       panjang Desc 
 * 0            1       game mode dari game yang dipilih
 * 1            1       skor dari player 1
 * 2            1       skor dari player 2
 * 3            1       panjang dari nama player 1
 * 4            p1.len  nama dari player 1
 * 5 + p1.len   1       panjang dari nama player 2
 * 5 + p1.len   p2.len  nama dari player 2
 * 5 + p1.len   25      state dari game board sebagai array dengan panjang 25 byte
 * + p2.len
 * 
 */

typedef struct {
    unsigned char game_mode;
    Player p1;
    Player p2;
    int BoardState[25];
} History;

/**
 * Setiap struct, akan ada informasi panjang dari buffer. dinotasikan sebanyak 1 byte
 * Player Elo Spec
 * offset        panjang  desc
 * 0             1        panjang dari nama
 * 1             nama_len nama dari user
 * 2 + nama_len  4        elo dari player
 */
typedef struct {
    char name[20];
    int elo;
} PlayerElo;


// Membuka file yang dibutuhkan untuk leaderboard dan history. Prosedur ini harus dieksekusi saat dibuka.
void OpenLeaderboard(Leaderboard *l);
// Menutup file yang dibutuhkan untuk leaderboard dan history. Prosedur ini harus diekskusi setelah program selesai dijalankan.
void CloseLeaderboard(Leaderboard *l);
// Merefresh buffer leaderboard jika history baru sudah ditulis ke dalam file.
void RefreshLeaderboardBuffer(Leaderboard *l);
// Menulis history ke file history.
void WriteHistory(Leaderboard *l, History *h);
// Membaca history
// len = berapa banyak history yang ingin dibaca
// offset = offset ke berapa history yang ingin dibaca
bool ReadHistory(Leaderboard *l, History *h, int len, int offset);

// Mengupdate elo player dengan nama yang sama, jika nama tidak ditemukan, akan dibuat yang baru.
void WritePlayerElo(Leaderboard *l, PlayerElo *p);

// ambil player elo berdasarkan nama. Mengembalikan true jika ditemukan, dan false jika tidak ditemukan.
bool GetPlayerElo(Leaderboard *l, PlayerElo *p, char * name);

// Ambil panjang seluruh history
int GetHistoryLen(Leaderboard *l);

// Membaca leaderboard
// *p = array dari sebuah PlayerElo. panjangnya harus paling tidak 5.
bool ReadTop5Player(Leaderboard *l, PlayerElo *p);

// konversi dari PlayerElo ke bytes.
int PlayerEloToBuffer(char *buffer, PlayerElo h);
// konversi dari bytes ke Player Elo.
int BufferToPlayerElo(char *buffer, PlayerElo *h);

// konversi dari History ke bytes.
int HistoryToBuffer(char *buffer, History h);
// konversi dari bytes ke History.
int BufferToHistory(char *buffer, History *h);

void TestHistory();

#endif // _LEADERBOARD_H_