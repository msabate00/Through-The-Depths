#ifndef __ENEMY_BOSSFIREBALL_H__
#define __ENEMY_BOSSFIREBALL_H__

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

class EnemyBossFireball : public Entity
{
public:

	EnemyBossFireball();

	virtual ~EnemyBossFireball();

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


	bool activeBoss = false;
	int health = 20;
	int maxHealth = 20;

private:

	b2Transform startTransform;




	Animation idleAnim;
	Animation runAnim;
	Animation walkAnim;
	Animation attackAnim;
	Animation attackShootStartAnim;
	Animation attackShootAnim;
	Animation attackShootEndAnim;
	Animation attackJumpAnim;
	Animation dmgAnim;
	Animation dieAnim;
	Animation teleportInAnim;
	Animation teleportOutAnim;


	bool isDying = false;
	bool isAttacking = false;
	bool goToPath = false;
	bool cansado = false;
	float walkSpeed, runSpeed, attackSpeed;

	Timer cansadoTimer;

	Timer toRunTimer;
	Timer doAttackShootTimer;
	Timer attackShootDurationTimer;
	float attackShootDuration = 10;

	bool setPosicionTpRandom = false;
	bool vueltaNormal = true;
	


};

#endif // __ENEMY_BOSSFIREBALL_H__