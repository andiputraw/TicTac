#include "leaderboard.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"

void __PrintBuffer(char *buffer, int buffer_len){
    for(int i = 0; i < buffer_len; i++){
        printf("%d ", buffer[i]);
    }
    printf("\n");
}

void __PrintHistory(History h){
    printf("\n--HISTORY--\n");
    printf("GAME MODE: %d\n", h.game_mode);

    printf("P1 Name : %s\n", h.p1.name);
    printf("P1 Score:%d\n", h.p1.score);

    printf("P2 Name : %s\n", h.p2.name);
    printf("P2 Score:%d\n", h.p2.score);

    printf("Board State\n");
    for(int i = 0; i < 25; i++){
        printf("%d ", h.BoardState[i]);
    }
}

int HistoryToBuffer(char *buffer, History h){
    int i, p1_len, p2_len = 0;
    int j = 0;
    i = 0;

    // Taruh game mode, skor p1 dan p2 ke buffer
    buffer[i++] = h.game_mode;
    buffer[i++] = (char)h.p1.score;
    buffer[i++] = (char)h.p2.score;

    // cari panjang nama p1 dan taruh di buffer
    p1_len = strlen(h.p1.name);
    buffer[i++] = (char) p1_len;
    // loop sebanyak panjang nama p1, dan taruh setiap karakter ke buffer
    for(j = 0; j < p1_len; j++){
        buffer[i++] = h.p1.name[j];
    }

    // cari panjang nama p2 dan taruh di buffer
    p2_len = strlen(h.p2.name);
    buffer[i++] = (char) p2_len;

    // loop sebanyak panjang nama p2, dan taruh setiap karakter ke buffer
    for(j = 0; j < p2_len; j++){
        buffer[i++] = h.p2.name[j];
    }
    // taruh state board state ke buffer
    for(j = 0; j < 25; j++){
        buffer[i++] = (unsigned char)h.BoardState[j];
    }
    // __PrintBuffer(buffer, i);
    return i;
}

int BufferToHistory(char *buffer, History *h){
    int i = 0, p1_len = 0, p2_len = 0;
    int j = 0;
    // parsing buffer dan taruh ke game_mode dan skor masing masing
    h->game_mode = buffer[i++];
    h->p1.score = (int)buffer[i++];
    h->p2.score = (int)buffer[i++];

    // ambil panjang yang ada di buffer
    p1_len = (int)buffer[i++];

    // loop sebanyak panjang yang ada di buffer, lalu copy setiap karakter ke p1.name
    for(j = 0; j < p1_len; j++){
        h->p1.name[j] = buffer[i++];
    }
    // null terminator
    h->p1.name[j] = '\0';

    // ambil panjang nama p2 yang ada di buffer
    p2_len = (int)buffer[i++];
    // loop sebanyak panjang yang ada di buffer, lalu copy setiap karakter ke p1.name
    for(j = 0; j < p2_len; j++){
        h->p2.name[j] = buffer[i++];
    }
    // null terminator
    h->p2.name[j] = '\0';

    // copy board state (sisanya) di buffer
    for(j = 0; j < 25; j++){
        h->BoardState[j] = buffer[i++];
    }

    return i;
}

void OpenLeaderboard(Leaderboard *l){
    l->history = fopen(HISTORY_PATH, "a+");
    if(l->history == NULL){
        perror("Failed opening file: ");
    }

    fgets(l->history_buffer, TWO_MB, l->history);

    l->leaderboard = fopen(LEADERBOARD_PATH, "a+");
    if(l->leaderboard == NULL){
        perror("Failed opening file: ");
    }
    fgets(l->leaderboard_buffer, TWO_MB, l->leaderboard);
}

void RefreshLeaderboardBuffer(Leaderboard *l){
    if (fseek(l->history, 0 ,SEEK_SET) != 0){
        perror("Failed seeking to start of file");
        return;
    }
    fgets(l->history_buffer, TWO_MB, l->history);

    if (fseek(l->leaderboard, 0 ,SEEK_SET) != 0){
        perror("Failed seeking to start of file");
        return;
    }
    fgets(l->leaderboard_buffer, TWO_MB, l->leaderboard);
}

void CloseLeaderboard(Leaderboard *l) {
    fclose(l->history);
    fclose(l->leaderboard);
}

void WriteHistory(Leaderboard *l, History *h){
    char alloc[1024];
    int count = 0;
    if (fseek(l->history, 0, SEEK_END) != 0){
        perror("Failed seeking to end of file");
        return;
    }

    count = HistoryToBuffer(&alloc, *h);
    fwrite(alloc, sizeof(char), count, l->history);
}

void TestHistory(){
    printf("TEST HISTORY SERIALIZING\n");
    // TEST LEADERBOARD
    int boxes[25] = {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    Player p1 = (Player){
        .name= "andiputraw",
        .score = 5,
    };

    Player p2 = (Player){
        .name = "maulana",
        .score = 3,
    };

    History h = (History){
        .p1 = p1,
        .p2 = p2,
        .game_mode = BOARD_3_X_3
    };
    memcpy(h.BoardState, boxes, sizeof(int) * 25);

    char buffer[255];
    int len = HistoryToBuffer(buffer, h);
    __PrintBuffer(buffer, len);
    History parsed_h;
    BufferToHistory(buffer,&parsed_h);
    __PrintHistory(parsed_h);

    printf("\nTEST FILE LEADERBOARD\n");
    Leaderboard l;
    OpenLeaderboard(&l);
    WriteHistory(&l, &h);
    CloseLeaderboard(&l);
    printf("TEST HISTORY SERIALIZING COMPLETED\n");
}