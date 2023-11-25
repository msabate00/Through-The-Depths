#ifndef __ENEMY_ARMADILLO_H__
#define __ENEMY_ARMADILLO_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Timer.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

class EnemyArmadillo : public Entity
{
public:

	EnemyArmadillo();

	virtual ~EnemyArmadillo();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnExitCollision(PhysBody* physA, PhysBody* physB);

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;




	/*NEW JUMPING SYSTEM*/
	float jumpForce = 4.5f;
	bool canJump = true;
	int numFootContacts = 0;


	bool isFacingLeft = false;
	bool traspassingColision = false;


	Animation* currentAnimation = nullptr;

private:

	b2Transform startTransform;




	Animation runAnim;
	Animation walkAnim;
	Animation idleAnim;
	Animation jumpUpAnim;
	Animation jumpDownAnim;
	Animation ghostAnim;
	Animation dieAnim;
	Animation attackAnim;
	Animation attackLoopAnim;
	Animation trackAnim;


	bool isDying = false;
	bool isAttacking = false;
	bool goToPath = false;

	float walkSpeed, runSpeed;

	DynArray<iPoint> lastPath;

};

#endif // __ENEMY_ARMADILLO_H__