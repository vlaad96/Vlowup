#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include"j1Collisions.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:

	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool Update();

	//Save and Load

	bool Save(pugi::xml_node& data)const;
	bool Load(pugi::xml_node& data);

	//Special movements

	void Jump();

public:

	Collider * col;

	bool jumping = false;
	bool dead = false;
	bool won = false;

	fPoint speed;
	fPoint position;

public:

	SDL_Texture * graphics = nullptr;
	Animation* current_animation;
	Animation idle;
	Animation run;
	Animation jump;
	Animation death;
};

#endif //_j1PLAYER_H_
