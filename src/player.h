#ifndef PLAYER_H
#define PLAYER_H

#define FIRST_TURN 1
#define SECOND_TURN 2

typedef struct {
    char name[20];
    int turn;
} Player;

void UpdateTurn(Player *p);

#endif // !PLAYER_H