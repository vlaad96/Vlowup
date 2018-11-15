#ifndef __J1ENTITIES_H__
#define __J1ENTITIES_H__

#include "j1Module.h"

#define MAX_ENTITIES 100

class Entity;
class Player;
struct SDL_Texture;

enum ENTITY_TYPES
{
	NO_TYPE,
	PLAYER

};

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
};

class j1Entities : public j1Module
{

public:

	j1Entities();
	~j1Entities();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool Save(pugi::xml_node& config)const;
	bool Load(pugi::xml_node& config);

public:

	Entity *		entities[MAX_ENTITIES];
	Player*			player = nullptr;

private:

	EntityInfo		queue[MAX_ENTITIES];
};

#endif // !__J1ENTITIES_H__

