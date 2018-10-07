#ifndef _j1PLAYER_H_
#define _j1PLAYER_H_

#include"j1Module.h"
#include "Animation.h"
#include "j1Collisions.h"
#include "p2Point.h"

struct SDL_Texture;

class j1Player : public j1Module {

	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node& config);

	bool Start();
	bool PostUpdate();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

public:

	SDL_Texture * graphics = nullptr;
	Animation* current_animation;
	Animation idle;
	Animation run;
	Animation jump;
	Animation fall;
	Animation death;
	Animation slide;


};
#endif // !_j1PLAYER_H_

