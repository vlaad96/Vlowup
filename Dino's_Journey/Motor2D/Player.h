#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Entities.h"
#include "Entity.h"
#include "Animation.h"
#include "j1Collisions.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Scene.h"


class Player : public Entity
{
	//Main functions of player module
public:

	Player(int x, int y, ENTITY_TYPES type);
	~Player();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider* col1, Collider* col2);

	//Save and Load funcions
	bool Save(pugi::xml_node &config)const;
	bool Load(pugi::xml_node &config);

	//Movement functions
	void Jump(float dt);

	//Other functions and variables
public:

	fPoint position;
	fPoint speed;
	
	bool godMode = false;
	bool isDead = false;
	bool isLookingRight = false;

	bool isJumping = false;
	bool isFalling = false;
	int jumpPosition;


	float gmSpeed;
	float gravity;
	int initPosX;
	int initPosY;
	int jumpingTime;
	float speedMultiplierX;
	float speedMultiplierY;
	float displacemetX;
	
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

	uint jumpForce;
	uint jumpHeight;

};
#endif //_j1PLAYER_H_
