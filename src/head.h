#ifndef HEAD_H
#define HEAD_H

#include "defs.h"
#include "gamestate.h"

typedef struct Head Head;

/* Head Initializer/Constructor */
void  Head_init(Head *head);
Head *Head_new();

/* Head Destructor */
void  Head_free(Head *head);

/* Head Methods */
void Head_update(Head *head, GameState *game_state);
void Head_input( Head *head, GameState *game_state);
void Head_render(Head *head, GameState *game_state);

#endif /* HEAD_H */
