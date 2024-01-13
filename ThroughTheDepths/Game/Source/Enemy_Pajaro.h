#ifndef __ENEMY_PAJARO_H__
#define __ENEMY_PAJARO_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Timer.h"
#include "Animation.h"
#include "DynArray.h"
#include "Pathfinding.h"

struct SDL_Texture;

class EnemyPajaro : public Entity
{
public:

	EnemyPajaro();

	virtual ~EnemyPajaro();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnExitCollision(PhysBody* physA, PhysBody* physB);

	void Movement(float dt);
	void Dying();
	void AnimationState();

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;




	/*NEW JUMPING SYSTEM*/
	float jumpForce = 4.5f;
	bool canJump = true;
	int numFootContacts = 0;


	bool isFacingLeft = false;
	bool goingUp = false;

	bool traspassingColision = false;


	Animation* currentAnimation = nullptr;

	int palomaAtaque;
	int muertePaloma;
	int volarPaloma;
private:

	b2Transform startTransform;




	Animation runAnim;
	Animation flyAnim;
	Animation walkAnim;
	Animation idleAnim;
	Animation jumpUpAnim;
	Animation jumpDownAnim;
	Animation ghostAnim;
	Animation dieAnim;
	Animation attackAnim;




	bool isDying = false;
	bool isAttacking = false;
	int lastPosY;
	bool goToPath = false;
	bool cansado = false;
	float walkSpeed, runSpeed, attackSpeed;

	Timer cansadoTimer;
	Timer goingUpTimer;
	int randomGoingUpTimer;

	DynArray<iPoint> lastPath;

	PathFinding* pathfinding;

};

#endif // __ENEMY_PAJARO_H__