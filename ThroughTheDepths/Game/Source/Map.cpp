
#include <string>
#include <iostream>

#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"
#include <bitset>
#include "Chest.h"
#include "Coin.h"
#include "PlantBarrier.h"
#include "PlantBreakable.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

std::string toBinary(int n)
{
    std::string r;
    while (n != 0) { r = (n % 2 == 0 ? "0" : "1") + r; n /= 2; }
    return r;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    return ret;
}

bool Map::Update(float dt)
{
    
    if(app->scene->getPlayer() != nullptr && app->scene->getPlayer()->pbody != nullptr){

        if (app->scene->getPlayer()->pbody->body->GetLinearVelocity().y < 0) {
            for (int i = 0; i < traspasedPlatformList.Count(); i++) {
                traspasedPlatformList.At(i)->data->body->SetActive(false);
            }
       
        
        }
        else {
            if (!app->scene->getPlayer()->traspassingColision) {
                for (int i = 0; i < traspasedPlatformList.Count(); i++) {
                    if (traspasedPlatformList.At(i)->data->body->GetPosition().y > app->scene->getPlayer()->pbody->body->GetPosition().y) {
                        traspasedPlatformList.At(i)->data->body->SetActive(true);
                    }
                    
                }
            }
        }
    }


    return true;
}

bool Map::PostUpdate()
{

    if (mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value &&
            mapLayerItem->data->properties.GetProperty("Front") != NULL && !mapLayerItem->data->properties.GetProperty("Front")->value
            ) {


            iPoint playerPos = app->scene->getPlayer()->position;            
            int xToTiledLeft = MAX((playerPos.x / 32)- TILES_TO_LOAD, 0);
            int xToTiledRight = MIN((playerPos.x / 32)+ TILES_TO_LOAD, mapLayerItem->data->width);

            int yToTiledTop = MAX((playerPos.y / 32) - TILES_TO_LOAD, 0);
            int yToTiledDown = MIN((playerPos.y / 32) + TILES_TO_LOAD, mapLayerItem->data->height);

            for (int x = xToTiledLeft; x < xToTiledRight; x++)
            {
               
                for (int y = yToTiledTop; y < yToTiledDown; y++)
                {
                    unsigned int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);
                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);
                    int bits = 0;
                    SDL_RendererFlip flip = SDL_FLIP_NONE;
                    int angle = 0;

                    if (gid >= 100000) {
                        uint tiledID = static_cast<uint>(gid & ~0xE0000000);
                        bits = gid >> 29;
                        tileset = GetTilesetFromTileId(tiledID);
                        r = tileset->GetTileRect(tiledID);

                    }
                    //1 = hoz_flip -> True || 1 = vert_flip -> True  || 0 = anti-diag flip -> False
                    switch (bits) {
                    case 0b101: flip = SDL_FLIP_NONE;           angle = 90;         break;
                    case 0b110: flip = SDL_FLIP_NONE;           angle += 180;       break;
                    case 0b011: flip = SDL_FLIP_NONE;           angle += 270;       break;
                    case 0b100: flip = SDL_FLIP_HORIZONTAL;     angle = 0;          break;
                    case 0b111: flip = SDL_FLIP_HORIZONTAL;     angle += 90;        break;
                    case 0b010: flip = SDL_FLIP_HORIZONTAL;     angle += 180;       break;
                    case 0b001: flip = SDL_FLIP_HORIZONTAL;     angle += 270;       break;
                    }
                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y, flip,
                        &r, 1, angle);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }

    return true;
}

bool Map::UpdateFrontEntities()
{

    if (mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value &&
            mapLayerItem->data->properties.GetProperty("Front") != NULL && mapLayerItem->data->properties.GetProperty("Front")->value
            ) {


            iPoint playerPos = app->scene->getPlayer()->position;
            int xToTiledLeft = MAX((playerPos.x / 32) - TILES_TO_LOAD, 0);
            int xToTiledRight = MIN((playerPos.x / 32) + TILES_TO_LOAD, mapLayerItem->data->width);

            int yToTiledTop = MAX((playerPos.y / 32) - TILES_TO_LOAD, 0);
            int yToTiledDown = MIN((playerPos.y / 32) + TILES_TO_LOAD, mapLayerItem->data->height);

            for (int x = xToTiledLeft; x < xToTiledRight; x++)
            {
                for (int y = yToTiledTop; y < yToTiledDown; y++)
                {
                    unsigned int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);
                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);
                    int bits = 0;
                    SDL_RendererFlip flip = SDL_FLIP_NONE;
                    int angle = 0;

                    if (gid >= 100000) {
                        uint tiledID = static_cast<uint>(gid & ~0xE0000000);
                        bits = gid >> 29;
                        tileset = GetTilesetFromTileId(tiledID);
                        r = tileset->GetTileRect(tiledID);

                    }
                    //1 = hoz_flip -> True || 1 = vert_flip -> True  || 0 = anti-diag flip -> False
                    switch (bits) {
                    case 0b101: flip = SDL_FLIP_NONE;           angle = 90;         break;
                    case 0b110: flip = SDL_FLIP_NONE;           angle += 180;       break;
                    case 0b011: flip = SDL_FLIP_NONE;           angle += 270;       break;
                    case 0b100: flip = SDL_FLIP_HORIZONTAL;     angle = 0;          break;
                    case 0b111: flip = SDL_FLIP_HORIZONTAL;     angle += 90;        break;
                    case 0b010: flip = SDL_FLIP_HORIZONTAL;     angle += 180;       break;
                    case 0b001: flip = SDL_FLIP_HORIZONTAL;     angle += 270;       break;
                    }
                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y, flip,
                        &r, 1, angle);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }

    return true;
}

iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y) 
{
    iPoint ret(0, 0);

    //

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    return true;
}

// Load new map
bool Map::Load()
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    if (ret == true)
    {
        ret = LoadAllObjectGroups(mapFileXML.child("map"));
    }
    // NOTE: Later you have to create a function here to load and create the colliders from the map





    //NUEVO
    LoadCollisionsObject();
    LoadCollisions("Colisions");
    LoadEntities("Entidades");
    
    if(ret == true)
    {
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
        mapData.type = MAPTYPE_UNKNOWN;
    }

    return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_uint();
        i++;
    }

    return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}



bool Map::LoadObject(pugi::xml_node& node, MapObjects* mapObjects)
{
    bool ret = true;

    //Load the attributes
    mapObjects->id = node.attribute("id").as_int();
    mapObjects->name = node.attribute("name").as_string();
    mapObjects->width = node.attribute("width").as_int();
    mapObjects->width = node.attribute("width").as_int();
    mapObjects->x = node.attribute("x").as_int();
    mapObjects->y = node.attribute("y").as_int();

    LoadProperties(node, mapObjects->properties);

    //Reserve the memory for the data 
    //mapObjects->objects = new MapObject[mapObjects->width * mapObjects->height];
    //memset(mapObjects->objects, 0, mapObjects->width * mapObjects->height);

    //Iterate over all the objects and assign the values
    pugi::xml_node object;
    int i = 0;
    for (object = node.child("object"); object && ret; object = object.next_sibling("object"))
    {
        mapObjects->objects.Add(new MapObject{
            object.attribute("id").as_uint(),
            object.attribute("x").as_uint(),
            object.attribute("y").as_uint(),
            object.attribute("width").as_uint(),
            object.attribute("height").as_uint(),
            List<uint>()

            });
        i++;
    }

    return ret;
}



bool Map::LoadAllObjectGroups(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node objectNode = mapNode.child("objectgroup"); objectNode && ret; objectNode = objectNode.next_sibling("objectgroup"))
    {
        //Load the layer
        MapObjects* mapObjects = new MapObjects();
        ret = LoadObject(objectNode, mapObjects);

        //add the layer to the map
        mapData.mapObjects.Add(mapObjects);
    }

    return ret;
}



bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}

//NUEVO
bool Map::LoadCollisions(std::string layerName) {


    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;
    bool ret = false;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->name.GetString() == layerName) {
            

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {

                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);
                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                   

                    /*No borrar, original sistema de colisiones*/
                    if (gid == tileset->firstgid) {
                        PhysBody* c1 = app->physics->CreateRectangle(pos.x+16, pos.y+16 , 32, 32, STATIC);
                        c1->ctype = ColliderType::PLATFORM;
                        ret = true;
                    }
                   

                    if (gid == tileset->firstgid + 1) {
                        PhysBody* c1 = app->physics->CreateRectangle(pos.x + 16, pos.y+2, 32, 4, STATIC);
                        c1->ctype = ColliderType::PLATFORM_TRASPASS;
                        traspasedPlatformList.Add(c1);
                        ret = true;
                    }
                    if (gid == tileset->firstgid + 6) {
                        PhysBody* c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
                        c1->ctype = ColliderType::SPYKES;
                        
                        ret = true;
                    }

                    //Fuera del mapa
                    if (gid == tileset->firstgid + 15) {
                        PhysBody* c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
                        c1->ctype = ColliderType::DIE_HOLE;

                        ret = true;
                    }

                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }

    return ret;




}

bool Map::LoadCollisionsObject()
{

    ListItem<MapObjects*>* mapObjectsItem;
    mapObjectsItem = mapData.mapObjects.start;
    bool ret = false;


    while (mapObjectsItem != NULL) {
        for (int i = 0; i < mapObjectsItem->data->objects.Count(); i++) {

            MapObject* object = mapObjectsItem->data->objects[i];

            PhysBody* c1 = app->physics->CreateRectangle(object->x + object->width/2, object->y + object->height/2, object->width, object->height, STATIC);
            c1->ctype = ColliderType::PLATFORM;

        }
            mapObjectsItem = mapObjectsItem->next;
    }


    return false;
}

bool Map::LoadEntities(std::string layerName)
{

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;
    bool ret = false;

    pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
    if (parseResult) {
        configNode = configFile.child("config");
    }
    else {
        LOG("Error in Map::LoadEntities(): %s", parseResult.description());
        return false;
    }

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->name.GetString() == layerName) {
            for (int x = 0; x < mapLayerItem->data->width; x++)
            {

                for (int y = 0; y < mapLayerItem->data->height; y++)
                {


                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);
                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    //Monedas
                    if (gid == tileset->firstgid) {
                       Coin* coin = (Coin*)app->entityManager->CreateEntity(EntityType::COIN);
                       coin->parameters = configNode.child("scene").child("coin");
                       coin->position = iPoint(pos.x + 16, pos.y + 16);
                    }

                    //cofre con Monedas
                    if (gid == tileset->firstgid + 1) {
                        Chest* chest = (Chest*)app->entityManager->CreateEntity(EntityType::CHEST_COIN);
                        chest->parameters = configNode.child("scene").child("chest");
                        chest->position = iPoint(pos.x + 16, pos.y + 16);
                      
                    }

                    //espina que se rompe
                    if (gid == tileset->firstgid + 2) {
                        PlantBreakable* entity = (PlantBreakable*)app->entityManager->CreateEntity(EntityType::PLANT_BREAKABLE);
                        entity->parameters = configNode.child("scene").child("plant_breakable");
                        entity->position = iPoint(pos.x + 16, pos.y + 16);
                    }

                    //barrera de espinas
                    if (gid == tileset->firstgid + 3) {
                        PlantBarrier* entity = (PlantBarrier*)app->entityManager->CreateEntity(EntityType::PLANT_BARRIER);
                        entity->parameters = configNode.child("scene").child("plant_barrier");
                        entity->position = iPoint(pos.x, pos.y + 16);
                    }

                    //player
                    if (gid == tileset->firstgid + 4) {

                        app->scene->setPlayer((Player*)app->entityManager->CreateEntity(EntityType::PLAYER));
                        app->scene->getPlayer()->parameters = configNode.child("scene").child("player");
                        app->scene->getPlayer()->position = iPoint(pos.x + 16, pos.y + 16);
                        
                    }


                }
            }

        }
        mapLayerItem = mapLayerItem->next;

    }




    return false;
}

Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}




