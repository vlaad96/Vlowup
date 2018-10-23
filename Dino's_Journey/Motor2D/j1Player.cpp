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
	position = { 0,0 };

}

j1Player::~j1Player()
{

	App->tex->UnLoad(graphics);
}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Initializing Player configuration");
	pugi::xml_node player = config.child("player");

	//Animations
	file_path.create(config.child("file path").child_value());
	animation.create(config.child("Animation").child_value());

	idleL = LoadAnimation(file_path.GetString(), "idle_left");
	idleR = LoadAnimation(file_path.GetString(), "idle_right");
	jump = LoadAnimation(file_path.GetString(), "jump");
	runL = LoadAnimation(file_path.GetString(), "run_left");
	runR = LoadAnimation(file_path.GetString(), "run_right");
	slideL = LoadAnimation(file_path.GetString(), "slide_left");
	slideR = LoadAnimation(file_path.GetString(), "slide_right");
	dying = LoadAnimation(file_path.GetString(), "death");

	return true;
}


bool j1Player::Start() 
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("textures/player.tmx");

	if (graphics == nullptr)
	{
		LOG("Error loading player textures %s", SDL_GetError);
		ret = false;
	}
	if (sprites == nullptr)
	{
		sprites = App->tex->Load(animation.GetString());
	}

	return ret;
}

bool j1Player::Update()
{
	current_animation = idleR;

	return true;
}