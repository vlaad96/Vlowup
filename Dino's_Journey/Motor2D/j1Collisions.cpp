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
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
}

	// Destructor
j1Collisions::~j1Collisions()
{}

bool j1Collisions::Awake()
{
	bool ret = true;
	return ret;
}

bool j1Collisions::Start()
{

		CollidersFromMap("Newlevel1.tmx"); //No va 
		bool ret = true;
		return ret;
}

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

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty and player colliders
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE || colliders[i]->type == COLLIDER_PLAYER)
			continue;

		if (colliders[i]->type == COLLIDER_WALL)
		{
			if (colliders[i]->WillCollideGround(App->player->col->rect, ceil(App->player->gravity)))
				App->player->touching.y = 1;

			if (colliders[i]->WillCollideTop(App->player->col->rect, ceil(App->player->speed.y)))
				App->player->touching.y = 2;

			if (colliders[i]->WillCollideLeft(App->player->col->rect, App->player->speed_modifier.x))
				App->player->touching.x = 1;

			if (colliders[i]->WillCollideRight(App->player->col->rect, App->player->speed_modifier.x))
				App->player->touching.x = 2;
		}
		else if ( colliders[i]->type == COLLIDER_DEATH)
		{
			c = colliders[i];

			if (App->player->col->CheckCollision(c->rect) == true)
			{

				if (matrix[App->player->col->type][c->type])
				{
					if (c->type == COLLIDER_DEATH)
					{
						App->player->dead = true;
					}
					
				
				}

			}
		}
	}

	DebugDraw();

	return true;
}


bool j1Collisions::PostUpdate()
{
	bool ret = true;
	return ret;
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
		case COLLIDER_DEATH: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha, true);
			break;
		case COLLIDER_WIN: //green too
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha, false);
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

void j1Collisions::CollidersFromMap(const char* file_name) {

	p2SString tmp("%s%s", App->map->folder.GetString(), file_name);

	pugi::xml_parse_result result = App->map->map_file.load_file(tmp.GetString());

	if (result == NULL) {

		LOG("Could not load xml file %s. pugi error: %s", file_name, result.description());
		return;
	}
	pugi::xml_node collider;
	pugi::xml_node type;
	const char* colliderlayername;

	for (type = App->map->map_file.child("map").child("objectgroup"); type && result; type = type.next_sibling("objectgroup")) {

		colliderlayername = type.attribute("name").as_string();

		for (collider = type.child("object"); collider&&result; collider = collider.next_sibling("object")) {

			if (colliderlayername == "Colliders_Wall") 
			{
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_WALL);
			}
			if (colliderlayername == "Colliders_Death") 
			{
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_DEATH);
			}
			if (colliderlayername == "Colliders_Win") 
			{
				AddCollider({ collider.attribute("x").as_int(),collider.attribute("y").as_int(),collider.attribute("width").as_int(),collider.attribute("height").as_int() }, COLLIDER_TYPE::COLLIDER_WIN);
			}
		}
	}
}
