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

/* List Operations */
void Head_add(   Head *head1, Head *head2); 
void Head_remove(Head *head); /* pun intended */

/* Head Methods */
void  Head_update(    Head *head, GameState *game_state);
void  Head_input(     Head *head, GameState *game_state);
void  Head_render(    Head *head, GameState *game_state);

/* Setters */
void  Head_setContext(Head *head, RenderTexture context);
void  Head_setFOV(    Head *head, float   fov);
void  Head_setPlayer( Head *head, Player *player);

/* Getters */
RenderTexture Head_getContext(Head *head);
float         Head_getFOV(    Head *head);
float         Head_getHalfFOV(Head *head);


#endif /* HEAD_H */
