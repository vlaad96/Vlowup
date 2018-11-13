#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#include "j1Module.h"
#include"SDL/include/SDL_rect.h"
#include"p2List.h"

enum COLLIDER_TYPE {

	COLLIDER_NONE = -1,
	COLLIDER_WALLS,
	COLLIDER_PLAYER,
	COLLIDER_SPIKES,
	COLLIDER_ACID,
	WIN_COLLIDER,
	MAX_COLLIDER

};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collisions :public j1Module
{
public:

	j1Collisions();
	~j1Collisions();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void DebugDraw();

	p2List<Collider*> colliders;
	

private:

	Collider * collider[MAX_COLLIDER];
	bool matrix[MAX_COLLIDER][MAX_COLLIDER];
	bool debug = false;
};

#endif // !__j1COLLISIONS_H__