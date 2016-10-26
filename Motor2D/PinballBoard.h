#ifndef _PinballBoard_
#define _PinballBoard_

#include "j1Module.h"

struct Sprite;
class PhysBody;
class b2RevoluteJoint;
class b2PrismaticJoint;
class b2MotorJoint;
struct path_joint;

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

	void Restart();


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
	void CreateBall();
	void DrawUI() const;
	void TeleportBall(PhysBody* tp);

private:

	uint count = 0;

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

	Sprite		right_puncher_o;
	Sprite		left_puncher_o;

	Sprite		left_puncher_y;
	Sprite		right_puncher_y;

	Sprite		teleport;

	Sprite		tunnel;

	Sprite		bonus_girl;

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
	PhysBody*	trigger_lefttube;
	PhysBody*	trigger_righttube;
	PhysBody*	trigger_lose_left;
	PhysBody*	trigger_lose_right;
	PhysBody*	trigger_top_mid_left;
	PhysBody*	trigger_top_mid_right;

	PhysBody*	to_destroy_top_mid_left;
	PhysBody*	to_destroy_top_mid_right;

	bool		lose_triggered = false;
	bool		to_create_mid_walls = false;
	bool		created_mid_walls = false;

	bool		lefttube_triggered = false;
	bool		lefttubejoint_created = true;
	path_joint*	lefttube;

	bool		righttube_triggered = false;
	bool		righttubejoint_created = true;
	path_joint* righttube;

	Sprite		pink_guy;
	Sprite		brown_web;
	Sprite		pink_web1;
	Sprite		pink_web2;
	Sprite		pink_web3;

	Sprite		bell;

	Sprite		mouth;

	Sprite		blue_flame;

	kicker_info mill;
	Sprite		mill_sprite;

	Sprite		bonus_left_letters;
	Sprite		bonus_left_frame;

	Sprite		bonus_right_letters;
	Sprite		bonus_right_frame;

	Sprite		x_arrows;
	Sprite		eat_arrows;
	Sprite		tel_marks;
	Sprite		right_eleph_marks;
	Sprite		left_eleph_marks;
	Sprite		rocket_single;
	Sprite		rocket_double;

	//UI
	Sprite		score_right;
	Sprite		score_left;
	int			score;
	int			high_score;

	Sprite		balls_left;
	Sprite		lose_ball;
	int			losed_balls = 0;

	int			orange_font;
	int			white_font;

	PhysBody*	tp_left;
	PhysBody*	tp_right;

	PhysBody*	tp_elephant_left_top;
	PhysBody*	tp_elephant_left_bot;

	PhysBody*	tp_elephant_right_top;
	PhysBody*	tp_elephant_right_bot;

	PhysBody*	to_tp = nullptr;
	PhysBody*	tp_listener;
	int			tp_aux = 0;

};

#endif // !_PinballBoard_

