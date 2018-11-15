#include "j1Collisions.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"

j1Collisions::j1Collisions()
{

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}

	name.create("Collision");

	matrix[COLLIDER_NONE][COLLIDER_NONE] = false;
	matrix[COLLIDER_NONE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_NONE][COLLIDER_WALLS] = false;
	matrix[COLLIDER_NONE][COLLIDER_SPIKES] = false;
	matrix[COLLIDER_NONE][COLLIDER_ACID] = false;
	matrix[COLLIDER_NONE][COLLIDER_FLAG] = false;

	matrix[COLLIDER_WALLS][COLLIDER_NONE] = false;
	matrix[COLLIDER_WALLS][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALLS][COLLIDER_SPIKES] = false;
	matrix[COLLIDER_WALLS][COLLIDER_ACID] = false;
	matrix[COLLIDER_WALLS][COLLIDER_FLAG] = false;
	matrix[COLLIDER_WALLS][COLLIDER_WALLS] = false;


	matrix[COLLIDER_PLAYER][COLLIDER_NONE] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_WALLS] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SPIKES] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ACID] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FLAG] = true;

	matrix[COLLIDER_SPIKES][COLLIDER_NONE] = false;
	matrix[COLLIDER_SPIKES][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SPIKES][COLLIDER_WALLS] = false;
	matrix[COLLIDER_SPIKES][COLLIDER_ACID] = false;
	matrix[COLLIDER_SPIKES][COLLIDER_FLAG] = false;
	matrix[COLLIDER_SPIKES][COLLIDER_SPIKES] = false;

	matrix[COLLIDER_ACID][COLLIDER_NONE] = true;
	matrix[COLLIDER_ACID][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ACID][COLLIDER_SPIKES] = false;
	matrix[COLLIDER_ACID][COLLIDER_WALLS] = false;
	matrix[COLLIDER_ACID][COLLIDER_FLAG] = false;
	matrix[COLLIDER_ACID][COLLIDER_ACID] = false;

	matrix[COLLIDER_FLAG][COLLIDER_NONE] = true;
	matrix[COLLIDER_FLAG][COLLIDER_ACID] = false;
	matrix[COLLIDER_FLAG][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FLAG][COLLIDER_WALLS] = false;
	matrix[COLLIDER_FLAG][COLLIDER_SPIKES] = false;
	matrix[COLLIDER_FLAG][COLLIDER_FLAG] = false;

}

j1Collisions::~j1Collisions()
{

}

bool j1Collisions::PreUpdate()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

bool j1Collisions::Update(float dt)
{
	Collider* col;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE || colliders[i]->type == COLLIDER_PLAYER)
			continue;

		if (colliders[i]->type == COLLIDER_WALLS)
		{
			if (colliders[i]->CheckBotCollider(App->player->colPlayer->rect, ceil(App->player->gravity))) //ceil rounds the number up, to the biggest possible. For example of 5.6, the ceil would be 6
				App->player->isTouchingGround = true;

			if (colliders[i]->CheckTopCollider(App->player->colPlayer->rect, ceil(App->player->speed.y)))
				App->player->hasWallAbove = true;

			if (colliders[i]->CheckLeftCollider(App->player->colPlayer->rect, ceil(App->player->speedMultiplierX)))
				App->player->hasWallBehind = true;

			if (colliders[i]->CheckRightCollider(App->player->colPlayer->rect, ceil(App->player->speedMultiplierX)))
				App->player->hasWallInFront = true;

		}

		else if (colliders[i]->type == COLLIDER_ACID || colliders[i]->type == COLLIDER_SPIKES || colliders[i]->type == COLLIDER_FLAG)
		{
			col = colliders[i];

			if (App->player->colPlayer->CheckCollision(col->rect) == true)
			{

				if (matrix[App->player->colPlayer->type][col->type])
				{
					if (col->type == COLLIDER_ACID || col->type == COLLIDER_SPIKES)
						App->player->isDead = true;
					else if (col->type == COLLIDER_FLAG)
					{
						if (App->map->level == 0)
						{
							App->map->CleanUp();
							App->map->Load("Newlevel2.tmx");
							App->map->level = 1;
						}
						else if (App->map->level == 1)
						{
							App->map->CleanUp();
							App->map->Load("Newlevel1.tmx");
							App->map->level = 0;
						}
					}
				}
			}
		}
	}

	DebugDraw();

	return true;
}


bool j1Collisions::CleanUp()
{

	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{

		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}
	return ret;

}

void j1Collisions::DebugDraw()
{

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) //collider draw
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALLS: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_SPIKES:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_ACID:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;

		case COLLIDER_FLAG: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 128, alpha);
			break;

		}
	}

}


bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x <= r.x + r.w &&
		rect.x + rect.w >= r.x &&
		rect.y <= r.y + r.h &&
		rect.h + rect.y >= r.y);

}

bool Collider::CheckLeftCollider(const SDL_Rect& r, int dist)const
{
	bool ret;

	if (r.y + r.h > rect.y &&
		r.y < rect.y + rect.h &&
		r.x < rect.x + rect.w + dist &&
		r.x + r.w > rect.x)
	{
		ret = true;
	}
	else
		ret = false;

	return ret;
}

bool Collider::CheckRightCollider(const SDL_Rect& r, int dist)const
{
	bool ret;

	if (r.y + r.h > rect.y &&
		r.y < rect.y + rect.h &&
		r.x + r.w > rect.x - dist &&
		r.x < rect.x + rect.w)
	{
		ret = true;
	}
	else
		ret = false;

	return ret;
}

bool Collider::CheckTopCollider(const SDL_Rect& r, int dist)const
{
	bool ret;

	if (r.y + r.h > rect.y &&
		r.y < rect.y + rect.h + dist &&
		r.x + r.w > rect.x &&
		r.x < rect.x + rect.w)
	{
		ret = true;
	}
	else
		ret = false;

	return ret;
}

bool Collider::CheckBotCollider(const SDL_Rect& r, int dist)const
{
	bool ret;

	if (r.y < rect.y + rect.h &&
		r.y + r.h > rect.y - dist &&
		r.x + r.w > rect.x  &&
		r.x < rect.x + rect.w)
	{
		ret = true;
	}
	else
		ret = false;

	return ret;
}