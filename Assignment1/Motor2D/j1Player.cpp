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
	//Animations

	//Idle
	idle.PushBack({ 2,502,363,389 });
	idle.PushBack({ 501,502,363,389 });
	idle.PushBack({ 1059,502,363,389 });
	idle.PushBack({ 1619,502,363,389 });
	idle.PushBack({ 2181,502,363,389 });
	idle.PushBack({ 2742,502,363,389 });
	idle.PushBack({ 3297,502,363,389 });
	idle.PushBack({ 3836,502,363,389 });
	idle.PushBack({ 4375,502,363,389 });
	idle.PushBack({ 4791,502,363,389 });

	idle.loop = true;
	idle.speed = 20.0f;

	//Running left

	//Running right

	//Jumping left

	//Jumping right

	//Death
	death.PushBack({ 3,104,556,358 });
	death.PushBack({ 560,104,556,358 });
	death.PushBack({ 1170,104,556,358 });
	death.PushBack({ 1697,104,556,358 });
	death.PushBack({ 2231,104,556,358 });
	death.PushBack({ 2791,104,556,358 });
	death.PushBack({ 3354,104,556,358 });
	death.PushBack({ 3913,104,556,358 });

	idle.loop = false;
	idle.speed = 20.0f;

	//Initial screen

	current_animation = &idle;

}

j1Player::~j1Player()
{
	App->tex->UnLoad(graphics);

}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Initializing Player config");
	pugi::xml_node player = config.child("player");

	//position variables
}

bool j1Player::Start()
{


}

bool j1Player::Update()
{

}

bool j1Player::Load(pugi::xml_node& data)
{

}

bool j1Player::Save(pugi::xml_node& data) const
{

}