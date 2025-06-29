#ifndef ACTION_H
#define ACTION_H


#include "actor.h"


typedef struct Action Action;

/* Constructor */
void Action_new(void (* action)(Actor *actor, GameState *game_state));
void Action_init(Action *action, void (*func)(Actor *actor, GameState *game_state));
/* Destructor */
void Action_free(Action *action);

/* Methods */
Action *Action_execute(Action *action, Actor *actor, GameState *game_state);

Action *Action_insert(Action *action, Action *to);
Action *Action_remove(Action *action);


#endif /* ACTION_H */
