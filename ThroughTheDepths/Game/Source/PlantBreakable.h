
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class PlantBreakable : public Entity
{
public:

	PlantBreakable();
	virtual ~PlantBreakable();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);






public:
	int item = 0;

	

private:
	
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation damagedAnim;

	bool isDamaged = false;

	int life = 3;
	


};

