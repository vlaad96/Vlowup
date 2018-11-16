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

	FlyingEnemy(int x, int y, ENTITY_TYPES type);
	~FlyingEnemy();

	bool Awake(pugi::xml_node& config);
	bool Update(float dt);

	bool Save(pugi::xml_node& config)const;
	bool Load(pugi::xml_node& config);

public:

	Animation* idle;

private:

	float movSpeed = 0;

};