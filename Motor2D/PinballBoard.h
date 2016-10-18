#ifndef _PinballBoard_
#define _PinballBoard_

#include "j1Module.h"

struct Sprite;
class PhysBody;

class PinballBoard : public j1Module {
public:

	PinballBoard();

	// Destructor
	virtual ~PinballBoard();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool Draw();

	// Called before quitting
	bool CleanUp();

private:
	bool CreateBoardPhyisics();
	bool CreateStickersCollisions();

private:

	p2SString	default_map;

	Sprite		walls;
	Sprite		background;

	PhysBody*	ball;
	Sprite		ball_sprite;

	Sprite		yellowsticker;
	Sprite		bluesticker;
	Sprite		greysticker;
};

#endif // !_PinballBoard_

