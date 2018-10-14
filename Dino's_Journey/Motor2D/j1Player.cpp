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
	frames = 0;
	time = 0;
	allow_time = true;
}

j1Player::~j1Player()
{

}

bool j1Player::Awake(pugi::xml_node& conf)
{
	LOG("Initializing Player config");
	pugi::xml_node player = conf.child("player");

	//Animations
	file_path.create(conf.child("file path").child_value());
	animation.create(conf.child("Animation").child_value());

	idle_left = LoadAnim(file_path.GetString(), "idle left");
	idle_right = LoadAnim(file_path.GetString(), "idle right");
	jump = LoadAnim(file_path.GetString(), "jump");
	run_left = LoadAnim(file_path.GetString(), "run left");
	run_right = LoadAnim(file_path.GetString(), "run right");
	slide_left = LoadAnim(file_path.GetString(), "slide left");
	slide_right = LoadAnim(file_path.GetString(), "slide right");
	death = LoadAnim(file_path.GetString(), "death");

	//Collisions
	int x = conf.child("collider").attribute("x").as_int();
	int y = conf.child("collider").attribute("y").as_int();
	int w = conf.child("collider").attribute("width").as_int();
	int h = conf.child("collider").attribute("height").as_int();
	player_collider = { x,y,w,h };

	jumping_time = conf.child("jumping_time").attribute("value").as_int();
	sliding_time = conf.child("sliding_time").attribute("value").as_int();
	speed_modifier.x = conf.child("speed_modifier.x").attribute("value").as_float();
	speed_modifier.y = conf.child("speed_modifier.y").attribute("value").as_float();
	gravity = conf.child("gravity").attribute("value").as_float();

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
	if (sprites == nullptr)
	{
		sprites = App->tex->Load(animation.GetString());
	}

	col = App->collision->AddCollider(player_collider, COLLIDER_PLAYER);


	return ret;
}
//Background drawing during update
bool j1Player::Update(float dt)
{
	if (touching.x != 0)
	{
		speed.y = speed_modifier.y;
	}
	else
		speed.y = speed_modifier.y * 2;

	current_animation = idle_right;
	speed.x = 0;

	if (dead)
	{
		current_animation = death;

		if (current_animation->Finished())
		{
			position.x = App->map->data.player_start_point.x;
			position.y = App->map->data.player_end_point.y - 5;
			death->Reset();
			dead = false;
		}
	}
	if (!won && !dead) {

		//run to the left
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{

			if (touching.y == 1)

				current_animation = run_left;

			if (touching.x != 1)

				speed.x = -speed_modifier.x;
		}
		//run to the right
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (touching.y == 1)
			{
				current_animation = run_right;
			}
			if (touching.x != 2)
			{
				speed.x = speed_modifier.x;
			}
		}
		//slide
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{

			sliding = true;
		}
		//Jumping
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{

			if (touching.y == 1)

				jumping = true;

		}


		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (!godmode)
			{
				godmode = true;
			}
			else
				godmode = false;
		}

		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			if (App->map->map != 0)
			{
				App->map->map = 0;
				App->map->CleanUp();
				App->map->Load("Newlevel1.tmx");
			}


			position.x = App->map->data.player_start_point.x;
			position.y = App->map->data.player_start_point.y - gravity;
		}

		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			position.x = App->map->data.player_start_point.x;
			position.y = App->map->data.player_start_point.y - gravity;
		}
	}

	Jump();
	Slide();

	App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame());

	//Camera for player

	App->render->camera.x = -position.x;
	App->render->camera.y = -position.y;

	frames++;
	return true;
}

bool j1Player::PostUpdate()
{

	bool ret = true;

	App->render->Blit(sprites, position.x, position.y, &current_animation->GetCurrentFrame());

	return ret;
}

Animation* j1Player::LoadAnim(const char* path, const char* aname) {

	Animation* animation = new Animation();

	bool anim = false;

	pugi::xml_document anim_file;
	pugi::xml_parse_result result = anim_file.load_file(path);


	if (result == NULL)
	{
		LOG("Issue loading animation");
	}

	pugi::xml_node objgroup;
	for (objgroup = anim_file.child("map").child("objectgroup"); objgroup; objgroup = objgroup.next_sibling("objectgroup"))
	{
		p2SString name = objgroup.attribute("name").as_string();
		if (name == aname)
		{
			anim = true;
			int x, y, h, w;

			for (pugi::xml_node sprite = objgroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
			{
				x = sprite.attribute("x").as_int();
				y = sprite.attribute("y").as_int();
				w = sprite.attribute("width").as_int();
				h = sprite.attribute("height").as_int();

				animation->PushBack({ x, y, w, h });
			}

		}
	}
	if (anim = true)
		return animation;
	else
		return nullptr;

}

bool j1Player::CleanUp()
{
	bool ret = true;
	App->tex->UnLoad(sprites);

	return ret;
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
			App->map->Load("Newlevel2.tmx");
			position.x = data.child("player_position").attribute("x").as_float();
			position.y = data.child("player_position").attribute("y").as_float();
		}
		else
		{
			App->map->map = 0;
			App->map->CleanUp();
			App->map->Load("Newlevel1.tmx");
			position.x = data.child("player_position").attribute("x").as_float();
			position.y = data.child("player_position").attribute("y").as_float();
		}
	}

	return true;
}

void j1Player::Jump()
{
	if (jumping)
	{

		if (allow_time)
		{
			time = frames;
			allow_time = false;
			touching.y = 0;

		}

		if (frames - time <= jumping_time && touching.y == 0)
		{
			current_animation = jump;
			position.y -= speed.y;
		}
		else
		{
			jumping = false;
			allow_time = true;
			jump->Reset();
			
		}
	}

}

void j1Player::Slide()
{
	if (sliding)
	{
		if (allow_time)
		{
			time = frames;
			allow_time = false;

			col->SetSize(col->rect.w, App->map->data.tile_height);
			
		}
	}
}