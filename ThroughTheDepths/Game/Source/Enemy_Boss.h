#ifndef __ENEMY_BOSS_H__
#define __ENEMY_BOSS_H__

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

class EnemyBoss : public Entity
{
public:

	EnemyBoss();

	virtual ~EnemyBoss();

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

	int ataqueBoss;
	int muerteBoss;
	int caminarBoss;

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
	Animation stopAnim;


	bool isDying = false;
	bool isAttacking = false;
	bool goToPath = false;
	bool cansado = false;
	float walkSpeed, runSpeed, attackSpeed;

	Timer cansadoTimer;

	DynArray<iPoint> lastPath;

	PathFinding* pathfinding;


};

#endif // __ENEMY_BOSS_H__