#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "PinballBoard.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"

PinballBoard::PinballBoard() : j1Module()
{
	name.create("pinball");

	background.rect = walls.rect = { 0,0,590,500 };
	ball_sprite.rect = { 0,0,13,13 };
	yellowsticker.rect = { 0,0,30,31 };
	bluesticker.rect = { 0,0,37,34 };
	greysticker.rect = { 0,0,28,28 };
	right_kicker.rect  = left_kicker.rect = { 0,0,54,26 };
	right_puncher_y.rect = right_puncher_o.rect = left_puncher_o.rect = left_puncher_y.rect = { 0,0,67,79 };
	teleport.rect = { 0,0,21,21 };
	mouth.rect = { 0,0,21,21 };
	x_sprite.rect = { 0,0,211,167 };
	top_kiker_sprite.rect = { 0,0,26,54 };
	score_left.rect = score_right.rect = { 0,0,81,35 };
	pink_guy.rect = { 0,0,106,114 };
	brown_web.rect = { 0,0,155,153 };
	pink_web1.rect = pink_web2.rect = pink_web3.rect = { 0,0,141,139 };
	launcher_sprite.rect = { 0,0,29,35 };
	balls_left.rect = { 0,0,42,35 };
	lose_ball.rect = { 0,0,16,16 };
	mill_sprite.rect = { 0,0,57,13 };
	tunnel.rect = { 0,0,530,244 };
	bonus_girl.rect = { 0,0,106,80 };
	bell.rect = { 0,0,51,51 };
	blue_flame.rect = { 0,0,32,32 };
	bonus_left_frame.rect = { 0,0,112,66 };
	bonus_left_letters.rect = { 0,0,80,24 };
	bonus_right_frame.rect = { 0,0,89,50 };
	bonus_right_letters.rect = { 0,0,89,50 };
	x_arrows.rect = {0,0,172,47};
	eat_arrows.rect = {0,0,29,56};
	tel_marks.rect = {0,0,113,96};
	right_eleph_marks.rect = {0,0,134,116};
	left_eleph_marks.rect = {0,0,137,115};
	rocket_single.rect = {0,0,27,39};
	rocket_double.rect = {0,0,51,46};
}

// Destructor
PinballBoard::~PinballBoard()
{

}

// Called before render is available
bool PinballBoard::Awake(pugi::xml_node& node)
{
	LOG("Loading Pinball Board");
	bool ret = true;

	offset.x = node.child("offset").attribute("x").as_int(0);
	offset.y = node.child("offset").attribute("y").as_int(0);

	return ret;
}

// Called before the first frame
bool PinballBoard::Start()
{
	CreateBoardPhyisics();
	CreateStickersCollisions();
	CreateKickers();
	CreateTrigers();

	walls.image = App->tex->Load("Sprites/Walls.png");
	background.image = App->tex->Load("Sprites/background.png");

	ball_sprite.image = App->tex->Load("Sprites/ball.png");
	CreateBall();

	yellowsticker.image = App->tex->Load("Sprites/yellowsticker.png");
	greysticker.image = App->tex->Load("Sprites/greysticker.png");
	bluesticker.image = App->tex->Load("Sprites/bluesticker.png");

	left_kicker.image = App->tex->Load("Sprites/left_kicker.png");
	right_kicker.image = App->tex->Load("Sprites/right_kicker.png");
	top_kiker_sprite.image = App->tex->Load("Sprites/top_kicker.png");

	right_puncher_o.image = App->tex->Load("Sprites/right_puncher_o.png");
	left_puncher_o.image = App->tex->Load("Sprites/left_puncher_o.png");

	right_puncher_y.image = App->tex->Load("Sprites/right_puncher_y.png");
	left_puncher_y.image = App->tex->Load("Sprites/left_puncher_y.png");

	x_sprite.image = App->tex->Load("Sprites/midle_x.png");

	score_left.image = App->tex->Load("Sprites/score_left.png");
	score_right.image = App->tex->Load("Sprites/score_right.png");

	orange_font = App->fonts->Load("Sprites/orange_font.png", " .!'0123456789ABCEFGHILNPRSTUabcdefghiklnoprstuvy   ", 2);
	white_font = App->fonts->Load("Sprites/white_font.png", " .!'0123456789ABCEFGHILNPRSTUabcdefghiklnoprstuvy   ", 2);
	score = 0;
	high_score = 0;

	pink_guy.image = App->tex->Load("Sprites/pink_guy.png");
	brown_web.image = App->tex->Load("Sprites/brown_web.png");
	pink_web1.image = App->tex->Load("Sprites/pink_web1.png");
	pink_web2.image = App->tex->Load("Sprites/pink_web2.png");
	pink_web3.image = App->tex->Load("Sprites/pink_web3.png");

	blue_flame.image = App->tex->Load("Sprites/blue_flame.png");

	bonus_girl.image = App->tex->Load("Sprites/bonus_girl.png");

	launcher_sprite.image = App->tex->Load("Sprites/launcher.png");

	balls_left.image = App->tex->Load("Sprites/balls_ui.png");
	lose_ball.image = App->tex->Load("Sprites/miss_ball.png");

	teleport.image = App->tex->Load("Sprites/teleport.png");

	tunnel.image = App->tex->Load("Sprites/tunnel.png");

	mill_sprite.image = App->tex->Load("Sprites/mill.png");

	mouth.image = App->tex->Load("Sprites/mouth.png");

	bell.image = App->tex->Load("Sprites/bell.png");

	bonus_left_frame.image = App->tex->Load("Sprites/bonus left frame.png");
	bonus_left_letters.image = App->tex->Load("Sprites/bonus_left_leters.png");

	bonus_right_frame.image = App->tex->Load("Sprites/bonus right frame.png");
	bonus_right_letters.image = App->tex->Load("Sprites/bonus_right_leters.png");

	x_arrows.image = App->tex->Load("Sprites/x_arrows.png");
	eat_arrows.image = App->tex->Load("Sprites/eat_guy_arrows.png");
	tel_marks.image = App->tex->Load("Sprites/telephone_triggers_marks.png");
	right_eleph_marks.image = App->tex->Load("Sprites/right_elephant_trigger_marks.png");
	left_eleph_marks.image = App->tex->Load("Sprites/left_elephant_trigger_marks.png");
	rocket_single.image = App->tex->Load("Sprites/rocket_trigger_mark.png");
	rocket_double.image = App->tex->Load("Sprites/rocket_trigger_mark.png");

	return true;
}


bool PinballBoard::Draw()
{
	App->render->Blit(background.image, 0, 0, &background.rect);

	App->render->Blit(brown_web.image, 80, 25, &brown_web.rect);

	//App->render->Blit(pink_web1.image, 87, 34, &pink_web1.rect);
	
	//App->render->Blit(pink_web2.image, 91, 32, &pink_web2.rect);
	
	//App->render->Blit(pink_web3.image, 80, 25, &pink_web3.rect);

	App->render->Blit(pink_guy.image, 125, 55, &pink_guy.rect);

	iPoint launch_pos;
	launcher.body->GetPosition(launch_pos.x, launch_pos.y);
	launch_pos.x -= launcher.body->width / 2;
	launch_pos.y -= launcher.body->height / 2;
	App->render->Blit(launcher_sprite.image, launch_pos.x-3, launch_pos.y, &launcher_sprite.rect);

	p2List_item<kicker_info>* lkick = left_kickers.start;
	while (lkick != NULL)
	{
		int kick_x, kick_y;
		lkick->data.anchor->GetPosition(kick_x, kick_y);
		App->render->Blit(left_kicker.image, kick_x, kick_y - left_kicker.rect.h / 4 , &left_kicker.rect, 1.0f, lkick->data.body->GetRotation(), 9, left_kicker.rect.h / 2);

		lkick = lkick->next;
	}

	p2List_item<kicker_info>* rkick = right_kickers.start;
	while (rkick != NULL)
	{
		int kick_x, kick_y;
		rkick->data.anchor->GetPosition(kick_x, kick_y);
		App->render->Blit(right_kicker.image, kick_x - 43, kick_y - 10, &right_kicker.rect, 1.0f, rkick->data.body->GetRotation(), 90, right_kicker.rect.h / 2);

		rkick = rkick->next;
	}

	App->render->Blit(bonus_left_frame.image, 122, 355, &bonus_left_frame.rect);
	App->render->Blit(bonus_right_frame.image, 367, 360, &bonus_right_frame.rect);

	// draw stuff

	App->render->Blit(bonus_right_letters.image, 367, 360, &bonus_right_letters.rect);
	App->render->Blit(bonus_left_letters.image, 137, 370, &bonus_left_letters.rect);
	App->render->Blit(bonus_girl.image, 125, 385, &bonus_girl.rect);

	App->render->Blit(teleport.image, 166, 228, &teleport.rect);
	App->render->Blit(teleport.image, 166, 276, &teleport.rect);
	App->render->Blit(teleport.image, 403, 228, &teleport.rect);
	App->render->Blit(teleport.image, 403, 276, &teleport.rect);
	App->render->Blit(teleport.image, 47, 316, &teleport.rect);
	App->render->Blit(teleport.image, 524, 316, &teleport.rect);

	int ball_x, ball_y;
	ball->GetPosition(ball_x, ball_y);
	if (ball->body->GetFixtureList()->GetFilterData().maskBits == TOP) {
		App->render->Blit(walls.image, 0, 0, &walls.rect);
		App->render->Blit(x_sprite.image, 190, 173, &x_sprite.rect);

		App->render->Blit(tunnel.image, 43, 0, &tunnel.rect);

		App->render->Blit(bluesticker.image, 274, 315, &bluesticker.rect); //blue stickers
		App->render->Blit(bluesticker.image, 256, 279, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 250, 243, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 304, 282, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 306, 244, &bluesticker.rect);
	}

	App->render->Blit(ball_sprite.image, ball_x, ball_y, &ball_sprite.rect, 1.0f, ball->GetRotation());

	if (ball->body->GetFixtureList()->GetFilterData().maskBits != TOP) {
		App->render->Blit(walls.image, 0, 0, &walls.rect);
		App->render->Blit(x_sprite.image, 190, 173, &x_sprite.rect);

		App->render->Blit(tunnel.image, 43, 0, &tunnel.rect);

		App->render->Blit(bluesticker.image, 274, 315, &bluesticker.rect); //blue stickers
		App->render->Blit(bluesticker.image, 256, 279, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 250, 243, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 304, 282, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 306, 244, &bluesticker.rect);
	}

	App->render->Blit(mill_sprite.image, 265, 380, &mill_sprite.rect, 1.0f, mill.body->GetRotation(), 57, 16);

	App->render->Blit(bell.image, 269, 158, &bell.rect);

	int kick_x, kick_y;
	top_kicker.anchor->GetPosition(kick_x, kick_y);
	App->render->Blit(top_kiker_sprite.image, kick_x-10, kick_y-4, &top_kiker_sprite.rect, 1.0f, top_kicker.body->GetRotation(), top_kiker_sprite.rect.w,top_kiker_sprite.rect.h/3);

	App->render->Blit(yellowsticker.image, 371, 89, &yellowsticker.rect);  // yellow stickers
	App->render->Blit(yellowsticker.image, 401, 126, &yellowsticker.rect);
	App->render->Blit(yellowsticker.image, 414, 84, &yellowsticker.rect);

	App->render->Blit(greysticker.image, 144, 88, &greysticker.rect);  // grey sticker

	App->render->Blit(right_puncher_y.image, 209, 374, &right_puncher_y.rect);	//yellow punchers
	App->render->Blit(left_puncher_y.image, 87, 374, &left_puncher_y.rect);

	App->render->Blit(right_puncher_o.image, 442, 372, &right_puncher_o.rect);	//orange punchers
	App->render->Blit(left_puncher_o.image, 316, 374, &left_puncher_o.rect);

	

	App->render->Blit(mouth.image, 38, 113, &mouth.rect);

	App->render->Blit(blue_flame.image, 451, 194, &blue_flame.rect);
	App->render->Blit(blue_flame.image, 463, 210, &blue_flame.rect);

	DrawUI();

	return true;
}

bool PinballBoard::Update(float dt)
{
	if (righttube_triggered) {
		if (!righttubejoint_created) {
			int path[78] = {
				491, 231,
				503, 212,
				512, 190,
				519, 163,
				519, 132,
				513, 120,
				501, 110,
				483, 104,
				475, 97,
				474, 81,
				479, 70,
				498, 61,
				507, 48,
				500, 33,
				487, 28,
				468, 26,
				455, 32,
				434, 40,
				421, 41,
				413, 32,
				406, 22,
				398, 15,
				387, 14,
				380, 16,
				376, 24,
				377, 34,
				375, 42,
				366, 45,
				357, 43,
				348, 36,
				338, 28,
				331, 26,
				324, 28,
				321, 35,
				318, 47,
				312, 54,
				299, 55,
				283, 53,
				254, 51
			};
			righttube = App->physics->CreatePathJoint(ball->body, path, 78, 40, -5);
			righttubejoint_created = true;
		}
		else {
			iPoint ball_pos;
			ball->GetPosition(ball_pos.x, ball_pos.y);
			if (righttube->cur_point < righttube->points) {
				if (count % 10 == 0) {
					righttube->cur_point++;
					righttube->joint->SetTarget(righttube->path[righttube->cur_point]);
				}
			}
			else {
				App->physics->DeleteJoint(righttube->joint);
				righttube_triggered = false;
				b2Filter fil;
				fil.categoryBits = BALL;
				fil.maskBits = BOARD;
				ball->body->GetFixtureList()->SetFilterData(fil);
				multip = 3;
				multip_start = count;
				extra_points = true;
			}
		}
	}

	if (lefttube_triggered) {
		if (!lefttubejoint_created) {
			int path[40] = {
				53, 217,
				45, 200,
				62, 175,
				58, 164,
				23, 157,
				17, 140,
				63, 115,
				65, 98,
				43, 80,
				49, 66,
				101, 64,
				110, 55,
				110, 33,
				133, 35,
				147, 52,
				160, 49,
				180, 30,
				199, 35,
				214, 49,
				252, 50
			};
			lefttube = App->physics->CreatePathJoint(ball->body, path, 40, 40,-5);
			lefttubejoint_created = true;
		}
		else {
			iPoint ball_pos;
			ball->GetPosition(ball_pos.x, ball_pos.y);
			if (lefttube->cur_point < lefttube->points) {
				if (count % 20 == 0) {
					lefttube->cur_point++;
					lefttube->joint->SetTarget(lefttube->path[lefttube->cur_point]);
				}
			}
			else {
				App->physics->DeleteJoint(lefttube->joint);
				lefttube_triggered = false;
				b2Filter fil;
				fil.categoryBits = BALL;
				fil.maskBits = BOARD;
				ball->body->GetFixtureList()->SetFilterData(fil);
				multip = 3;
				multip_start = count;
				extra_points = true;
			}
		}
	}

	if (to_create_mid_walls) {
		int lcolumn[6] = {
			295, 95,
			265, 110,
			269, 117,
		};
		to_destroy_top_mid_left = App->physics->CreateStaticChain(0, 0, lcolumn, 6);

		int rcolumn[6] = {
			300, 98,
			320, 120,
			324, 112,
		};
		to_destroy_top_mid_right = App->physics->CreateStaticChain(0, 0, rcolumn, 6);
		created_mid_walls = true;
		to_create_mid_walls = false;
	}


	if (multip_start + multip_time < count && extra_points) {
		multip = 1;
		extra_points = false;
	}
	if (to_tp != nullptr) {
		TeleportBall(to_tp);
		tp_aux = count;
		tp_listener = to_tp;
		to_tp = nullptr;
	}
	if (count - tp_aux > 25 && tp_listener != nullptr) {
		tp_listener->listener = App->pinball;
		tp_listener = nullptr;

	}

	count++;
	return true;
}

void PinballBoard::TeleportBall(PhysBody* tp)
{
	int x, y;
	tp->GetPosition(x, y);
	tp->listener = nullptr;
	ball->body->SetTransform(b2Vec2(0.02f*x, 0.02f*y), 0);
}

// Called before quitting
bool PinballBoard::CleanUp()
{
	LOG("Freeing scene");

	App->tex->UnLoad(walls.image);
	App->tex->UnLoad(background.image);
	App->tex->UnLoad(left_kicker.image);
	App->tex->UnLoad(right_kicker.image);
	App->tex->UnLoad(greysticker.image);
	App->tex->UnLoad(bluesticker.image);
	App->tex->UnLoad(yellowsticker.image);
	App->tex->UnLoad(right_puncher_o.image);
	App->tex->UnLoad(right_puncher_y.image);
	App->tex->UnLoad(left_puncher_o.image);
	App->tex->UnLoad(left_puncher_y.image);
	App->tex->UnLoad(top_kiker_sprite.image);
	App->tex->UnLoad(teleport.image);
	App->tex->UnLoad(tunnel.image);
	App->tex->UnLoad(bonus_girl.image);
	App->tex->UnLoad(mouth.image);
	App->tex->UnLoad(bell.image);
	App->tex->UnLoad(blue_flame.image);

	return true;
}

void PinballBoard::OnCollision(PhysBody * bodyA, PhysBody * bodyB)
{
	if (bodyA == launch_triger) {
		if (bodyB == ball) {
			b2Filter fil;
			fil.categoryBits = BALL;
			fil.maskBits = BOARD;
			ball->body->GetFixtureList()->SetFilterData(fil);
		}
	}
	else if (bodyA == x_lefttop_toTOP || bodyA == x_righttop_toTOP || bodyA == x_rightbot_toTOP || bodyA == x_leftbot_toTOP) {
		if (bodyB == ball) {
			b2Filter fil;
			fil.categoryBits = BALL;
			fil.maskBits = TOP;
			ball->body->GetFixtureList()->SetFilterData(fil);
		}
	}
	else if (bodyA == x_lefttop_toBOARD || bodyA == x_righttop_toBOARD || bodyA == x_rightbot_toBOARD || bodyA == x_leftbot_toBOARD) {
		if (bodyB == ball) {
			b2Filter fil;
			fil.categoryBits = BALL;
			fil.maskBits = BOARD;
			ball->body->GetFixtureList()->SetFilterData(fil);
		}
	}
	else if (bodyA == tolaunch_triger) {
		if (bodyB == ball) {
			b2Filter fil;
			fil.categoryBits = BALL;
			fil.maskBits = LAUNCH;
			ball->body->GetFixtureList()->SetFilterData(fil);
		}
	}
	else if (bodyA == trigger_lose_left || bodyA == trigger_lose_right) {
		if (bodyB == ball) {
			if (losed_balls < 3) {
				if (!lose_triggered) {
					losed_balls++;
					lose_triggered = true;
				}
				else
					Restart();
			}
		}
	}
	else if (bodyA == trigger_lefttube) {
		if (bodyB == ball) {
			b2Filter fil;
			fil.categoryBits = BALL;
			fil.maskBits = TOP;
			ball->body->GetFixtureList()->SetFilterData(fil);
			lefttube_triggered = true;
			lefttubejoint_created = false;
		}
	}
	else if (bodyA == trigger_righttube) {
		if (bodyB == ball) {
			b2Filter fil;
			fil.categoryBits = BALL;
			fil.maskBits = TOP;
			ball->body->GetFixtureList()->SetFilterData(fil);
			righttube_triggered = true;
			righttubejoint_created = false;
		}
	}
	else if (bodyA == trigger_top_mid_left || bodyA == trigger_top_mid_right) {
		if (bodyB == ball) {
			if(!created_mid_walls) to_create_mid_walls = true;
		}
	}
	else if (bodyA == tp_left) {
		if (bodyB == ball) {
			to_tp = tp_right;
		}
	}
	else if (bodyA == tp_right) {
		if (bodyB == ball) {
			to_tp = tp_left;
		}
	}
	else if (bodyA == tp_elephant_left_top) {
		if (bodyB == ball) {
			to_tp = tp_elephant_left_bot;
		}
	}
	else if (bodyA == tp_elephant_left_bot) {
		if (bodyB == ball) {
			to_tp = tp_elephant_left_top;
		}
	}
	else if (bodyA == tp_elephant_right_top) {
		if (bodyB == ball) {
			to_tp = tp_elephant_right_bot;
		}
	}
	else if (bodyA == tp_elephant_right_bot) {
		if (bodyB == ball) {
			to_tp = tp_elephant_right_top;
		}
	}
}

bool PinballBoard::CreateBoardPhyisics()
{
	int wallshape[394] = {			//Wall Shape
		135, 490,
		135, 527,
		212, 527,
		213, 483,
		213, 478,
		216, 475,
		222, 472,
		228, 469,
		231, 467,
		231, 463,
		231, 460,
		232, 457,
		235, 455,
		281, 434,
		285, 431,
		287, 428,
		290, 423,
		290, 419,
		291, 416,
		293, 414,
		295, 414,
		297, 416,
		298, 419,
		299, 423,
		300, 427,
		302, 430,
		305, 433,
		309, 435,
		352, 455,
		355, 457,
		357, 460,
		357, 466,
		360, 469,
		364, 471,
		368, 473,
		373, 476,
		375, 479,
		375, 527,
		457, 527,
		457, 490,
		458, 486,
		461, 484,
		533, 448,
		534, 372,
		524, 356,
		524, 349,
		533, 339,
		541, 336,
		546, 330,
		546, 320,
		540, 315,
		531, 315,
		524, 319,
		523, 324,
		513, 332,
		507, 328,
		528, 301,
		534, 297,
		541, 288,
		544, 280,
		544, 266,
		542, 261,
		543, 245,
		546, 231,
		547, 228,
		542, 228,
		532, 220,
		531, 215,
		524, 222,
		523, 227,
		514, 228,
		510, 225,
		506, 220,
		481, 180,
		481, 174,
		492, 160,
		499, 146,
		505, 130,
		509, 109,
		510, 90,
		506, 66,
		499, 53,
		487, 38,
		474, 28,
		458, 21,
		441, 16,
		424, 15,
		410, 15,
		396, 16,
		385, 18,
		375, 21,
		362, 25,
		350, 32,
		341, 39,
		333, 46,
		328, 53,
		325, 46,
		324, 35,
		320, 23,
		314, 17,
		305, 14,
		298, 13,
		289, 14,
		281, 18,
		274, 25,
		268, 34,
		266, 42,
		267, 50,
		263, 51,
		257, 45,
		249, 38,
		241, 31,
		230, 25,
		220, 21,
		211, 19,
		200, 16,
		190, 14,
		182, 13,
		148, 13,
		133, 17,
		121, 22,
		110, 29,
		99, 39,
		91, 49,
		85, 59,
		81, 69,
		79, 79,
		78, 86,
		78, 101,
		80, 116,
		84, 128,
		89, 137,
		96, 142,
		104, 150,
		111, 159,
		117, 168,
		122, 177,
		125, 184,
		124, 186,
		112, 193,
		100, 202,
		93, 207,
		85, 215,
		79, 221,
		76, 225,
		72, 222,
		69, 216,
		65, 205,
		62, 194,
		61, 185,
		59, 175,
		58, 165,
		58, 155,
		58, 143,
		58, 133,
		59, 124,
		58, 118,
		55, 113,
		50, 112,
		44, 112,
		39, 116,
		37, 121,
		36, 129,
		35, 142,
		35, 155,
		35, 168,
		35, 181,
		36, 197,
		39, 218,
		42, 232,
		45, 246,
		49, 257,
		54, 270,
		59, 278,
		54, 281,
		52, 285,
		85, 325,
		85, 329,
		83, 332,
		79, 330,
		72, 325,
		69, 325,
		68, 321,
		66, 317,
		60, 315,
		53, 316,
		47, 321,
		46, 326,
		49, 333,
		54, 338,
		60, 339,
		65, 345,
		69, 351,
		69, 355,
		59, 370,
		59, 448,
		133, 485
	};
	int size = 394;
	App->physics->CreateStaticChain(0, 0, wallshape, size);
	
	//everything related to top-mid
	{
		int left_top[28] = {			//The shape that is on the left of the orange guy
			259, 89,
			260, 92,
			258, 95,
			254, 100,
			247, 106,
			240, 106,
			241, 94,
			240, 83,
			238, 73,
			236, 68,
			239, 65,
			244, 65,
			250, 74,
			256, 84
		};
		size = 28;
		App->physics->CreateStaticChain(0, 0, left_top, size);

		int mid_top[124] = {				//The shape that is on the bottom of the orange guy
			303, 173,
			305, 171,
			322, 114,
			324, 111,
			328, 112,
			334, 117,
			340, 120,
			347, 123,
			356, 124,
			361, 126,
			364, 129,
			367, 136,
			371, 142,
			377, 148,
			384, 154,
			391, 157,
			395, 160,
			397, 163,
			397, 167,
			395, 170,
			392, 174,
			388, 176,
			384, 178,
			366, 179,
			358, 180,
			347, 181,
			336, 183,
			326, 185,
			315, 188,
			305, 191,
			299, 194,
			296, 196,
			292, 194,
			284, 191,
			275, 188,
			265, 185,
			256, 183,
			244, 181,
			233, 180,
			221, 179,
			208, 179,
			203, 178,
			199, 176,
			197, 174,
			197, 169,
			202, 165,
			207, 161,
			212, 157,
			217, 153,
			222, 147,
			227, 140,
			231, 134,
			234, 127,
			235, 123,
			239, 122,
			246, 121,
			252, 118,
			259, 114,
			262, 111,
			265, 110,
			267, 112,
			284, 172
		};
		size = 124;
		App->physics->CreateStaticChain(0, 0, mid_top, size);  // Points need revision

		int right_top[28] = {					//The shape that is next to the right foot of the orange guy
			329, 89,
			350, 55,
			354, 53,
			357, 54,
			359, 58,
			361, 67,
			361, 76,
			357, 91,
			359, 105,
			360, 108,
			355, 109,
			344, 105,
			336, 100,
			330, 95
		};
		size = 28;
		App->physics->CreateStaticChain(0, 0, right_top, size);
	}

	int x_triangle[4] = {
		22,25,
		58,6
	};
	size = 4;
	App->physics->CreateStaticChain(190, 173, x_triangle, size, 0.0f, BOARD);

	int x_triangle_right[4] = {
		58,25,
		22, 6
	};
	size = 4;
	App->physics->CreateStaticChain(318, 173, x_triangle_right, size, 0.0f, BOARD);

	//elephants
	{										
		int left_elephant[122] = {					
			167, 234,
			164, 237,
			166, 244,
			171, 249,
			177, 251,
			183, 248,
			186, 244,
			187, 238,
			185, 235,
			185, 231,
			188, 229,
			201, 229,
			209, 231,
			217, 234,
			223, 238,
			227, 243,
			231, 250,
			235, 259,
			239, 269,
			242, 279,
			244, 288,
			244, 306,
			246, 316,
			249, 324,
			252, 330,
			258, 337,
			264, 343,
			272, 348,
			281, 352,
			283, 357,
			282, 362,
			277, 363,
			268, 360,
			254, 351,
			244, 340,
			238, 329,
			234, 317,
			232, 297,
			232, 287,
			229, 279,
			223, 273,
			217, 270,
			211, 270,
			205, 273,
			197, 280,
			194, 284,
			186, 284,
			185, 279,
			180, 275,
			175, 274,
			170, 276,
			166, 279,
			165, 284,
			161, 283,
			145, 257,
			144, 248,
			148, 237,
			154, 231,
			158, 229,
			165, 229,
			168, 232
		};
		size = 122;
		App->physics->CreateStaticChain(0, 0, left_elephant, size);

		int right_elephant[110] = {
			404, 232,
			396, 230,
			386, 231,
			377, 233,
			370, 237,
			363, 243,
			359, 251,
			354, 262,
			350, 273,
			347, 284,
			346, 294,
			346, 307,
			345, 315,
			341, 326,
			335, 336,
			326, 344,
			317, 349,
			310, 352,
			308, 357,
			312, 362,
			324, 358,
			333, 352,
			342, 344,
			346, 336,
			351, 327,
			353, 316,
			356, 305,
			357, 295,
			358, 285,
			359, 281,
			364, 274,
			373, 270,
			381, 270,
			388, 273,
			394, 280,
			402, 284,
			405, 278,
			410, 274,
			418, 274,
			424, 278,
			425, 283,
			428, 283,
			445, 255,
			446, 251,
			444, 243,
			440, 235,
			434, 230,
			426, 230,
			424, 232,
			426, 239,
			423, 247,
			418, 251,
			411, 251,
			404, 246,
			402, 239
		};
		size = 110;
		App->physics->CreateStaticChain(0, 0, right_elephant, size);
	}				

	//Angles that are next to the kickers
	{	
		int left_bot[26] = {
			73, 378,
			75, 377,
			78, 376,
			80, 379,
			80, 430,
			82, 434,
			127, 455,
			129, 458,
			127, 462,
			124, 462,
			77, 440,
			74, 438,
			73, 435
		};
		size = 26;
		App->physics->CreateStaticChain(0, 0, left_bot, size);

		int right_bot[24] = {
			511, 379,
			513, 377,
			516, 376,
			518, 379,
			518, 435,
			516, 439,
			469, 461,
			465, 461,
			462, 459,
			464, 455,
			510, 433,
			511, 431
		};
		size = 24;
		App->physics->CreateStaticChain(0, 0, right_bot, size);
	}

	lefttube_triggered = false;

	//right separators between right foot and telephone
	{										
		int right_leftseparator[16] = {			//The shape that is next to the right_top
			375, 50,
			375, 71,
			378, 74,
			381, 74,
			383, 71,
			383, 50,
			381, 47,
			377, 48
		};
		size = 16;
		App->physics->CreateStaticChain(0, 0, right_leftseparator, size);

		int right_midseparator[16] = {
			396, 44,
			398, 40,
			401, 41,
			403, 44,
			403, 64,
			401, 66,
			398, 66,
			396, 64
		};
		App->physics->CreateStaticChain(0, 0, right_midseparator, size);

		int right_rightseparator[16] = {
			416, 39,
			418, 37,
			422, 36,
			424, 39,
			424, 58,
			422, 61,
			418, 61,
			416, 58
		};
		App->physics->CreateStaticChain(0, 0, right_rightseparator, size);
	}						

	//telephone
	int telephone[48] = {
		437, 34,
		451, 34,
		461, 39,
		470, 46,
		478, 55,
		484, 65,
		489, 76,
		491, 88,
		491, 110,
		487, 124,
		481, 137,
		474, 148,
		465, 158,
		454, 163,
		443, 161,
		450, 129,
		470, 128,
		473, 119,
		474, 106,
		472, 91,
		468, 76,
		461, 67,
		442, 67,
		437, 46
	};
	size = 48;
	App->physics->CreateStaticChain(0, 0, telephone, size);

	//"Punchers" <- Things that are next to the kickers that punch the ball
	{
		int left_leftpuncher[20] = {
			98, 385,
			97, 422,
			99, 427,
			132, 443,
			135, 442,
			138, 439,
			138, 434,
			108, 383,
			105, 380,
			101, 380
		};
		size = 20;
		App->physics->CreateStaticChain(0, 0, left_leftpuncher, size);

		int left_rightpuncher[20]{
			261, 386,
			261, 422,
			259, 428,
			224, 444,
			221, 443,
			218, 440,
			218, 435,
			249, 386,
			252, 381,
			257, 381
		};
		App->physics->CreateStaticChain(0, 0, left_rightpuncher, size);

		int right_leftpuncher[20]{
			327, 387,
			327, 423,
			329, 429,
			364, 445,
			367, 444,
			369, 441,
			369, 436,
			338, 387,
			335, 382,
			330, 382
		};
		App->physics->CreateStaticChain(0, 0, right_leftpuncher, size);

		int right_rightpuncher[20]{
			494, 385,
			495, 421,
			493, 426,
			460, 442,
			457, 441,
			454, 438,
			454, 433,
			484, 382,
			487, 379,
			491, 379
		};
		App->physics->CreateStaticChain(0, 0, right_rightpuncher, size);

	}

	int launcher_tub[20] = {
		281, 160,
		281, 490,
		307, 490,
		307, 424,
		307, 201,
		307, 160,
		309, 160,
		309, 493,
		279, 493,
		279, 160
	};
	size = 20;
	App->physics->CreateStaticChain(0, 0, launcher_tub, size, 0.0f, LAUNCH, BALL);


	int tunnel[340] = {
		41, 215,
		37, 206,
		37, 196,
		47, 184,
		54, 176,
		53, 169,
		43, 168,
		27, 167,
		17, 162,
		11, 152,
		10, 140,
		16, 131,
		31, 121,
		49, 114,
		58, 108,
		58, 101,
		51, 95,
		40, 87,
		35, 82,
		37, 70,
		47, 59,
		64, 55,
		83, 57,
		101, 55,
		102, 44,
		102, 33,
		111, 25,
		126, 24,
		139, 28,
		148, 39,
		151, 43,
		157, 42,
		168, 29,
		179, 23,
		192, 23,
		209, 32,
		215, 40,
		224, 43,
		230, 28,
		246, 15,
		265, 17,
		278, 29,
		283, 45,
		295, 48,
		308, 47,
		313, 35,
		319, 22,
		326, 19,
		335, 19,
		346, 25,
		360, 36,
		369, 37,
		369, 19,
		373, 12,
		382, 7,
		400, 7,
		411, 13,
		419, 25,
		424, 32,
		434, 33,
		461, 20,
		473, 19,
		487, 21,
		502, 27,
		511, 35,
		515, 47,
		513, 56,
		507, 65,
		499, 70,
		490, 73,
		482, 79,
		481, 91,
		488, 98,
		500, 101,
		511, 107,
		519, 118,
		524, 129,
		525, 141,
		526, 158,
		523, 175,
		518, 191,
		513, 206,
		506, 220,
		500, 231,
		525, 290,
		423, 239,
		485, 226,
		492, 215,
		499, 205,
		503, 193,
		507, 180,
		511, 166,
		513, 149,
		512, 135,
		507, 123,
		497, 116,
		484, 113,
		473, 106,
		467, 96,
		466, 82,
		470, 68,
		486, 57,
		496, 53,
		500, 47,
		490, 37,
		475, 33,
		465, 35,
		447, 44,
		431, 49,
		420, 48,
		408, 35,
		400, 26,
		392, 21,
		385, 22,
		385, 40,
		382, 47,
		375, 52,
		358, 53,
		347, 46,
		338, 38,
		332, 33,
		329, 34,
		327, 47,
		320, 58,
		309, 63,
		297, 63,
		281, 61,
		269, 79,
		237, 81,
		224, 58,
		213, 56,
		206, 52,
		199, 45,
		191, 38,
		181, 38,
		172, 48,
		165, 55,
		154, 59,
		145, 58,
		135, 49,
		130, 41,
		119, 40,
		118, 57,
		113, 67,
		102, 72,
		87, 73,
		72, 72,
		62, 70,
		53, 74,
		56, 79,
		68, 88,
		73, 97,
		75, 108,
		69, 118,
		59, 126,
		44, 134,
		30, 140,
		26, 146,
		30, 151,
		48, 152,
		61, 155,
		68, 161,
		71, 173,
		67, 186,
		61, 193,
		53, 201,
		54, 206,
		61, 211,
		122, 221,
		35, 278
	};
	size = 340;
	App->physics->CreateStaticChain(43, 0, tunnel, size, 0.0f, TOP, BALL);



	// Middle X
	{
		int midle_x[112] = {
			21, 6,
			42, 6,
			54, 7,
			66, 9,
			76, 11,
			86, 14,
			97, 18,
			105, 21,
			109, 21,
			113, 18,
			125, 14,
			135, 11,
			148, 8,
			161, 6,
			173, 5,
			234, 5,
			234, 35,
			188, 25,
			181, 26,
			168, 31,
			145, 42,
			145, 44,
			150, 47,
			161, 58,
			173, 69,
			185, 81,
			193, 91,
			205, 108,
			266, 117,
			164, 221,
			160, 154,
			146, 128,
			137, 113,
			124, 94,
			109, 75,
			105, 74,
			95, 85,
			84, 100,
			76, 112,
			65, 130,
			51, 158,
			48, 220,
			-53, 118,
			2, 109,
			23, 85,
			38, 70,
			51, 57,
			67, 44,
			67, 42,
			62, 39,
			50, 33,
			40, 29,
			31, 27,
			22, 25,
			-18, 30,
			-18, 4
		};
		size = 112;
		App->physics->CreateStaticChain(190, 173, midle_x, size, 0.0f, TOP, BALL);
	}
	return true;
}

bool PinballBoard::CreateStickersCollisions()
{
	App->physics->CreateStaticCircle(386, 102, 10, 1.025f); //yellow stickers
	App->physics->CreateStaticCircle(429, 97, 10, 1.025f);
	App->physics->CreateStaticCircle(416, 139, 10, 1.025f);

	App->physics->CreateStaticCircle(158, 102, 10, 1.025f); // grey sticker

	App->physics->CreateStaticCircle(323, 259, 10, 1.025f); //blue stickers
	App->physics->CreateStaticCircle(267, 258, 10, 1.025f);
	App->physics->CreateStaticCircle(273, 294, 10, 1.025f);
	App->physics->CreateStaticCircle(291, 330, 10, 1.025f);
	App->physics->CreateStaticCircle(321, 297, 10, 1.025f);
	return true;
}

bool PinballBoard::CreateKickers()
{
	kicker_info kick;
	kick.anchor = App->physics->CreateStaticCircle(128, 462, 5, 0.0f, BOARD, BALL);   // left bottom left kicker
	int left_kicker_points[8] = {
		8, 8,
		8, 17,
		52, 17,
		52, 14
	};
	int size = 8;
	kick.body = App->physics->CreatePolygon(128, 462, left_kicker_points, size, 0.0f, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 8,13 }, true, 25, -25, true, 30, 40);
	left_kickers.add(kick);

	kick.anchor = App->physics->CreateStaticCircle(58, 285, 5, 0.0f, BOARD, BALL);		//left mid kicker
	kick.body = App->physics->CreatePolygon(58, 285, left_kicker_points, size, 0.0f, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 8,13 }, true, 45, -5, true, 30, 40);
	left_kickers.add(kick);

	kick.anchor = App->physics->CreateStaticCircle(358, 462, 5, 0.0f, BOARD, BALL);		//right bottom left kicker
	kick.body = App->physics->CreatePolygon(358, 462, left_kicker_points, size, 0.0f, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 8,13 }, true, 25, -25, true, 30, 40);
	left_kickers.add(kick);

	int right_kicker_points[8] = {
		50, 8,
		50, 17,
		6, 17,
		6, 14
	};
	kick.anchor = App->physics->CreateStaticCircle(228, 464, 5, 0.0f, BOARD, BALL);		//left bottom right kicker
	kick.body = App->physics->CreatePolygon(228, 464, right_kicker_points, size, 0.0f, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 50,13 }, true, 25, -25, true, -30, 40);
	right_kickers.add(kick);

	kick.anchor = App->physics->CreateStaticCircle(540, 282, 5, 0.0f, BOARD, BALL);		//right mid kicker
	kick.body = App->physics->CreatePolygon(540, 282, right_kicker_points, size, 0.0f, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 50,13 }, true, 5, -45, true, -30, 40);
	right_kickers.add(kick);

	kick.anchor = App->physics->CreateStaticCircle(461, 462, 5, 0.0f, BOARD, BALL);		//right bottom right kicker
	kick.body = App->physics->CreatePolygon(461, 462, right_kicker_points, size, 0.0f, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 50,13 }, true, 25, -25, true, -30, 40);
	right_kickers.add(kick);

	int top_kicker_points[10] = {
		9, 7,
		12, 49,
		14, 50,
		15, 48,
		17, 7
	};
	size = 10;
	kick.anchor = App->physics->CreateStaticCircle(295, 98, 5, 0.0f, BOARD, BALL);
	kick.body = App->physics->CreatePolygon(295, 98, top_kicker_points, size, 0.0f, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 13,8 }, true, 20, -20, true, 30, 40);
	top_kicker = kick;
	
	launcher.anchor = App->physics->CreateStaticRectangle(294, 486, 5, 5);
	launcher.body = App->physics->CreateRectangle(294, 480, 20, 4, 0.0f, LAUNCH);
	launcher.joint = App->physics->CreatePrismaticJoint(launcher.anchor, launcher.body, { 0,0 }, { 0,0 }, true, 1, -30, true, -10, 5);

	mill.anchor = App->physics->CreateStaticCircle(295, 388, 5, 0.0f, BOARD, BALL);
	mill.body = App->physics->CreateRectangle(295, 338, 46, 4, 0.0f);
	kick.joint = App->physics->CreateRevoluteJoint(mill.anchor, mill.body, { 0,0 }, { 0,0 }, false, 0, 0, true, 2, 2);

	return true;
}

bool PinballBoard::CreateTrigers()
{
	launch_triger = App->physics->CreateRectangleSensor(295, 152, 32, 3, 0.0f, LAUNCH);
	launch_triger->listener = App->pinball;

	tolaunch_triger = App->physics->CreateRectangleSensor(295, 157, 32, 3);
	tolaunch_triger->listener = App->pinball;

	x_lefttop_toTOP = App->physics->CreateRectangleSensor(216, 189, 3, 10);
	x_lefttop_toTOP->listener = App->pinball;
	x_lefttop_toBOARD = App->physics->CreateRectangleSensor(210, 189, 3, 10, 0.0f, TOP);
	x_lefttop_toBOARD->listener = App->pinball;

	x_righttop_toTOP = App->physics->CreateRectangleSensor(372, 189, 3, 10);
	x_righttop_toTOP->listener = App->pinball;
	x_righttop_toBOARD = App->physics->CreateRectangleSensor(378, 189, 3, 10, 0.0f, TOP);
	x_righttop_toBOARD->listener = App->pinball;

	x_rightbot_toTOP = App->physics->CreateRectangleSensor(377, 312, 3, 70, 0.0f, BOARD, BALL, 40);
	x_rightbot_toTOP->listener = App->pinball;
	x_rightbot_toBOARD = App->physics->CreateRectangleSensor(381, 319, 3, 80, 0.0f, TOP, BALL, 40);
	x_rightbot_toBOARD->listener = App->pinball;

	x_leftbot_toTOP = App->physics->CreateRectangleSensor(210, 313, 3, 70, 0.0f, BOARD, BALL, -40);
	x_leftbot_toTOP->listener = App->pinball;
	x_leftbot_toBOARD = App->physics->CreateRectangleSensor(206, 319, 3, 90, 0.0f, TOP, BALL, -40);
	x_leftbot_toBOARD->listener = App->pinball;

	trigger_lefttube = App->physics->CreateRectangleSensor(90, 212, 3, 10, 0.0f, BOARD, BALL, 39);
	trigger_lefttube->listener = App->pinball;

	trigger_righttube = App->physics->CreateRectangleSensor(535, 230, 3, 15, 0.0f, BOARD, BALL, -45);
	trigger_righttube->listener = App->pinball;

	trigger_top_mid_left = App->physics->CreateRectangleSensor(262, 92, 3, 30, 0.0f, BOARD, BALL, 25);
	trigger_top_mid_left->listener = App->pinball;

	trigger_top_mid_right = App->physics->CreateRectangleSensor(325, 92, 3, 30, 0.0f, BOARD, BALL, -25);
	trigger_top_mid_right->listener = App->pinball;

	trigger_lose_left = App->physics->CreateRectangleSensor(170, 520, 90, 5, 0.0f, BOARD, BALL);
	trigger_lose_left->listener = App->pinball;

	trigger_lose_right = App->physics->CreateRectangleSensor(415, 520, 90, 5, 0.0f, BOARD, BALL);
	trigger_lose_right->listener = App->pinball;

	tp_left = App->physics->CreateRectangleSensor(57, 326, 5, 5);
	tp_left->listener = App->pinball;
	
	tp_right = App->physics->CreateRectangleSensor(534, 326, 5, 5);
	tp_right->listener = App->pinball;

	tp_elephant_left_top = App->physics->CreateRectangleSensor(176, 288, 5, 5);
	tp_elephant_left_top->listener = App->pinball;

	tp_elephant_left_bot = App->physics->CreateRectangleSensor(176, 238, 5, 5);
	tp_elephant_left_bot->listener = App->pinball;

	tp_elephant_right_top = App->physics->CreateRectangleSensor(413, 238, 5, 5);
	tp_elephant_right_top->listener = App->pinball;

	tp_elephant_right_bot = App->physics->CreateRectangleSensor(413, 286, 5, 5);
	tp_elephant_right_bot->listener = App->pinball;


	return true;
}

void PinballBoard::Restart()
{
	CreateBall();
	created_mid_walls = false;
	if (to_destroy_top_mid_left != nullptr) {
		App->physics->DeleteObject(to_destroy_top_mid_left);
		App->physics->DeleteObject(to_destroy_top_mid_right);
	}
	lose_triggered = false;
}


void PinballBoard::AddScore()
{
	score += multip * 50;
}

void PinballBoard::AddBonusScore()
{
	score += 750 * multip;
}


void PinballBoard::DrawUI() const
{
	App->render->Blit(score_left.image, 25, 460, &score_left.rect);
	App->render->Blit(score_right.image, 487, 460, &score_right.rect);
	char char_score[10];
	sprintf_s(char_score, "%.5d", score);
	App->fonts->Blit(27, 475, white_font, char_score);
	char char_hscore[10];
	sprintf_s(char_hscore, "%.5d", high_score);
	App->fonts->Blit(489, 475, orange_font, char_hscore);

	App->render->Blit(balls_left.image, 310, 455, &balls_left.rect);

	for (int i = 0; i < losed_balls; i++) {
		App->render->Blit(lose_ball.image, 313+i*10, 470, &lose_ball.rect);
	}
}

void PinballBoard::CreateBall()
{
	if (ball != nullptr) {
		App->physics->DeleteObject(ball);
	}
	ball = App->physics->CreateCircle(300, 440, 6, 0.0f, BALL, LAUNCH);
}