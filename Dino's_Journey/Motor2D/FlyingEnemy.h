#pragma once
#include "Entity.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Pathfinding.h"
#include "j1Entities.h"
#include "Animation.h"
#include "Player.h"

class FlyingEnemy : public Entity
{
public:

	FlyingEnemy(int x, int y);
	~FlyingEnemy();

	Animation* idle;


};