#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1App.h"
#include "p2PQueue.h"

struct SDL_Texture;


#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

struct Pathfinding
{
	p2DynArray<iPoint>	path;
	p2PQueue<iPoint>	frontier;
	p2List<iPoint>		visited;
	p2List<iPoint>		breadcrumbs;

};

class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	bool Start();


	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Main function to request a path from A to B
	int CreatePath(const iPoint& origin, const iPoint& destination, Pathfinding* path);

	// To request all tiles involved in the last generated path
	const p2DynArray<iPoint>* GetLastPath() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;


private:

	// size of the map
	uint					width;
	uint					height;
	// all map walkability values [0..255]
	uchar*					map;
	// we store the created path here
	p2DynArray<iPoint>		last_path;

	SDL_Texture*			pathTile = nullptr;
	Pathfinding*			pathPtr = nullptr;

};

/



#endif // __j1PATHFINDING_H__