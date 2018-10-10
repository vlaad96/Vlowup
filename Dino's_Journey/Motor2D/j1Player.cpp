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


	//running animation

	//jumping animation

	current_animation = &idle;
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
