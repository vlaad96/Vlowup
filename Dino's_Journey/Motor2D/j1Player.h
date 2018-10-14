#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include"j1Collisions.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

enum player_state
{
	IDLE = 0,
	LEFT,
	RIGHT,
	JUMPING,
	DEAD
};

class j1Player : public j1Module
{
public:

	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	//Save and Load
	bool Save(pugi::xml_node& data)const;
	bool Load(pugi::xml_node& data);

	//Special movements
	void Jump();
	void Slide();

	// Loading animation function
	Animation* LoadAnim(const char* path, const char* name);

	//Collisions
	void OnCollision(Collider* c1, Collider* c2);

public:

	Collider * col;
	SDL_Texture* graphics = nullptr;

	bool jumping = false;
	bool sliding = false;
	bool dead = false;
	bool won = false;
	bool godmode = false;
	bool isColliding = false;

	iPoint touching;
	fPoint speed;
	fPoint position;
	fPoint speed_modifier;
	float gravity;
	int jumping_time;
	int sliding_time;
	player_state playerstate;
	int frames;
	int time;
	bool allow_time;

	Animation* current_animation = nullptr;
	Animation* idle_right;
	Animation* idle_left;
	Animation* run_right;
	Animation* run_left;
	Animation* jump;
	Animation* death;
	Animation* slide_left;
	Animation* slide_right;

	SDL_Texture* sprites = nullptr;

	SDL_Rect player_collider;

private:
	p2SString file_path;
	p2SString animation;
};

#endif //_j1PLAYER_H_
