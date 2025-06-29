#include <stdbool.h>
#include <stdlib.h>

#include <raylib.h>

#include "defs.h"
#include "head.h"
#include "player.h"


typedef struct
Head
{
	struct Head   *prev;
	struct Head   *next;
	Player        *player;
    Camera         camera;
	RenderTexture  context;
	Vector2        context_pos;
    float          fov;
    float          half_fov;
	int            controller;
	bool           dirty;
}
Head;

/****************************
	C O N S T R U C T O R
****************************/
void 
Head_init(Head *self)
{
	self->next = self;
	self->prev = self;
}

Head *
Head_new()
{
	Head *head = malloc(sizeof(Head));
	if (!head) {
		ERR_OUT("Failed to allocate memory for Appstate.");
		return NULL;
	}

	Head_init(head);

	return head;
}

/**************************
	D E S T R U C T O R
**************************/
void
Head_free(Head *head)
{
	Head_remove(head);
	free(head);
}


/*************************************
	L I S T    O P E R A T I O N S
*************************************/
void
Head_add(Head *self, Head *head)
{
	if (!self || !head) return;
	
	Head *tail = head->prev;
	tail->next = self;
	head->prev = self;
	
	self->next = head;
	self->prev = tail;
}

void
Head_remove(Head *head)
{
	if (!head) return;

	Head
		*prev = head->prev,
		*next = head->next;

	prev->next = next;
	next->prev = prev;
}


/***************************************
	M E M B E R    F U N C T I O N S
***************************************/
void
Head_update(
	Head      *self,
	GameState *game_state
)
{
	if (self->player)
		self->camera.position = VECTOR2_TO_3(Thing_get_position(THING(self->player)), CAMERA_HEIGHT);
} /* Head_update */


void
Head_input(
	Head      *self,
	GameState *game_state
)
{
	
} /* Head_input */


void
Head_render(
	Head      *self,
	GameState *game_state
)
{
	BeginTextureMode(self->context);
		GameState_drawScene(game_state);
	EndTextureMode();
} /* Head_render */


void
Head_setFOV(Head *self, float fov)
{
	self->fov   = fov;
	self->dirty = true;
} /* Head_setFOV */


float
Head_getFOV(Head *self)
{
	return self->fov;
} /* Head_getFOV */


float
Head_getHalfFOV(Head *self)
{
	if (self->dirty) self->half_fov = self->fov / 2.0f;
	return self->half_fov;
} /* Head_getHalfFOV */


void
Head_setContext(Head *self, RenderTexture context)
{
	self->context = context;
} /* Head_setContext */


RenderTexture
Head_getContext(Head *self)
{
	return self->context;
} /* Head_getContext */
