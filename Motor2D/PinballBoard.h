#ifndef _PinballBoard_
#define _PinballBoard_

#include "j1Module.h"

struct Sprite;
class PhysBody;
class b2RevoluteJoint;
class b2PrismaticJoint;

struct kicker_info {
	PhysBody* anchor;
	PhysBody* body;
	b2RevoluteJoint* joint;
};

struct launcher_info {
	PhysBody* anchor;
	PhysBody* body;
	b2PrismaticJoint* joint;
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

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void CreateBall();

public:

	p2List<kicker_info> left_kickers;
	p2List<kicker_info> right_kickers;
	kicker_info			top_kicker;
	launcher_info		launcher;

private:
	bool CreateBoardPhyisics();
	bool CreateStickersCollisions();
	bool CreateKickers();
	bool CreateTrigers();

	void DrawUI();

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
	Sprite		top_kiker_sprite;

	Sprite		right_puncher;
	Sprite		left_puncher;

	Sprite		x_sprite;

	PhysBody*	launch_triger;
	PhysBody*	tolaunch_triger;
	Sprite		launcher_sprite;

	PhysBody*	x_righttop_toTOP;
	PhysBody*	x_lefttop_toTOP;
	PhysBody*	x_leftbot_toTOP;
	PhysBody*	x_rightbot_toTOP;
	PhysBody*	x_righttop_toBOARD;
	PhysBody*	x_lefttop_toBOARD;
	PhysBody*	x_leftbot_toBOARD;
	PhysBody*	x_rightbot_toBOARD;

	Sprite		pink_guy;
	Sprite		brown_web;
	Sprite		pink_web1;
	Sprite		pink_web2;
	Sprite		pink_web3;

	//UI
	Sprite		score_right;
	Sprite		score_left;

	int			orange_font;
	int			white_font;

	int			score;
	int			high_score;

};

#endif // !_PinballBoard_

