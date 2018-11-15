#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Collisions.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Window.h"

j1Player::j1Player()
{
	current_animation = nullptr;

	name.create("player");

}

j1Player::~j1Player() {}


bool j1Player::Awake(pugi::xml_node& config)
{
	LOG("Loading Player Parser");

	bool ret = true;

	//Player position
	initPosX = config.child("position").attribute("x").as_int();
	initPosY = config.child("position").attribute("y").as_int();

	//Animations
	file_path.create(config.child("file_path").child_value());
	Textures.create(config.child("textures").child_value());

	idle = LoadAnimation(file_path.GetString(), "Idle");
	jump = LoadAnimation(file_path.GetString(), "JumpR");
	run = LoadAnimation(file_path.GetString(), "RunR");
	dying = LoadAnimation(file_path.GetString(), "Death");
	int x = config.child("Collider").attribute("x").as_int();
	int y = config.child("Collider").attribute("y").as_int();
	int width = config.child("Collider").attribute("width").as_int();
	int height = config.child("Collider").attribute("height").as_int();

	//Speed and jumps
	pugi::xml_node speed = config.child("speed");
	
	//VALUES IN CONFIG ARE PLACEHOLDERS PLS
	gmSpeed = config.child("dynamics").attribute("godmode").as_float();
	gravity = config.child("gravity").attribute("value").as_float();
	jumpingTime = config.child("jumping_time").attribute("value").as_int();
	speedMultiplierX = config.child("speed_multiplier").attribute("valueX").as_float();
	speedMultiplierY = config.child("speed_multiplier").attribute("valueY").as_float();
	collisionMargin = config.child("margin").attribute("collisionMargin").as_int();
	jumpForce = config.child("jump_force").attribute("value").as_uint();
	jumpHeight = config.child("jump_height").attribute("value").as_uint();

	player_collider = { x,y,width,height };//SDL_Rect

	return ret;
}


bool j1Player::Start() 
{
	LOG("Loading player textures");
	sprites = App->tex->Load("textures/SpriteSheet.png");

	bool ret = true;
	
	colPlayer = App->collision->AddCollider(player_collider, COLLIDER_PLAYER, this);

	position.x = initPosX;
	position.y = initPosY;
	
	current_animation = idle;

	return ret;
}

bool j1Player::Update(float dt)
{

	//Player Controls

	//When in god mode
	if (godMode) 
	{

		current_animation = godmode;

		//Running right
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			
			position.x += gmSpeed;

		}
		//Running left
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
	
			position.x -= gmSpeed;

		}
		//Jumping
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
		
			position.y -= gmSpeed;

		}

	
	}
	else
	{
		// Idle
		if (App->input->GetKey(SDL_SCANCODE_D) ==KEY_IDLE
			&& App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
			current_animation = idle;


		//Movement of the player
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (hasWallBehind == false && isDead == false)
			{
				position.x -= speedMultiplierX;
				isLookingRight = false;
				current_animation = run;
			}
			else if (isDead == true)
			{
				isLookingRight = false;
				current_animation = dying;
			}
			else
				current_animation = idle;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (hasWallInFront == false && isDead == false)
			{
				position.x += speedMultiplierX;
				isLookingRight = true;
				current_animation = run;
			}
			else if (isDead == true)
			{
				isLookingRight = true;
				current_animation = dying;
			}
			else
				current_animation = idle;
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && isDead == false)
		{
			if (isJumping == false)
			{
				jumpPosition = initPosX - jumpHeight;
				isJumping = true;
			}
		}

	}


	//player collider
	colPlayer->SetPos(position.x, position.y);
	
	//Distance from start for parallax
	displacemetX = App->map->data.player_start_point.x - position.x;

	//Draw everything
	SDL_Rect dino = current_animation->GetCurrentFrame();

	App->render->Blit(sprites, (int)position.x, (int)position.y, &dino);


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

bool j1Player::Save(pugi::xml_node &config)const
{

	config.append_child("PlayerPosx").append_attribute("value") = position.x;
	config.append_child("PlayerPosy").append_attribute("value") = position.y;

	config.append_child("godmode").append_attribute("value") = godMode;

	return true;
}

bool j1Player::Load(pugi::xml_node &config)
{

	godMode = config.child("godmode").attribute("value").as_bool();

	position.x = config.child("PlayerPosx").attribute("value").as_float();
	position.y = config.child("PlayerPosy").attribute("value").as_float();

	return true;
}

bool j1Player::CleanUp()
{
	LOG("UNLOADING PLAYER");
	App->tex->UnLoad(sprites);

	return true;
}

void j1Player::OnCollision(Collider* col1, Collider* col2)
{

	if (col1->type == COLLIDER_PLAYER || col1->type == COLLIDER_NONE)
	{

		if (colPlayer->rect.y + colPlayer->rect.h >= col2->rect.y + collisionMargin
			&& colPlayer->rect.y <= col2->rect.y + col2->rect.h)
		{
			if (colPlayer->rect.x + colPlayer->rect.w >= col2->rect.w
				&& colPlayer->rect.x + colPlayer->rect.w >= col2->rect.x + col2->rect.w)
			{
				hasWallBehind = true;

			}
		}
		else
		{
			if (colPlayer->rect.x <= col2->rect.x + col2->rect.w
				&& colPlayer->rect.x + colPlayer->rect.w >= col2->rect.x + col2->rect.w)
			{
				hasWallBehind = true;
			}
		}
		

	}

}

void j1Player::Jump(float dt)
{
	if (!isJumping) 
	{
		initPosY += gravity * dt;
	}

	if (isJumping == true && initPosY != jumpPosition)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
			initPosY -= jumpForce * dt;
	}

	if (initPosY <= jumpPosition || App->input->GetKey(SDL_SCANCODE_SPACE == KEY_UP))
	{
		gravity += 10;
		isJumping = false;
		isFalling = true;
	}
}