#ifndef __WALL_BOSS_H__
#define __WALL_BOSS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class WallBoss : public Entity
{
public:

	WallBoss();
	virtual ~WallBoss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


public:
	int item = 0;
	bool enterZone = false;

private:
	
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation appearAnim;



};
#endif // __WALL_BOSS_H__

