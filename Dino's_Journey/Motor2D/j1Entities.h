#ifndef __J1ENTITIES_H__
#define __J1ENTITIES_H__

#include "j1Module.h"
#include "j1Collisions.h"
#include "p2List.h"

#define MAX_ENTITIES 100

class Entity;
class Player;
struct SDL_Texture;

enum ENTITY_TYPES
{
	NO_TYPE,
	PLAYER,
	FLYING_ENEMY,
	GROUND_ENEMY

};

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
	int x, y;
};

class j1Entities : public j1Module
{

public:

	j1Entities();
	~j1Entities();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool Save(pugi::xml_node& config)const;
	bool Load(pugi::xml_node& config);

	void OnCollision(Collider* col1, Collider* col2);
	Entity* AddEntity(ENTITY_TYPES type, int x, int y);

public:

	Entity *		entities[MAX_ENTITIES];
	Player*			player = nullptr;

private:

	EntityInfo		queue[MAX_ENTITIES];
};

#endif // !__J1ENTITIES_H__

