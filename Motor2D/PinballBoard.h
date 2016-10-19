#ifndef _PinballBoard_
#define _PinballBoard_

#include "j1Module.h"

struct Sprite;
class PhysBody;
class b2RevoluteJoint;

struct kicker_info {
	PhysBody* anchor;
	PhysBody* body;
	b2RevoluteJoint* joint;
};

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

public:

	p2List<kicker_info> left_kickers;

private:
	bool CreateBoardPhyisics();
	bool CreateStickersCollisions();
	bool CreateKickers();

private:

	p2SString	default_map;

	Sprite		walls;
	Sprite		background;

	PhysBody*	ball;
	Sprite		ball_sprite;

	Sprite		yellowsticker;
	Sprite		bluesticker;
	Sprite		greysticker;

	Sprite		left_kicker;

};

#endif // !_PinballBoard_

