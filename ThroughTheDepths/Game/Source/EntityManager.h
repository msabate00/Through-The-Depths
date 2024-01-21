#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"
#include "wallBoss.h"
#include "Enemy_Boss.h"


class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called every frame
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);
	void DestroyAllEntities();
	void DestroyAllCoinsToDestroy();
	void DestroyAllEnemiesToDestroy();

	void AddEntity(Entity* entity);


	void ApagarTodosLosSave();

public:

	List<Entity*> entities;
	List<Entity*> coins;
	List<Entity*> enemies;
	List<iPoint> coins_to_destroyPos;
	List<iPoint> enemies_to_destroyPos;
	int current_id = 0;

	WallBoss* wallBoss;
	EnemyBoss* boss;

};

#endif // __ENTITYMANAGER_H__
