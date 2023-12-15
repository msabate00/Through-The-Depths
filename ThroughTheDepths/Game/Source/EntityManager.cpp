#include "EntityManager.h"
#include "Player.h"
#include "Chest.h"
#include "Coin.h"
#include "PlantBreakable.h"
#include "Enemy_Armadillo.h"
#include "Enemy_Pajaro.h"
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
	coins.Clear();

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
		coins.Add(entity);
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
	case EntityType::ENEMY_PAJARO:
		entity = new EnemyPajaro();
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

void EntityManager::DestroyAllCoinsToDestroy()
{
	ListItem<Entity*>* item;
	for (item = coins.start; item != NULL; item = item->next)
	{
		ListItem<iPoint>* destoyCoin;
		for (destoyCoin = coins_to_destroyPos.start; destoyCoin != NULL; destoyCoin = destoyCoin->next) {


			if(app->map->WorldToMap(destoyCoin->data.x, destoyCoin->data.y) == app->map->WorldToMap(item->data->position.x, item->data->position.y)) {

			
				item->data->active = false;
				/*entities.Del(item);
				coins.Del(item);
				coins_to_destroyPos.Del(destoyCoin);*/
			}
		}
		
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
			((SaveStatue*)pEntity)->savedAnim.Reset();
		}
		
	}
}