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

	int row = 0;
	sprite_dist.x = 100;
	sprite_dist.y = 80;

	//When you die
	for (int i = 0; i < 8; i++)
	{
		death.PushBack({ sprite_dist.x*i, sprite_dist.y*row, 99,73 });

		death.loop = false;
		death.speed = 0.8f;
		row++;
	}

	//idle animation
	
	for (int i = 0; i < 10; i++)
	{
		idle.PushBack({ sprite_dist.x*i, sprite_dist.y*row, 99,73 });

		idle.loop = true;
		idle.speed = 0.8f;
		row++;
	}

	//running animation

	for (int i = 0; i < 8; i++)
	{
		run.PushBack({ sprite_dist.x*i, sprite_dist.y*row, 99,73 });

		run.loop = true;
		row++;
	}

	//jumping animation

	for (int i = 0; i < 7; i++)
	{
		jump.PushBack({ sprite_dist.x*i, sprite_dist.y*row, 99,73 });

		jump.loop = false;
		row++;
	}
	
	for (int i = 0; i < 3; i++)
	{
		slide.PushBack({ sprite_dist.x*i, sprite_dist.y*row, 99,73 });

		slide.loop = false;
		row++;
	}
	
}

j1Player::~j1Player()
{

	App->tex->UnLoad(graphics);

}

bool j1Player::Awake(pugi::xml_node& conf)
{
	LOG("Initializing Player config");
	pugi::xml_node player = conf.child("player");

	speed_modifier.x = conf.child("speed_modifier.x").attribute("value").as_float();
	speed_modifier.y = conf.child("speed_modifier.y").attribute("value").as_float();

	return true;
}

bool j1Player::Start()
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("textures/SpriteSheet.png");

	if (graphics == nullptr) {
		LOG("Error loading player textures %s", SDL_GetError);
		ret = false;
	}

	SDL_Rect player_dimensions{ 0,0,99,73 };

	SDL_Rect collider_rect{ 0,0,player_dimensions.w, player_dimensions.h };

	touching.x = 0;
	touching.y = 0;

	col = App->collision->AddCollider(collider_rect, COLLIDER_PLAYER);


	return ret;
}

bool j1Player::Update()
{
	if (touching.x != 0)
	{
		speed.y = speed_modifier.y;
	}
	else
		speed.y = speed_modifier.y * 2;


	speed.x = 0;

	if (dead)
	{
		current_animation = &death;

		if (current_animation->Finished())
		{
			death.Reset();
			dead = false;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{

	}
	

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_REPEAT) 
	{
		if (!godmode)
		{
			godmode = true;
		}
		else
			godmode = false;
	}

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

void j1Player::Jump()
{


}