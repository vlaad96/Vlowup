#include "FlyingEnemy.h"


FlyingEnemy::FlyingEnemy(int x, int y, ENTITY_TYPES type) : Entity(x,y, type)
{
	type = ENTITY_TYPES::FLYING_ENEMY;

	
}