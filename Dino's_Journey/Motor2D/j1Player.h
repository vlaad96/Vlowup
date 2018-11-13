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
	bool CleanUp();

	void OnCollision(Collider* col1, Collider* col2);

	//Save and Load funcions
	bool Save(pugi::xml_node &config)const;
	bool Load(pugi::xml_node &config);

	//Other functions and variables
public:

	fPoint position;
	
	bool godMode = false;
	bool isDead = false;
	bool isLookingRight = false;

	float gmSpeed;
	float gravity;
	int initPosX;
	int initPosY;
	int jumpingTime;
	float speedMultiplierX;
	float speedMultiplierY;
	
	int collisionMargin;
	Collider* colPlayer;
	SDL_Rect player_collider;
	bool isTouchingGround = false;
	bool hasWallInFront = false;
	bool hasWallBehind = false;
	bool hasWallAbove = false;

	//Texture relateed
public:

	Animation* current_animation = nullptr;
	SDL_Texture* sprites = nullptr;

	Animation* idle = nullptr;
	Animation* run = nullptr;
	Animation* jump = nullptr;
	Animation* godmode = nullptr;

	Animation* dying = nullptr;
	
	Animation* LoadAnimation(const char* path, const char* name);

	//Animation path and folder
private:
	p2SString file_path;
	p2SString Textures;

};
#endif //_j1PLAYER_H_
