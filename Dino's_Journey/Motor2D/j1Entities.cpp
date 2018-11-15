#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Entities.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "Entity.h"

#include "Player.h"

j1Entities::j1Entities()
{
	name.create("entity");

	for (uint i = 0; i < MAX_ENTITIES; ++i)
	{
		entities[i] = nullptr;
	}
}

j1Entities::~j1Entities() 
{

}

bool j1Entities::Awake(pugi::xml_node& config)
{

	return true;
}

bool j1Entities::Start()
{

	player = new Player(PLAYER);
	player->Start();

	return true;
}

bool j1Entities::PreUpdate()
{

	return true;
}

bool j1Entities::Update(float dt)
{

	return true;
}

bool j1Entities::PostUpdate()
{

	return true;
}

bool j1Entities::CleanUp()
{

	return true;
}

bool j1Entities::Save(pugi::xml_node& config)const
{

	return true;
}

bool j1Entities::Load(pugi::xml_node& config)
{

	return true;
}