#include "EntityManager.h"
#include "Player.h"
#include "Chest.h"
#include "Coin.h"
#include "PlantBreakable.h"
#include "Enemy_Armadillo.h"
#include "PlantBarrier.h"
#include "SaveStatue.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::CHEST_COIN:
		entity = new Chest();
		break;
	case EntityType::COIN:
		entity = new Coin();
		break;
	case EntityType::PLANT_BARRIER:
		entity = new PlantBarrier();
		break;
	case EntityType::PLANT_BREAKABLE:
		entity = new PlantBreakable();
		break;
	case EntityType::SAVE_STATUE:
		entity = new SaveStatue();
		break;
	case EntityType::ENEMY_ARMADILLO:
		entity = new EnemyArmadillo();
		break;
	default:
		break;
	}

	entities.Add(entity);
	entity->Awake();
	

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) {
			entities.Del(item);
		}
	}
}

void EntityManager::DestroyAllEntities()
{
	ListItem<Entity*>* item;
	for (item = entities.start; item != NULL; item = item->next)
	{
		
		entities.Del(item);
		
	}

}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	

	return ret;
}

bool EntityManager::PostUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->PostUpdate();
	}

	app->map->UpdateFrontEntities();

	return ret;
}

void EntityManager::ApagarTodosLosSave() {

	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->type == EntityType::SAVE_STATUE) {
			((SaveStatue*)pEntity)->saved = false;
		}
		
	}
}