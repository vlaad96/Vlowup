#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include "j1Module.h"
#include "Animation.h"
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
	bool Update();

	//Save and Load funcions
	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&)const;

	//Other functions and variables
public:

	fPoint position;
	SDL_Texture* graphics = nullptr;
	

	//Texture relateed
public:

	Animation* current_animation = nullptr;
	SDL_Texture* sprites = nullptr;

	SDL_Rect player_collider;
	
	Animation idleR;
	Animation idleL;
	Animation runR;
	Animation runL;
	Animation jumpR;
	Animation jumpL;
	Animation slideR;
	Animation slideL;
	Animation dying;
	
	Animation* LoadAnimation(const char* path, const char* name);

	//Animation path and folder
private:
	p2SString file_path;
	p2SString Textures;

};
#endif //_j1PLAYER_H_
