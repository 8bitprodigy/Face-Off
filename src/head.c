#include <stdlib.h>

#include "head.h"
#include "player.h"


typedef struct
Head
{
	struct Head   *next;
	struct Head   *prev;
	Player        *player;
    Camera         camera;
	RenderTexture  context;
	Vector2        context_pos;
    float          fov;
    float          Whalf_fov;
	int            controller;
}
Head;

