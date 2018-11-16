#include "Entity.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Pathfinding.h"
#include "j1Entities.h"
#include "Animation.h"
#include "Player.h"

class GroundEnemy :public Entity
{
public:

	GroundEnemy(int x, int y);
	~GroundEnemy();

	Animation* idle;
};
