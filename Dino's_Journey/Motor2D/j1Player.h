#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include "Animation.h"
#include "j1Collisions.h"
#include "p2Point.h"


struct SDL_Texture;


class j1Player : public j1Module
{
	//Main functions of player module
public:

	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);

	//Save and Load funcions
	bool Save(pugi::xml_node &config)const;
	bool Load(pugi::xml_node &config);

	//Other functions and variables
public:

	fPoint position;
	fPoint speed;
	float gravity;
	bool godMode = false;
	float gmSpeed;

	Collider* colPlayer;
	

	//Texture relateed
public:

	Animation* current_animation = nullptr;
	SDL_Texture* sprites = nullptr;

	SDL_Rect player_collider;

	Animation* idleR = nullptr;
	Animation* idleL = nullptr;
	Animation* runR = nullptr;
	Animation* runL = nullptr;
	Animation* jumpR = nullptr;
	Animation* jumpL = nullptr;
	Animation* dying = nullptr;
	
	Animation* LoadAnimation(const char* path, const char* name);

	//Animation path and folder
private:
	p2SString file_path;
	p2SString Textures;

};
#endif //_j1PLAYER_H_
