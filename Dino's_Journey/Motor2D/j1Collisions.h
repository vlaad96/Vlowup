#ifndef _j1COLLISIONS_H_
#define _j1COLLISIONS_H_

#define MAX_COLLIDERS 1000

#include"j1Module.h"
#include "p2Log.h"

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_DEAD,
	COLLIDER_PLAYER,
	COLLIDER_MAX,
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type) :
		rect(rectangle),
		type(type)
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
	bool WillCollideLeft(const SDL_Rect& r, int distance) const;
	bool WillCollideRight(const SDL_Rect& r, int distance) const;
	bool WillCollideGround(const SDL_Rect& r, int distance) const;
	bool WillCollideTop(const SDL_Rect& r, int distance) const;
	

};

class j1Collisions : public j1Module
{
public:

	j1Collisions();
	~j1Collisions();

	bool Update(float dt);
	bool PreUpdate();
	bool CleanUp();
	
	bool CheckCollisionAfterSlide(const SDL_Rect& r, int dist) const;

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type);
	
	void DebugDraw();
	
	void MapTilesToColliders(pugi::xml_node &node, const SDL_Rect r);

private:

	Collider * colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // !_j1COLLISIONS_H

