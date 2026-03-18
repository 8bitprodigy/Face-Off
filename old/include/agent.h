#ifndef AGENT_H
#define AGENT_H

#include "actor.h"
#include "gamestate.h"


#define AGENT( self ) ((Agent

typedef struct Agent Agent;


/* Agent Constructor */
void   Agent_init(Agent *agent, Body    body,     Vector2 position, float rotation, float radius);
Agent *Agent_new( Body   body,  Vector2 position, float   rotation, float radius);
/* Agent Destructor */
void   Agent_free(Agent *agent);

/* Agent Methods */
void Agent_update( Actor *actor, GameState *game_state);


#endif /* AGENT_H */
