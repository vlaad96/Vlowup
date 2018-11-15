#ifndef __ENEMY_H__
#define __ENEMY_H__


#include "p2Point.h"
#include "p2DynArray.h"
#include "Animation.h"


struct SDL_Texture;
struct Collider;

enum ENTITY_TYPES;

class Entity : public j1Entities
{

public:

	fPoint position;
	fPoint speedMultiplier;
	fPoint speed;
	Collider* col = nullptr;

	Animation* currentAnimation = nullptr;
	bool flip = false;//Used to flipping animations during blit

public:

	Entity(int x, int y, ENTITY_TYPES type);
	virtual ~Entity();

	const Collider* GetCollider()const;

	void DebugDraw(SDL_Texture* spritesheet);



public:

	ENTITY_TYPES type;
};


#endif // !__ENEMY_H__

