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

}

j1Player::~j1Player()
{
	App->tex->UnLoad(graphics);

}

bool j1Player::Start()
{


}

bool j1Player::PostUpdate()
{

}

bool j1Player::Load(pugi::xml_node& data)
{

}

bool j1Player::Save(pugi::xml_node& data) const
{

}