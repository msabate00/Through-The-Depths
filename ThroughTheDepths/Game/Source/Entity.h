#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "Timer.h"

class PhysBody;


enum class EntityState {
	IDLE,
	RUNNING,
	ATTACKING,
	JUMPING,
	FALLING,
	TRACK,
	WALKAROUND,
	DYING

};



enum class EntityType
{
	PLAYER,
	ITEM,
	CHEST_COIN,
	COIN,
	PLANT_BARRIER,
	PLANT_BREAKABLE,
	ENEMY_ARMADILLO,
	UNKNOWN

};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PostUpdate() {
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {};
	virtual void OnExitCollision(PhysBody* physA, PhysBody* physB) {};

public:

	SString name;
	EntityType type;
	EntityState state;
	bool active = true;
	pugi::xml_node parameters; 

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	iPoint originalPosition;
	int tilesView;
	int tilesAttack;
	bool onView = false;
	bool renderable = true;
	bool isAttacking = false;

	iPoint playerPos;
	iPoint origPos;
	iPoint targPos;

	PhysBody* pbody = nullptr;

	Timer attackTimer;
	float attackTimeMax;

	float speed;

	//DynArray<iPoint>* path = nullptr;

};

#endif // __ENTITY_H__