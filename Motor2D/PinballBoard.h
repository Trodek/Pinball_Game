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

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	p2List<kicker_info> left_kickers;
	p2List<kicker_info> right_kickers;

private:
	bool CreateBoardPhyisics();
	bool CreateStickersCollisions();
	bool CreateKickers();
	bool CreateTrigers();

private:

	iPoint offset;
	
	p2SString	default_map;

	Sprite		walls;
	Sprite		background;

	PhysBody*	ball;
	Sprite		ball_sprite;

	Sprite		yellowsticker;
	Sprite		bluesticker;
	Sprite		greysticker;

	Sprite		left_kicker;
	Sprite		right_kicker;

	Sprite		right_puncher;
	Sprite		left_puncher;

	PhysBody*	launch_triger;

};

#endif // !_PinballBoard_

