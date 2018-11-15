#include "j1App.h"
#include "Entity.h"
#include "j1Entities.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Player.h"
#include "j1Audio.h"

Entity::Entity(int x, int y, ENTITY_TYPES type)
{
	position.x = x;
	position.y = y;
	this->type = type;

}

Entity::~Entity()
{
	if (col != nullptr)
		col->to_delete = true;
}

const Collider* Entity::GetCollider()const
{
	return col;
}

void Entity::DebugDraw(SDL_Texture* spritesheet)
{
	if (currentAnimation != nullptr)
		App->render->Blit(spritesheet, position.x, position.y, &(currentAnimation->GetCurrentFrame()));
}