#include "j1Collisions.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"

j1Collisions::j1Collisions()
{

	for (uint i = 0; i < COLLIDER_MAX; ++i)
	{
		colliders[i] = nullptr;
	}

	name.create("Collision");

	matrix[COLLIDER_WALLS][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_WALLS][COLLIDER_WALLS] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_WALLS] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][CHECKPOINT] = true;
}

j1Collisions::~j1Collisions()
{

}

bool j1Collisions::PreUpdate()
{

	return true;
}

bool j1Collisions::Update(float dt)
{
	return true;
}

bool j1Collisions::PostUpdate()
{
	return true;
}

bool j1Collisions::CleanUp()
{

	LOG("Freeing all colliders");

	for (uint i = 0; i < COLLIDER_MAX; ++i)
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
	Collider * to_Add = new Collider(rect, type, callback);

	colliders.add(to_Add);

	return to_Add;

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
		case CHECKPOINT: // blue
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