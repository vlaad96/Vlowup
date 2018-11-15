#include "j1App.h"
#include "Entity.h"
#include "j1Entities.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Player.h"
#include "j1Audio.h"

Entity::Entity(ENTITY_TYPES type) : type(type)
{

}

Entity::~Entity()
{
	
}