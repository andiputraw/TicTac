#include "leaderboard.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include <time.h>

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


void __PrintPlayerElo(PlayerElo e) {
    printf("\n--PLAYER ELO--\n");
    printf("PLAYER NAME: %s\n", e.name);
    printf("PLAYER ELO: %d\n", e.elo);
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
    l->history_buffer = malloc(TWO_MB);
    l->leaderboard_buffer = malloc(TWO_MB);
    memset(l->history_buffer, 0, TWO_MB);
    memset(l->leaderboard_buffer, 0, TWO_MB); 
    l->history = fopen(HISTORY_PATH, "rb+");

    if(l->history == NULL){
        l->history = fopen(HISTORY_PATH, "wb+");
        if(l->history == NULL){
           perror("Failed opening file: ");
           return;
        }
    }

    fgets(l->history_buffer, TWO_MB, l->history);

    l->leaderboard = fopen(LEADERBOARD_PATH, "rb+");
    if(l->leaderboard == NULL){
        l->leaderboard = fopen(LEADERBOARD_PATH, "wb+");
        if(l->leaderboard == NULL){
           perror("Failed opening file: ");
           return;
        }
    }
    fgets(l->leaderboard_buffer, TWO_MB, l->leaderboard);
}

void RefreshLeaderboardBuffer(Leaderboard *l){
    fflush(l->history);
    fflush(l->leaderboard);
    if (fseek(l->history, 0 ,SEEK_SET) != 0){
        perror("Failed seeking to start of file");
        return;
    }
    memset(l->history_buffer, 0, TWO_MB);
    fread(l->history_buffer, 1, TWO_MB, l->history);

    if (fseek(l->leaderboard, 0 ,SEEK_SET) != 0){
        perror("Failed seeking to start of file");
        return;
    }
    memset(l->leaderboard_buffer, 0, TWO_MB);
    fread(l->leaderboard_buffer, 1, TWO_MB, l->leaderboard);
}

void CloseLeaderboard(Leaderboard *l) {
    fclose(l->history);
    fclose(l->leaderboard);
    free(l->history_buffer);
    free(l->leaderboard_buffer);
}

void WriteHistory(Leaderboard *l, History *h){
    char alloc[1024] = {0};
    int count = 0;
    int historyAmount = 0;
    // Baca byte pertama dari file
    if(fseek(l->history, 0, SEEK_SET) != 0 ){
        perror("Failed seeking to first of file");
        return;
    }
    // Tambahkan 1 jika ada isinya, kalau tidak. isi dnegan 1
    fread(alloc, sizeof(char), 1, l->history);
    if(alloc[0] == 0){
        historyAmount = 1;
    }else{
        historyAmount = alloc[0] + 1;
    }

    if(fseek(l->history, 0, SEEK_SET) != 0 ){
        perror("Failed seeking to first of file");
        return;
    }
    
    alloc[0] = (char)historyAmount;
    long history = ftell(l->history);
    fwrite(alloc, sizeof(char), 1, l->history);
    fflush(l->history);

    if (fseek(l->history, 0, SEEK_END) != 0){
        perror("Failed seeking to end of file");
        return;
    }

    count = HistoryToBuffer(&alloc, *h);
    fwrite(&count, sizeof(char), 1, l->history);
    fwrite(alloc, sizeof(char), count, l->history);
    fflush(l->history);
}

bool ReadHistory(Leaderboard *l, History *h, int len, int offset){
    int i = 0, j = 0, totalRead = 0;
    int currOffset = 0;
    int perHistoryOffset = 0;
    int totalHistory = GetHistoryLen(l);
    currOffset++;
    if(len + offset > totalHistory){
        printf("ERROR: len + offset is more that total history");
        return false;
    }
    // pindahkan currOffset ke panjang offset 
    for(i = 0; i < offset; i++){
        perHistoryOffset = l->history_buffer[currOffset++];
        currOffset += perHistoryOffset;
    }

    // populate history sepanjang len
    for(i = 0; i < len; i++){
        // skip panjang buffer history
        currOffset++;
        totalRead = BufferToHistory(&l->history_buffer[currOffset], &h[i]);
        currOffset += totalRead;
    }
    return true;
}

int GetHistoryLen(Leaderboard *l){
    
    return (unsigned char)l->history_buffer[0];
}
// mengembalikan lokasi cursor disaat nama yang sama muncul. cursor menunjuk ke panjang nama
// kembalikan -1 saaat tidak ketemu
int __PlayerLocationCursor(Leaderboard *l, char *name, int len){
    RefreshLeaderboardBuffer(l);
    int cursor = 0;
    int buffLen = 0;
    int stringLen = 0;
    for(cursor = 0; l->leaderboard_buffer[cursor] != 0; cursor += buffLen + 1 ){
        // ambil panjang buffer
        buffLen = l->leaderboard_buffer[cursor];
        // ambil panjang nama
        stringLen = l->leaderboard_buffer[cursor + 1];
        if(len != stringLen){
            continue;
        }   

        if (memcmp(&l->leaderboard_buffer[cursor + 2], name, len) == 0){
            return cursor + 1;
        }
    }
    return -1;
}

void WritePlayerElo(Leaderboard *l, PlayerElo *p) {
    char buff[1024];
    int buffLen;
    int playerCursorLoc;
    buffLen = PlayerEloToBuffer(buff, *p);
    // player cursor loc mengembalikan lokasi dimana cursor menunjukan ke buffer yang menandakan panjang dari string
    playerCursorLoc = __PlayerLocationCursor(l, p->name, strlen(p->name));

    // Tulis diujung file jike nama player tidak ketemu
    if(playerCursorLoc == -1){
        if (fseek(l->leaderboard, 0, SEEK_END) != 0){
            perror("ERROR: failed to move cursor to the end of file");
            return;
        }

        fwrite(&buffLen, sizeof(char), 1,l->leaderboard);
        fwrite(buff, sizeof(char), buffLen, l->leaderboard);
        fflush(l->leaderboard);
        return;
    }

    if (fseek(l->leaderboard, playerCursorLoc, SEEK_SET) != 0 ){
        perror("ERROR: failed to move cursor to the intended position");
        return;
    }
    fwrite(buff, sizeof(char), buffLen, l->leaderboard);
    fflush(l->leaderboard);
}

bool ReadTop5Player(Leaderboard *l, PlayerElo *p){
    int i = 0, j = 0,k = 0;

    int currOffset = 0;
    int currBufLen = 0;
    int pNameLen = 0;
    int currElo = 0;
    int top5Offset[5] = {0,0,0,0,0};
    int top5Score[5] = {0,0,0,0,0};
    int top5Len = 0;
    for(i = 0; l->leaderboard_buffer[currOffset] != 0; currOffset += currBufLen ) {
        currBufLen = l->leaderboard_buffer[currOffset] + 1;
        memcpy(&currElo, &l->leaderboard_buffer[currOffset + currBufLen - 4], sizeof(int));

        for(j = 0; j < 5; j++){
            if(currElo > top5Score[j]){
                for(k = 4; k > j; k--){
                    top5Score[k] = top5Score[k - 1];
                    top5Offset[k] = top5Offset[k - 1];
                }
                top5Score[j] = currElo;
                top5Offset[j] = currOffset + 1;
                break;
            }
        }
    }
    
    for( i = 0; i < 5 && top5Offset[i] != 0; i++) {
        BufferToPlayerElo(&l->leaderboard_buffer[top5Offset[i]], &p[i]);
    }

    return true;
}

int PlayerEloToBuffer(char *buffer, PlayerElo h){
    int i = 0, nameLen = 0;
    int j = 0;
    i = 0;

    nameLen = strlen(h.name);
    buffer[i++] = nameLen;

    for(j = 0; j < nameLen; j++){
        buffer[i++] = h.name[j];
    }

    memcpy(&buffer[i], &h.elo, sizeof(int));
    i += sizeof(int);
    return i;  
}
int BufferToPlayerElo(char *buffer, PlayerElo *h){
    int i = 0, nameLen = 0;
    int j = 0;
    i = 0;

    nameLen = buffer[i++];

    for( j = 0; j < nameLen; j++){
        h->name[j] =  buffer[i++];
    }
    h->name[j] = '\0';

    memcpy(&h->elo, &buffer[i], sizeof(int));
    i += sizeof(int);
    return i;
}

void TestHistory(){
    printf("TEST HISTORY SERIALIZING\n");
     srand(time(0));
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
    
    History historyFromFile[2]; 
    printf("\nTEST FILE LEADERBOARD\n");
    Leaderboard l;
    OpenLeaderboard(&l);
    for(int i = 0; i < 9; i++){
        sprintf(h.p1.name, "P1-%d", i);
        sprintf(h.p2.name, "Pe-%d", i);
        h.p1.score = rand() % 2 + 1;
        h.p2.score = rand() % 2 + 1;
        h.game_mode = rand() % 2 + 1;
        WriteHistory(&l, &h);
    }
    RefreshLeaderboardBuffer(&l);
    ReadHistory(&l, &historyFromFile[0], 2, 2);
    printf("\n HISTORY RESULT FROM FILE\n");

    __PrintHistory(historyFromFile[0]);
    __PrintHistory(historyFromFile[1]);
    
    printf("TEST HISTORY SERIALIZING COMPLETED\n");

    printf("\nTEST PLAYER ELO BUFFER\n");
    PlayerElo elo = {
        .name = "ANDIPUTRAW",
        .elo = 9999,
    };
    char buffer2[255];
    len = PlayerEloToBuffer(buffer2, elo);
    __PrintBuffer(buffer2, len);
    PlayerElo foo;
    BufferToPlayerElo(buffer2, &foo);
    __PrintPlayerElo(foo);
    
    printf("\nTEST PLAYER ELO WRITE TO FILE\n");

    WritePlayerElo(&l, &elo);
    elo.elo = 2341;
    
    
    WritePlayerElo(&l, &elo);
    RefreshLeaderboardBuffer(&l);
   
    // Check top 5
    for(int i = 0; i < 5; i++) {
        sprintf(elo.name, "Player %d", i );
        elo.elo = rand() % (1000 - 100 + 1);
        WritePlayerElo(&l, &elo);
    }

    PlayerElo playerElos[5];
    ReadTop5Player(&l, &playerElos[0]);
    for(int i =0; i < 5; i++){
        printf("\nTOP %d \n", i + 1);
        __PrintPlayerElo(playerElos[i]);
    }
    CloseLeaderboard(&l);
}