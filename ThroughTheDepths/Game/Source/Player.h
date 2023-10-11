#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Timer.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	//float accelerationY = 0;
	//float accelerationY_max = 10;
	//int jumpStep = 0;

	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;



	/*NEW JUMPING SYSTEM*/
	float jumpForce = 5;

	bool isFacingLeft = false;
	bool traspassingColision = false;

private:
	
	List<PhysBody*> colisionTraspassing;
	Timer traspassingTimer;
	

};

#endif // __PLAYER_H__