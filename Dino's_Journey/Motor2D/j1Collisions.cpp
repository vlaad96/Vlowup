#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1Player.h"
#include "j1Map.h"

j1Collisions::j1Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_DEAD] = true;
}

// Destructor
j1Collisions::~j1Collisions()
{}



bool j1Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
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

// Called before render is available
bool j1Collisions::Update(float dt)
{
	Collider* c;

	/*for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty and player colliders
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE || colliders[i]->type == COLLIDER_PLAYER)
			continue;

		if (colliders[i]->type == COLLIDER_WALL)
		{
			if (colliders[i]->WillCollideGround(App->player->collider->rect, ceil(App->player->gravity)))
				App->player->contact.y = 1;

			if (colliders[i]->WillCollideTop(App->player->collider->rect, ceil(App->player->speed.y)))
				App->player->contact.y = 2;

			if (colliders[i]->WillCollideLeft(App->player->collider->rect, App->player->speed_modifier.x))
				App->player->contact.x = 1;

			if (colliders[i]->WillCollideRight(App->player->collider->rect, App->player->speed_modifier.x))
				App->player->contact.x = 2;
		

		DebugDraw();

		return true;
	}*/
	return true;
}

void j1Collisions::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
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
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha, false);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha, false);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha, false);
			break;
		case COLLIDER_DEAD: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha, true);
			break;
		}
	}
}

// Called before quitting
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

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type);
			break;
		}
	}

	return ret;
}


// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideLeft(const SDL_Rect& r, int distance) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x < rect.x + rect.w + distance && r.x + r.w > rect.x)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideRight(const SDL_Rect& r, int distance) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h && r.x + r.w > rect.x - distance && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideGround(const SDL_Rect& r, int distance) const
{
	if (r.y < rect.y + rect.h && r.y + r.h > rect.y - distance && r.x + r.w > rect.x && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool Collider::WillCollideTop(const SDL_Rect& r, int distance) const
{
	if (r.y + r.h > rect.y && r.y < rect.y + rect.h + distance && r.x + r.w > rect.x && r.x < rect.x + rect.w)
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool j1Collisions::CheckCollisionAfterSlide(const SDL_Rect& r, int dist) const
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty and player colliders
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE || colliders[i]->type == COLLIDER_PLAYER)
			continue;

		if (colliders[i]->type == COLLIDER_WALL && colliders[i]->WillCollideTop(r, dist))
		{
			return true;
		}
	}

	return false;
}

//THIS WILL GRAB THE TILES FROM THE MAP FROM LAYER "Colliders" AND TRANSFORM THEM INTO COLLIDERS (incomplete)
void j1Collisions::MapTilesToColliders(pugi::xml_node &node, const SDL_Rect r) { //At node pass the node layer = map_file.child("map").child("layer") It's at the map

	for (p2List_item<MapLayer*>*layers = App->map->data.layers.start; layers != nullptr; layers = layers->next) 
	{
		if (layers->data->name == "Colliders") 
		{
			for (int y = 0; y < App->map->data.height; ++y)
			{
				for (int x = 0; x < App->map->data.width; ++x)
				{
					int tile_id = layers->data->Get(x, y);
					if (tile_id == 145) {

						iPoint pos = App->map->MapToWorld(x, y);
						App->collision->AddCollider({ pos.x, pos.y, App->map->data.tile_width, App->map->data.tile_height }, COLLIDER_WALL);

					}
				}
			}
		}
	}

	LOG("Error Parsing map, couldn't find colliders layer");
}
