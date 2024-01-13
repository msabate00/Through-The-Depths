
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Food : public Entity
{
public:

	Food();
	virtual ~Food();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();






public:
	int item = 0;

	

private:
	
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation pickedAnim;
	


};

