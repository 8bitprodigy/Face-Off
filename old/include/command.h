#ifndef COMMAND_H
#define COMMAND_H


#include "actor.h"


typedef struct Command Command;
typedef void (*Action)(Actor *, GameState *);

/* Constructor */
Command *Command_new(    Action   action);
void     Command_init(   Command *command, Action   action);
/* Destructor */
void     Command_free(   Command *command);

/* Methods */
Command *Command_execute(Command *command, Actor   *actor,  GameState *game_state);

Command *Command_insert( Command *command, Command *to);
Command *Command_remove( Command *command);


#endif /* COMMAND_H */
