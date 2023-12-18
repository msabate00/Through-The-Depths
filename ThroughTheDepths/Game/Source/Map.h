#ifndef __MAP_H__
#define __MAP_H__




#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"
#include "Pathfinding.h"
#include "Animation.h"

// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	Property* GetProperty(const char* name);

	List<Property*> list;
};

struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	float opacity = 1.0f;
	SString	tint;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct MapObject {
	
	uint id;
	uint x;
	uint y;
	uint width;
	uint height;
	List<uint> points;
};

struct MapObjects
{
	SString	name;
	int id;
	int x;
	int y;
	int width;
	int height;
	List<MapObject*> objects;

	Properties properties;

};




struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
	MapTypes type;

	List<MapLayer*> maplayers;
	List<MapObjects*> mapObjects;

};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called each loop iteration
	bool Update(float dt);
	bool PostUpdate();


	// Called each loop iteration after entities
	bool UpdateFrontEntities();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load();

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y);
	int GetTileWidth();
	int GetTileHeight();
	void CreateNavigationMap(int& width, int& height, uchar** buffer, MapLayer* navigationLayer) const;
private:

	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSet(pugi::xml_node mapFile);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);
	bool LoadObject(pugi::xml_node& node, MapObjects* layer);
	bool LoadAllObjectGroups(pugi::xml_node mapNode);
	TileSet* GetTilesetFromTileId(int gid) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	bool LoadCollisions(std::string layerName);
	bool LoadCollisionsObject();
	bool LoadEntities(std::string layerName);

	bool LoadNavigationLayer();
	


public: 

	MapData mapData;
	PathFinding* pathfindingFloor;
	PathFinding* pathfindingFly;
	MapLayer* navigationLayer_Floor;
	MapLayer* navigationLayer_Fly;

	SDL_Texture* tilePathTexRed = nullptr;
	SDL_Texture* tilePathTexBrown = nullptr;


private:

    SString mapFileName;
    SString mapFileName_Bosque;
    SString mapFileName_Pueblo;
	SString mapFolder;

	SString tilePathTexRedPath;
	SString tilePathTexBrownPath;

    bool mapLoaded;
	List<PhysBody*> traspasedPlatformList;
	
	List<PhysBody*> collisionsList;
	/*int collisionsListCount = 0;*/

	
	iPoint startPointcolisions = iPoint(-1, -1);
	iPoint colisionsPointsSize = iPoint(-1, -1);
	iPoint colisionsLastCords = iPoint(-1, -1);

	pugi::xml_document configFile;
	pugi::xml_node configNode;

	SString rainingTextPath;
	SDL_Texture* rainingTex;
	Animation* currentAnimation;
	Animation rainingAnim;




};

#endif // __MAP_H__