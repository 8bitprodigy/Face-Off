#include <stdlib.h>

#include "action.h"
#include "defs.h"


/**************************
	D E F I N I T I O N
**************************/
typedef struct
Action
{
	Action *next;
	union {
		Vector2      vec2;
		float        f[2];
		int          i[2];
		unsigned int u[2];
	};
	void (*action)(Actor *actor, GameState *game_state);
}
Action;


/****************************
	C O N S T R U C T O R
****************************/
void
Action_init(
	Action *action,
	void (* func)(Actor *actor, GameState *game_state)
)
{
	action->next   = action;
	action->action = func;
} /* Action_init */


void
Action_new(
	void (* func)(Actor *actor, GameState *game_state)
)
{
	Action *action = malloc(sizeof(Action));
	if (!action) {
		ERR_OUT("Failed to allocate memory for Action.");
		return NULL;
	}

	Action_init(action, func);

	return action;
} /* Action_new */


/**************************
	D E S T R U C T O R
**************************/
void
Action_free(Action *self)
{
	Action *next = self->next;
	free(self);
	return next;
} /* Action_free */


/********************
	M E T H O D S
********************/
Action *
Action_execute(
	Action    *self,
	Actor     *actor,
	GameState *game_state
)
{
	self->action(actor, game_state);
	return Action_remove(self);
} /* Action_execute */


Action *
Action_insert(Action *self, Action *to)
{
	self->next = to;
	return self;
} /* Action_insert */


Action *
Action_remove(Action *self)
{
	return self->next;
} /* Action_remove */
