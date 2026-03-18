#include <stdlib.h>

#include "command.h"
#include "defs.h"


/**************************
	D E F I N I T I O N
**************************/
typedef struct
Command
{
	Command *next;
	union {
		Vector2      vec2;
		float        f[2];
		int          i[2];
		unsigned int u[2];
	};
	Action action;
}
Command;


/****************************
	C O N S T R U C T O R
****************************/
void
Command_init(Command *command, Action action)
{
	command->next   = command;
	command->action = action;
} /* Command_init */


Command *
Command_new(Action action)
{
	Command *command = malloc(sizeof(Command));
	if (!command) {
		ERR_OUT("Failed to allocate memory for Command.");
		return NULL;
	}

	Command_init(command, action);

	return command;
} /* Command_new */


/**************************
	D E S T R U C T O R
**************************/
void
Command_free(Command *self)
{
	Command *next = self->next;
	free(self);
	return next;
} /* Command_free */


/********************
	M E T H O D S
********************/
Command *
Command_execute(
	Command    *self,
	Actor     *actor,
	GameState *game_state
)
{
	self->action(actor, game_state);
	return Command_remove(self);
} /* Command_execute */


Command *
Command_insert(Command *self, Command *to)
{
	self->next = to;
	return self;
} /* Command_insert */


Command *
Command_remove(Command *self)
{
	return self->next;
} /* Command_remove */
