#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1Scene.h"

j1Player::j1Player()
{
	name.create("player");
	position.x = 0;
	position.y = 0;

	//When you die

	death.PushBack({ 3,104,556,358 });
	death.PushBack({ 560,104,556,358 });
	death.PushBack({ 1170,104,556,358 });
	death.PushBack({ 1697,104,556,358 });
	death.PushBack({ 2231,104,556,358 });
	death.PushBack({ 2791,104,556,358 });
	death.PushBack({ 3354,104,556,358 });
	death.PushBack({ 3913,104,556,358 });

	//death.loop = false;
	//death.speed = 0.8;

	//idle animation
	idle.PushBack({ 0,495,502,396 });
	idle.PushBack({ 502,495,502,396 });
	idle.PushBack({ 1004,495,502,396 });
	idle.PushBack({ 1508,495,502,396 });
	idle.PushBack({ 2012,495,502,396 });
	idle.PushBack({ 2516,495,502,396 });
	idle.PushBack({ 3020,495,502,396 });
	idle.PushBack({ 3524,495,502,396 });
	idle.PushBack({ 4028,495,502,396 });
	idle.PushBack({ 4532,495,502,396 });   




	//running animation

	//jumping animation
	
	//FROM JUMPY SPRITE 
	//jump.PushBack({ 0,0,476,440 });			//Standing
	//jump.PushBack({ 440,0,476,440 });			//Preprejump
	//jump.PushBack({ 880,0,476,440 });			//Prejump
	//jump.PushBack({ 1320,0,476,440 });		//Elevating
	//jump.PushBack({ 1760,0,476,440 });		//Floating
	//jump.PushBack({ 2200,0,476,440 });		//
	//jump.PushBack({ 2640,0,476,440 });		//
	//jump.PushBack({ 3080,0,476,440 });		//
	//jump.PushBack({ 3520,0,476,440 });        //
	//jump.PushBack({ 3960,0,476,440 });		//Top
	//jump.PushBack({ 4400,0,476,440 });		//Droping
	//jump.PushBack({ 4840,0,476,440 });		//Touching floor

	current_animation = &idle; //should be &jump?
}

j1Player::~j1Player()
{

	App->tex->UnLoad(graphics);

}

bool j1Player::Awake(pugi::xml_node& conf)
{
	LOG("Initializing Player config");
	pugi::xml_node player = conf.child("player");

	return true;
}

bool j1Player::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("textures/SpriteSheet.png");

	SDL_Rect ground{};
	SDL_Rect collider_player_rect{};

	col = App->collision->AddCollider(collider_player_rect, COLLIDER_PLAYER);

	return ret;
}

bool j1Player::Update()
{
	//Blit
	App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame());

	//Camera to follow the player
	App->render->camera.x = -position.x + 400;
	App->render->camera.y = -position.y + 400;

	return true;
}

//Save player stats
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node player = data.append_child("player_position");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;
	player.append_attribute("map") = App->map->map;

	return true;
}

//Load player stats
bool j1Player::Load(pugi::xml_node& data)
{
	if (App->map->map == data.child("player_position").attribute("map").as_int())
	{
		position.x = data.child("player_position").attribute("x").as_float();
		position.y = data.child("player_position").attribute("y").as_float();
	}
	else
	{
		if (App->map->map == 0)
		{
			App->map->map = 1;
			App->map->CleanUp();
			App->map->Load("Level 2 final.tmx");
			position.x = data.child("player_position").attribute("x").as_float();
			position.y = data.child("player_position").attribute("y").as_float();
		}
		else
		{
			App->map->map = 0;
			App->map->CleanUp();
			App->map->Load("Level 1 final.tmx");
			position.x = data.child("player_position").attribute("x").as_float();
			position.y = data.child("player_position").attribute("y").as_float();
		}
	}

	return true;
}
