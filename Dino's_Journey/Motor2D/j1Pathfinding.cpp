#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Entities.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");


}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}


bool j1PathFinding::Start()
{
	pathTile = App->tex->Load("maps/PathTile.png");
	if (pathTile == nullptr)
	{
		LOG("Cannot load path textures");
		return false;
	}

	pathPtr = new Pathfinding();

	return true;
}


// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	App->tex->UnLoad(pathTile);

	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	width = App->map->data.width;
	height = App->map->data.height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	CheckBoundaries(pos);
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, Pathfinding* path)
{
	int ret = -1;

	iPoint goal = App->map->WorldToMap(destination.x, destination.y);
	iPoint start = App->map->WorldToMap(origin.x, origin.y);

	if (CheckBoundaries(goal)) //To check if the area is walkable
	{
		path->frontier.Push(start, 0);

		iPoint curr;

		if (path->frontier.Pop(curr))
		{

			iPoint neighbors[8];
			neighbors[0].create(curr.x + 1, curr.y + 0);
			neighbors[1].create(curr.x + 0, curr.y + 1);
			neighbors[2].create(curr.x - 1, curr.y + 0);
			neighbors[3].create(curr.x + 0, curr.y - 1);
			neighbors[4].create(curr.x + 1, curr.y + 1);
			neighbors[5].create(curr.x + 1, curr.y - 1);
			neighbors[6].create(curr.x - 1, curr.y + 1);
			neighbors[7].create(curr.x - 1, curr.y - 1);


			for (uint i = 0; i < 8; ++i)
			{
				uint distance = neighbors[i].DistanceTo(goal);

				if (IsWalkable(neighbors[i]))
				{
					if (path->visited.find(neighbors[i]) == -1)
					{
						path->frontier.Push(neighbors[i], distance);
						path->visited.add(neighbors[i]);
						path->breadcrumbs.add(curr);
					}

					
				}
			}
		}

	}
	return ret;
}