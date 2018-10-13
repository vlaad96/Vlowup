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
	bool Update(float dt);

	//Save and Load

	bool Save(pugi::xml_node& data)const;
	bool Load(pugi::xml_node& data);

	//Special movements

	void Jump();
	void Slide();

public:

	Collider * col;

	bool jumping = false;
	bool sliding = false;
	bool dead = false;
	bool won = false;
	bool godmode = false;

	fPoint speed;
	fPoint position;
	iPoint touching; //It says if the player is touching the ground or walls
	//x for left and right walls and y for ground and ceiling.
	iPoint sprite_dist;
	fPoint speed_modifier;

public:

	SDL_Texture * graphics = nullptr;
	Animation* current_animation;
	Animation idle;
	Animation run;
	Animation jump;
	Animation death;
	Animation slide;
};

#endif //_j1PLAYER_H_
