#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1Entities.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "Animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_timer.h"
#include "SDL_mixer\include\SDL_mixer.h"


struct SDL_Texture;
struct Collider;

class Entity : public j1Entities
{
public:

	Entity(ENTITY_TYPES type);
	virtual ~Entity();

public:

	ENTITY_TYPES type;
};


#endif // !__ENTITY_H__

