#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

#include "j1Module.h"
#include"SDL/include/SDL_rect.h"
#include"p2List.h"

#define MAX_COLLIDERS 1000

enum COLLIDER_TYPE {

	COLLIDER_NONE = -1,
	COLLIDER_WALLS,
	COLLIDER_PLAYER,
	COLLIDER_SPIKES,
	COLLIDER_ACID,
	COLLIDER_FLAG,
	COLLIDER_MAX

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

	void SetSize(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}

	bool CheckCollision(const SDL_Rect& r) const;
	bool CheckLeftCollider(const SDL_Rect& r, int dist)const;
	bool CheckRightCollider(const SDL_Rect& r, int dist)const;
	bool CheckBotCollider(const SDL_Rect& r, int dist)const;
	bool CheckTopCollider(const SDL_Rect& r, int dist)const;
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


private:

	Collider * colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // !__j1COLLISIONS_H__