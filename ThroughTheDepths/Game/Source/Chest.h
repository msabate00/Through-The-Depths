
#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Chest : public Entity
{
public:

	enum class CHEST_STATE {
		CLOSED,
		OPENING,
		OPENED
	};

	Chest();
	virtual ~Chest();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void ChangeState(CHEST_STATE state);




public:

	bool isOpened = false;
	int item = 0;

	

private:

	CHEST_STATE state = CHEST_STATE::CLOSED;
	
	SDL_Texture* texture;
	const char* texturePath;
	PhysBody* pbody;

	Animation* currentAnimation = nullptr;
	Animation closedAnim;
	Animation openingAnim;
	Animation openedAnim;


};

