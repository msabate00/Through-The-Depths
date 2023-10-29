#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Timer.h"
#include "Animation.h"

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
	void OnExitCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 4.5f;
	float speedFast = 4.5f;
	//float accelerationY = 0;
	//float accelerationY_max = 10;
	//int jumpStep = 0;

	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	PhysBody* pbodyFoot;
	int pickCoinFxId;



	/*NEW JUMPING SYSTEM*/
	float jumpForce = 5;
	bool canJump = true;
	int numFootContacts = 0;


	bool isFacingLeft = false;
	bool traspassingColision = false;


	Animation* currentAnimation = nullptr;

private:
	
	b2Transform startTransform;

	List<PhysBody*> colisionTraspassing;
	Timer traspassingTimer;
	Timer watchUpDownTimer;
	int yCameraOffset = 0;

	

	Animation runAnim;
	Animation walkAnim;
	Animation idleAnim;
	Animation jumpUpAnim;
	Animation jumpDownAnim;
	Animation ghostAnim;
	Animation dieAnim;
	Animation attackAnim;


	bool isDying = false;
	bool isAttacking = false;


	

};

#endif // __PLAYER_H__