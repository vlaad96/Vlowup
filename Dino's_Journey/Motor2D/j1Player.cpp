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

	position.x = 64;
	position.y = 384;


}

j1Player::~j1Player()
{
	App->tex->UnLoad(graphics);
	App->tex->UnLoad(sprites);
}

bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");

	bool ret = true;

	//Animations
	file_path.create(config.child("file_path").child_value());
	Textures.create(config.child("textures").child_value());

	idleR = LoadAnimation(file_path.GetString(), "Idle");
	jumpL = LoadAnimation(file_path.GetString(), "JumpL");
	jumpR = LoadAnimation(file_path.GetString(), "JumpR");
	runL = LoadAnimation(file_path.GetString(), "RunR");
	runR = LoadAnimation(file_path.GetString(), "RunL");
	dying = LoadAnimation(file_path.GetString(), "Death");
	int x = config.child("Collider").attribute("x").as_int();
	int y = config.child("Collider").attribute("y").as_int();
	int width = config.child("Collider").attribute("width").as_int();
	int height = config.child("Collider").attribute("height").as_int();

	player_collider = { x,y,width,height };//SDL_Rect
	

	return ret;
}


bool j1Player::Start() 
{
	LOG("Loading player textures");
	bool ret = true;
	graphics = App->tex->Load("textures/SpriteSheet.png");

	
	/*if (sprites == nullptr)
	{
		sprites = App->tex->Load(Textures.GetString());
	}*/

	current_animation = idleR;

	return ret;
}

bool j1Player::Update()
{
	current_animation = idleR;

	//Movement

	//Running right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		current_animation = runR;
		
	}
	//Running left
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		current_animation = runL;

	}
	//Jumping
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		current_animation = jumpR;

	}
	//slide
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		current_animation = slideR;

	}

	//Draw everything

	App->render->Blit(sprites, position.x, position.y, &current_animation->GetCurrentFrame());


	return true;
}

Animation* j1Player::LoadAnimation(const char* path, const char* name)
{
	Animation* anim = new Animation();

	bool ret = false;

	pugi::xml_document animDoc;
	pugi::xml_parse_result res = animDoc.load_file(path);

	if (res == NULL)
	{
		LOG("Cannot load animations");
	}

	pugi::xml_node objectgroup;
	for (objectgroup = animDoc.child("map").child("objectgroup");objectgroup; objectgroup = objectgroup.next_sibling("objectgroup"))
	{
		p2SString iname = objectgroup.attribute("name").as_string();

		if (iname == name) 
		{
			ret = true;

			int x, y, width, height;

			for (pugi::xml_node sprite = objectgroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
			{
				x = sprite.attribute("x").as_int();
				y = sprite.attribute("y").as_int();
				width = sprite.attribute("width").as_int();
				height = sprite.attribute("height").as_int();

				anim->PushBack({ x,y,width,height });
			}
		}
	}

	if (ret == true)
		return anim;
	else
		return nullptr;

}