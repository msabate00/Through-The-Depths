
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class SaveStatue : public Entity
{
public:

	SaveStatue();
	virtual ~SaveStatue();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();






public:

	bool saved = false;
	Animation savedAnim;

private:
	
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	
	


};

