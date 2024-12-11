#ifndef _LEADERBOARD_H_
#define _LEADERBOARD_H_

#include "player.h"
#include <stdio.h>
#include <errno.h>
// #include <inttypes.h>

#define LEADERBOARD_PATH "leaderboard.tictac"
#define HISTORY_PATH "history.tictac"
#define TWO_MB 1024 

typedef struct {
    // File taruh leaderboard
    FILE *leaderboard;
    char leaderboard_buffer[TWO_MB];

    // file taruh history
    FILE *history;
    char history_buffer[TWO_MB];
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


// Membuka file yang dibutuhkan untuk leaderboard dan history. Prosedur ini harus dieksekusi saat dibuka.
void OpenLeaderboard(Leaderboard *l);
// Menutup file yang dibutuhkan untuk leaderboard dan history. Prosedur ini harus diekskusi setelah program selesai dijalankan.
void CloseLeaderboard(Leaderboard *l);
// Merefresh buffer leaderboard jika history baru sudah ditulis ke dalam file.
void RefreshLeaderboardBuffer(Leaderboard *l);
// Menulis history ke file history.
void WriteHistory(Leaderboard *l, History *h);


void ParseHistory(History *h, int len, int page);
int HistoryToBuffer(char *buffer, History h);
int BufferToHistory(char *buffer, History *h);

void TestHistory();

#endif // _LEADERBOARD_H_