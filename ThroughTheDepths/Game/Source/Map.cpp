
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
#include "Enemy_Armadillo.h"
#include "SaveStatue.h"

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

    mapFileName_Bosque = config.child("mapBosque").attribute("path").as_string(); 
    mapFileName_Pueblo = config.child("mapPueblo").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    tilePathTexRedPath = config.child("tilePathTexRed").attribute("path").as_string();
    tilePathTexBrownPath = config.child("tilePathTexBrown").attribute("path").as_string();

    return ret;
}

bool Map::Start()
{
   
   /* collisionsListCount = 0;*/
    switch (app->sceneLevel)
    {
    case 0:  mapFileName = mapFileName_Bosque; break;
    case 1:  mapFileName = mapFileName_Pueblo; break;
    default:  mapFileName = mapFileName_Bosque; break;
    }

    return true;
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
            mapLayerItem->data->properties.GetProperty("Front") != NULL && !mapLayerItem->data->properties.GetProperty("Front")->value) {


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


                   
                    SDL_SetTextureColorMod(tileset->texture, 255 * mapLayerItem->data->opacity, 255 * mapLayerItem->data->opacity, 255 * mapLayerItem->data->opacity);
                    

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

                    SDL_SetTextureColorMod(tileset->texture, 255 * mapLayerItem->data->opacity, 255 * mapLayerItem->data->opacity, 255 * mapLayerItem->data->opacity);

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

    // L09: DONE 3: Get the screen coordinates of tile positions for isometric maps 
    if (mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
        ret.x = x * mapData.tileWidth;
        ret.y = y * mapData.tileHeight;
    }

    if (mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
        ret.x = x * mapData.tileWidth / 2 - y * mapData.tileWidth / 2;
        ret.y = x * mapData.tileHeight / 2 + y * mapData.tileHeight / 2;
    }

    return ret;
}

iPoint Map::WorldToMap(int x, int y) 
{
    iPoint ret(0, 0);

    if (mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
        ret.x = x / mapData.tileWidth;
        ret.y = y / mapData.tileHeight;
    }

    if (mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
        float half_width = mapData.tileWidth / 2;
        float half_height = mapData.tileHeight / 2;
        ret.x = int((x / half_width + y / half_height) / 2);
        ret.y = int((y / half_height - (x / half_width)) / 2);
    }

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
    mapData.maplayers.Clear();


    // Remove all groupobject
    ListItem<MapObjects*>* objectsItem;
    objectsItem = mapData.mapObjects.start;

    while (objectsItem != NULL)
    {
        RELEASE(objectsItem->data);
        objectsItem = objectsItem->next;
    }
    mapData.mapObjects.Clear();



    
    // Remove all colisions
    ListItem<PhysBody*>* collision;
    collision = collisionsList.start;
    while (collision != NULL) {
        app->physics->GetWorld()->DestroyBody(collision->data->body);
        collision = collision->next;
    }
    collisionsList.Clear();

    //Remove traspassed list
    traspasedPlatformList.Clear();

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


   


    if (ret == true) {
        ret = LoadNavigationLayer();
        if (!ret) { LOG("FALLO AL CARGAR EL MAPA DE NAVEGACION"); }
        else {
            pathfindingFloor = new PathFinding();
            pathfindingFly = new PathFinding();
            uchar* navigationMap = NULL;
            CreateNavigationMap(mapData.width, mapData.height, &navigationMap, navigationLayer_Floor);
            pathfindingFloor->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap);
            pathfindingFloor->tilePathTex = app->tex->Load(tilePathTexBrownPath.GetString());
            pathfindingFly->tilePathTex = app->tex->Load(tilePathTexRedPath.GetString());

            CreateNavigationMap(mapData.width, mapData.height, &navigationMap, navigationLayer_Fly);
            pathfindingFly->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap);

            RELEASE_ARRAY(navigationMap);
        }
    }
   
  



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

        SString orientationStr = map.attribute("orientation").as_string();
        if (orientationStr == "orthogonal") {
            mapData.type = MAPTYPE_ORTHOGONAL;
        }
        else if (orientationStr == "isometric") {
            mapData.type = MAPTYPE_ISOMETRIC;
        }
        else {
            LOG("Map orientation not found");
            ret = false;
        }


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
    
    if (node.attribute("opacity") != NULL) {
        layer->opacity = node.attribute("opacity").as_float();
    }
    else {
        layer->opacity = 1.0f;
    }

    if (node.attribute("tintcolor") != NULL) {
        layer->tint = node.attribute("tintcolor").as_string();
    }
    else {
        layer->tint = "#FFFFFF";
    }
   

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
                    PhysBody* c1;
                    if (gid == tileset->firstgid + 0) {
                        c1 = app->physics->CreateRectangle(pos.x+16, pos.y+16 , 32, 32, STATIC);
                        c1->ctype = ColliderType::PLATFORM;
                        collisionsList.Add(c1);
                        ret = true;
                    }
                   

                    if (gid == tileset->firstgid + 1) {
                        c1 = app->physics->CreateRectangle(pos.x + 16, pos.y+2, 32, 4, STATIC);
                        c1->ctype = ColliderType::PLATFORM_TRASPASS;
                        traspasedPlatformList.Add(c1);
                        collisionsList.Add(c1);
                        ret = true;
                    }
                    if (gid == tileset->firstgid + 6) {
                        c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
                        c1->ctype = ColliderType::SPYKES;
                        collisionsList.Add(c1);
                        ret = true;
                    }

                    //Fuera del mapa
                    if (gid == tileset->firstgid + 15) {
                        c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
                        c1->ctype = ColliderType::DIE_HOLE;
                        collisionsList.Add(c1);
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
            PhysBody* c1;
            c1 = app->physics->CreateRectangle(object->x + object->width/2, object->y + object->height/2, object->width, object->height, STATIC);
            c1->ctype = ColliderType::PLATFORM;
            collisionsList.Add(c1);
            ret = true;
        }
        mapObjectsItem = mapObjectsItem->next;
    }


    return ret;
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
    LOG("CARGA ENTIDADES");
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
                       coin->parameters = configNode.child("scene").child("textures").child("coin");
                       coin->position = iPoint(pos.x + 16, pos.y + 16);
                    }

                    //cofre con Monedas
                    if (gid == tileset->firstgid + 1) {
                        Chest* chest = (Chest*)app->entityManager->CreateEntity(EntityType::CHEST_COIN);
                        chest->parameters = configNode.child("scene").child("textures").child("chest");
                        chest->position = iPoint(pos.x + 16, pos.y + 16);
                      
                    }

                    //espina que se rompe
                    if (gid == tileset->firstgid + 2) {
                        PlantBreakable* entity = (PlantBreakable*)app->entityManager->CreateEntity(EntityType::PLANT_BREAKABLE);
                        entity->parameters = configNode.child("scene").child("textures").child("plant_breakable");
                        entity->position = iPoint(pos.x + 16, pos.y + 16);
                    }

                    //barrera de espinas
                    if (gid == tileset->firstgid + 3) {
                        PlantBarrier* entity = (PlantBarrier*)app->entityManager->CreateEntity(EntityType::PLANT_BARRIER);
                        entity->parameters = configNode.child("scene").child("textures").child("plant_barrier");
                        entity->position = iPoint(pos.x, pos.y + 16);
                    }

                    //player
                    if (gid == tileset->firstgid + 4) {

                        app->scene->setPlayer((Player*)app->entityManager->CreateEntity(EntityType::PLAYER));
                        app->scene->getPlayer()->parameters = configNode.child("scene").child("player");
                        app->scene->getPlayer()->position = iPoint(pos.x + 16, pos.y + 16);
                        
                    }

                    //Enemy armadillo
                    if (gid == tileset->firstgid + 5) {

                        EnemyArmadillo* entity = (EnemyArmadillo*)app->entityManager->CreateEntity(EntityType::ENEMY_ARMADILLO);
                        entity->parameters = configNode.child("scene").child("enemyArmadillo");
                        entity->position = iPoint(pos.x + 16, pos.y + 16);

                    }

                    //Entidad de guardar
                    if (gid == tileset->firstgid + 6) {

                        SaveStatue* entity = (SaveStatue*)app->entityManager->CreateEntity(EntityType::SAVE_STATUE);
                        entity->parameters = configNode.child("scene").child("textures").child("saveStatue");
                        entity->position = iPoint(pos.x + 16, pos.y + 16 - 32 );

                    }

                    //Enemy pajaro con cuchillo
                    if (gid == tileset->firstgid + 7) {

                        SaveStatue* entity = (SaveStatue*)app->entityManager->CreateEntity(EntityType::ENEMY_PAJARO);
                        entity->parameters = configNode.child("scene").child("enemyPajaro");
                        entity->position = iPoint(pos.x + 16, pos.y + 16 - 32);

                    }


                }
            }

        }
        mapLayerItem = mapLayerItem->next;

    }




    return false;
}




bool Map::LoadNavigationLayer() {
    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;
    navigationLayer_Floor = mapLayerItem->data;
    navigationLayer_Fly = mapLayerItem->data;
    bool ret = false;
    bool ret2 = false;

    //Search the layer in the map that contains information for navigation
    while (mapLayerItem != NULL) {
        if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value &&
            mapLayerItem->data->properties.GetProperty("Floor") != NULL && mapLayerItem->data->properties.GetProperty("Floor")->value) {

            navigationLayer_Floor = mapLayerItem->data;
            ret = true;
            break;
        }
        mapLayerItem = mapLayerItem->next;
    }

    while (mapLayerItem != NULL) {
        if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value &&
            mapLayerItem->data->properties.GetProperty("Floor") != NULL && !mapLayerItem->data->properties.GetProperty("Floor")->value) {

            navigationLayer_Fly = mapLayerItem->data;
            ret2 = true;
            break;
        }
        mapLayerItem = mapLayerItem->next;
    }

    return (ret && ret2);
}



void Map::CreateNavigationMap(int& width, int& height, uchar** buffer, MapLayer* navigationLayer) const
{
    bool ret = false;

    //Sets the size of the map. The navigation map is a unidimensional array 
    uchar* navigationMap = new uchar[navigationLayer->width * navigationLayer->height];
    //reserves the memory for the navigation map
    memset(navigationMap, 1, navigationLayer->width * navigationLayer->height);

    for (int x = 0; x < mapData.width; x++)
    {
        for (int y = 0; y < mapData.height; y++)
        {
            //i is the index of x,y coordinate in a unidimensional array that represents the navigation map
            int i = (y * navigationLayer->width) + x;

            //Gets the gid of the map in the navigation layer
            int gid = navigationLayer->Get(x, y);
            TileSet* tileset = GetTilesetFromTileId(gid);
            //If the gid is a blockedGid is an area that I cannot navigate, so is set in the navigation map as 0, all the other areas can be navigated
            //!!!! make sure that you assign blockedGid according to your map
            
            if (navigationLayer == navigationLayer_Floor) {
                if (gid == tileset->firstgid + 1) navigationMap[i] = 1;
                else navigationMap[i] = 0;
            }
            else {
                if (gid == tileset->firstgid) navigationMap[i] = 0;
                else navigationMap[i] = 1;
            }

           
        }
    }

    *buffer = navigationMap;
    width = mapData.width;
    height = mapData.height;

}

int Map::GetTileWidth() {
    return mapData.tileWidth;
}

int Map::GetTileHeight() {
    return mapData.tileHeight;
}


Properties::Property* Properties::GetProperty(const char* name)
{
    Property* p = NULL;
    if (this != nullptr) {
        ListItem<Property*>* item = list.start;
       

        while (item)
        {
            if (item->data->name == name) {
                p = item->data;
                break;
            }
            item = item->next;
        }
    }
    return p;
}




