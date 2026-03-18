#include <stdlib.h>

#include "agent_private.h"
#include "defs.h"


/****************************
    C O N S T R U C T O R    
****************************/
void
Agent_init(
	Agent   *agent,
	Body     body,
	Vector2  position,
	float    rotation,
	float    radius
)
{
	Actor *actor = ACTOR(agent);

	Actor_init(actor, body, position, rotation, radius);
	actor->update = &Agent_update;
} /* Agent_init */

Agent *
Agent_new(
	Body    body,
	Vector2 position,
	float   rotation,
	float   radius
)
{
	Agent *agent = malloc(sizeof(Agent));

	if (!agent) {
        ERR_OUT("Failed to allocate memory for Actor.");
        return NULL;
	}

	Agent_init(agent, body, position, rotation, radius);

	return agent;
} /* Agent_new */


/**************************
    D E S T R U C T O R    
**************************/
void
Agent_free(Agent *agent)
{
	Actor_free(ACTOR(agent));
} /* Agent_free */


/********************************
    A G E N T   M E T H O D S    
********************************/
void
Agent_update(Actor *actor, GameState *game_state)
{
	Actor_update(actor, game_state);
} /* Agent_update */
