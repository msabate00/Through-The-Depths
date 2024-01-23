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

	void Movement(float dt);
	void GodModeMovement(float dt);
	void CameraMovement(float dt);
	void BossCameraMovement(float dt);
	void DebugCameraMovement(float dt);

	void AudioController();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnExitCollision(PhysBody* physA, PhysBody* physB);

	void SetPosition(int x, int y);

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



	int pasoCesped1;
	int pasoCesped2;
	int pasoCesped3;
	int pasoCesped4;
	int pasoCesped5;
	int pasoCesped6;
	int pasoCesped7;

	int pasoMetal;
	int pasoRoca;
	int caidaMuerte;
	int recibirAtaque;
	int ataqueEspada;
	int saltoJugador;
	int monedaSonido;

	int sonidoDeVictoria;

	/*int musicaBosque;*/

	Timer pasosTimer;
	float pasosTime = 400;

	/*NEW JUMPING SYSTEM*/
	float jumpForce = 4.5f;
	bool canJump = true;
	int numFootContacts = 0;


	bool isFacingLeft = false;
	bool traspassingColision = false;


	Animation* currentAnimation = nullptr;

	int health = 5;

private:
	
	b2Transform startTransform;

	List<PhysBody*> colisionTraspassing;
	Timer traspassingTimer;
	Timer watchUpDownTimer;
	Timer invulnerableTimer;
	float invulnerableTime;
	int yCameraOffset = 0;

	
	Animation lluviaAnim;
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
	bool isDyingandRespawning = false;

	

};

#endif // __PLAYER_H__