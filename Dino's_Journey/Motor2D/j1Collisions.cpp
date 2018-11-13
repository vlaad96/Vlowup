#include "j1Collisions.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"

j1Collisions::j1Collisions()
{

	for (uint i = 0; i < MAX_COLLIDER; ++i)
	{
		collider[i] = nullptr;
	}

	name.create("Collision");

	matrix[COLLIDER_NONE][COLLIDER_NONE] = false;
	matrix[COLLIDER_NONE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_NONE][COLLIDER_WALLS] = false;
	matrix[COLLIDER_NONE][COLLIDER_SPIKES] = false;
	matrix[COLLIDER_NONE][COLLIDER_ACID] = false;
	matrix[COLLIDER_NONE][WIN_COLLIDER] = false;

	matrix[COLLIDER_WALLS][COLLIDER_NONE] = false;
	matrix[COLLIDER_WALLS][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALLS][COLLIDER_SPIKES] = false;
	matrix[COLLIDER_WALLS][COLLIDER_ACID] = false;
	matrix[COLLIDER_WALLS][WIN_COLLIDER] = false;
	matrix[COLLIDER_WALLS][COLLIDER_WALLS] = false;


	matrix[COLLIDER_PLAYER][COLLIDER_NONE] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_WALLS] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SPIKES] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ACID] = true;
	matrix[COLLIDER_PLAYER][WIN_COLLIDER] = true;

	matrix[COLLIDER_SPIKES][COLLIDER_NONE] = false;
	matrix[COLLIDER_SPIKES][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SPIKES][COLLIDER_WALLS] = false;
	matrix[COLLIDER_SPIKES][COLLIDER_ACID] = false;
	matrix[COLLIDER_SPIKES][WIN_COLLIDER] = false;
	matrix[COLLIDER_SPIKES][COLLIDER_SPIKES] = false;

	matrix[COLLIDER_ACID][COLLIDER_NONE] = true;
	matrix[COLLIDER_ACID][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ACID][COLLIDER_SPIKES] = false;
	matrix[COLLIDER_ACID][COLLIDER_WALLS] = false;
	matrix[COLLIDER_ACID][WIN_COLLIDER] = false;
	matrix[COLLIDER_ACID][COLLIDER_ACID] = false;

	matrix[WIN_COLLIDER][COLLIDER_NONE] = true;
	matrix[WIN_COLLIDER][COLLIDER_ACID] = false;
	matrix[WIN_COLLIDER][COLLIDER_PLAYER] = true;
	matrix[WIN_COLLIDER][COLLIDER_WALLS] = false;
	matrix[WIN_COLLIDER][COLLIDER_SPIKES] = false;
	matrix[WIN_COLLIDER][WIN_COLLIDER] = false;

}

j1Collisions::~j1Collisions()
{

}

bool j1Collisions::PreUpdate()
{
	p2List_item<Collider*>*item;
	item = colliders.start;

	while(item!=NULL)
	{
		if (item->data->to_delete == true)
		{
			RELEASE(item->data)
		}

		item = item->next;
	}

	Collider* col1;
	Collider* col2;

	for (uint i = 0; i < MAX_COLLIDER; ++i) 
	{
		if (collider[i] == nullptr) continue;

		if (collider[i]->type == COLLIDER_PLAYER || collider[i]->type == COLLIDER_NONE)
		{
			col1 = collider[i];

			for (uint j = 0; j < MAX_COLLIDER; ++j)
			{

				if (collider[j] == nullptr || i == j) continue;

				col2 = collider[j];

				if (col1->CheckCollision(col2->rect) == true)
				{
					if (matrix[col1->type][col2->type] && col1->callback)
						col1->callback->OnCollision(col1, col2);
				}
			}
		}

	}

	return true;
}

bool j1Collisions::Update(float dt)
{
	DebugDraw();

	return true;
}


bool j1Collisions::CleanUp()
{

	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDER; ++i)
	{
		if (collider[i] != nullptr)
		{
			delete collider[i];
			collider[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDER; ++i)
	{

		if (collider[i] == nullptr)
		{
			ret = collider[i] = new Collider(rect, type, callback);
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

	p2List_item <Collider*> *item;
	item = colliders.start;

	Uint8 alpha = 80;

	while (item != NULL)
	{

		switch (item->data->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(item->data->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALLS: // red
			App->render->DrawQuad(item->data->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(item->data->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_SPIKES:
			App->render->DrawQuad(item->data->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_ACID:
			App->render->DrawQuad(item->data->rect, 0, 0, 255, alpha);
			break;

		case WIN_COLLIDER: // blue
			App->render->DrawQuad(item->data->rect, 0, 0, 128, alpha);
			break;

		}
		item = item->next;
	}

}


bool Collider::CheckCollision(const SDL_Rect & r) const
{
	return (rect.x <= r.x + r.w &&
		rect.x + rect.w >= r.x &&
		rect.y <= r.y + r.h &&
		rect.h + rect.y >= r.y);

}