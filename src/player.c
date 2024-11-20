#include "player.h"

void UpdateTurn(Player *p){
   if(p->turn==FIRST_TURN){
        p->turn = SECOND_TURN;
    }else{
        p->turn = FIRST_TURN;
    }
}