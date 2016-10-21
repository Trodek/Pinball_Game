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

PinballBoard::PinballBoard() : j1Module()
{
	name.create("pinball");
	background.rect = walls.rect = { 0,0,590,500 };
	ball_sprite.rect = { 0,0,13,13 };
	yellowsticker.rect = { 0,0,30,31 };
	bluesticker.rect = { 0,0,37,34 };
	greysticker.rect = { 0,0,28,28 };
	right_kicker.rect  = left_kicker.rect = { 0,0,54,26 };
	right_puncher.rect = { 0,0,67,79 };
	x_sprite.rect = { 0,0,211,167 };

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
	ball = App->physics->CreateCircle(300, 200, 6, BALL, LAUNCH);

	yellowsticker.image = App->tex->Load("Sprites/yellowsticker.png");
	greysticker.image = App->tex->Load("Sprites/greysticker.png");
	bluesticker.image = App->tex->Load("Sprites/bluesticker.png");

	left_kicker.image = App->tex->Load("Sprites/left_kicker.png");
	right_kicker.image = App->tex->Load("Sprites/right_kicker.png");

	right_puncher.image = App->tex->Load("Sprites/right_puncher.png");

	x_sprite.image = App->tex->Load("Sprites/midle_x.png");

	return true;
}


bool PinballBoard::Draw()
{
	App->render->Blit(background.image, 0, 0, &background.rect);

	int ball_x, ball_y;
	ball->GetPosition(ball_x, ball_y);
	if (ball->body->GetFixtureList()->GetFilterData().maskBits == TOP) {
		App->render->Blit(x_sprite.image, 190, 173, &x_sprite.rect);

		App->render->Blit(bluesticker.image, 274, 315, &bluesticker.rect); //blue stickers
		App->render->Blit(bluesticker.image, 256, 279, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 250, 243, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 304, 282, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 306, 244, &bluesticker.rect);

		App->render->Blit(walls.image, 0, 0, &walls.rect);
	}

	App->render->Blit(ball_sprite.image, ball_x, ball_y, &ball_sprite.rect, 1.0f, ball->GetRotation());

	if (ball->body->GetFixtureList()->GetFilterData().maskBits != TOP) {
		App->render->Blit(x_sprite.image, 190, 173, &x_sprite.rect);

		App->render->Blit(bluesticker.image, 274, 315, &bluesticker.rect); //blue stickers
		App->render->Blit(bluesticker.image, 256, 279, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 250, 243, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 304, 282, &bluesticker.rect);
		App->render->Blit(bluesticker.image, 306, 244, &bluesticker.rect);

		App->render->Blit(walls.image, 0, 0, &walls.rect);
	}
	p2List_item<kicker_info>* lkick = left_kickers.start;
	while (lkick != NULL)
	{
		int kick_x, kick_y;
		lkick->data.anchor->GetPosition(kick_x, kick_y);
		App->render->Blit(left_kicker.image, kick_x, kick_y - left_kicker.rect.h / 4, &left_kicker.rect, 1.0f, lkick->data.body->GetRotation(), 9, left_kicker.rect.h / 2);

		lkick = lkick->next;
	}

	p2List_item<kicker_info>* rkick = right_kickers.start;
	while (rkick != NULL)
	{
		int kick_x, kick_y;
		rkick->data.anchor->GetPosition(kick_x, kick_y);
		App->render->Blit(right_kicker.image, kick_x - 40, kick_y - 12, &right_kicker.rect, 1.0f, rkick->data.body->GetRotation(), 53, right_kicker.rect.h / 2);

		rkick = rkick->next;
	}

	App->render->Blit(yellowsticker.image, 371, 89, &yellowsticker.rect);  // yellow stickers
	App->render->Blit(yellowsticker.image, 401, 126, &yellowsticker.rect);
	App->render->Blit(yellowsticker.image, 414, 84, &yellowsticker.rect);

	App->render->Blit(greysticker.image, 144, 88, &greysticker.rect);  // grey sticker

	return true;
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
	App->tex->UnLoad(right_puncher.image);

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
	if (bodyA == x_lefttop_toTOP || bodyA == x_righttop_toTOP || bodyA == x_rightbot_toTOP || bodyA == x_leftbot_toTOP) {
		if (bodyB == ball) {
			b2Filter fil;
			fil.categoryBits = BALL;
			fil.maskBits = TOP;
			ball->body->GetFixtureList()->SetFilterData(fil);
		}
	}
	if (bodyA == x_lefttop_toBOARD || bodyA == x_righttop_toBOARD || bodyA == x_rightbot_toBOARD || bodyA == x_leftbot_toBOARD) {
		if (bodyB == ball) {
			b2Filter fil;
			fil.categoryBits = BALL;
			fil.maskBits = BOARD;
			ball->body->GetFixtureList()->SetFilterData(fil);
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
	App->physics->CreateStaticChain(0, 0, launcher_tub, size, LAUNCH, BALL);

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
			207, 5,
			207, 30,
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
			233, 113,
			160, 187,
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
			59, 188,
			-7, 118,
			16, 96,
			25, 85,
			38, 70,
			51, 57,
			67, 44,
			67, 42,
			62, 39,
			50, 33,
			40, 29,
			29, 26,
			22, 25,
			9, 27,
			9, 3
		};
		size = 112;
		App->physics->CreateStaticChain(190, 173, midle_x, size, TOP, BALL);
	}
	return true;
}

bool PinballBoard::CreateStickersCollisions()
{
	App->physics->CreateStaticCircle(386, 102, 10); //yellow stickers
	App->physics->CreateStaticCircle(429, 97, 10);
	App->physics->CreateStaticCircle(416, 139, 10);

	App->physics->CreateStaticCircle(158, 102, 10); // grey sticker

	App->physics->CreateStaticCircle(323, 259, 10); //blue stickers
	App->physics->CreateStaticCircle(267, 258, 10);
	App->physics->CreateStaticCircle(273, 294, 10);
	App->physics->CreateStaticCircle(291, 330, 10);
	App->physics->CreateStaticCircle(321, 297, 10);
	return true;
}

bool PinballBoard::CreateKickers()
{
	kicker_info kick;
	kick.anchor = App->physics->CreateStaticCircle(128, 462, 5, BOARD, BALL);   // left bottom left kicker
	int left_kicker_points[8] = {
		8, 8,
		8, 17,
		52, 17,
		52, 14
	};
	int size = 8;
	kick.body = App->physics->CreatePolygon(128, 462, left_kicker_points, size, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 8,13 }, true, 25, -25, true, 30, 40);
	left_kickers.add(kick);

	kick.anchor = App->physics->CreateStaticCircle(58, 285, 5, BOARD, BALL);		//left mid kicker
	kick.body = App->physics->CreatePolygon(58, 285, left_kicker_points, size, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 8,13 }, true, 45, -5, true, 30, 40);
	left_kickers.add(kick);

	kick.anchor = App->physics->CreateStaticCircle(358, 462, 5, BOARD, BALL);		//right bottom left kicker
	kick.body = App->physics->CreatePolygon(358, 462, left_kicker_points, size, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 8,13 }, true, 25, -25, true, 30, 40);
	left_kickers.add(kick);

	int right_kicker_points[8] = {
		50, 8,
		50, 17,
		6, 17,
		6, 14
	};
	kick.anchor = App->physics->CreateStaticCircle(228, 464, 5, BOARD, BALL);		//left bottom right kicker
	kick.body = App->physics->CreatePolygon(228, 464, right_kicker_points, size, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 50,13 }, true, 25, -25, true, -30, 40);
	right_kickers.add(kick);

	kick.anchor = App->physics->CreateStaticCircle(540, 282, 5, BOARD, BALL);		//right mid kicker
	kick.body = App->physics->CreatePolygon(540, 282, right_kicker_points, size, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 50,13 }, true, 5, -45, true, -30, 40);
	right_kickers.add(kick);

	kick.anchor = App->physics->CreateStaticCircle(461, 462, 5, BOARD, BALL);		//right bottom right kicker
	kick.body = App->physics->CreatePolygon(461, 462, right_kicker_points, size, BOARD, BALL);
	kick.joint = App->physics->CreateRevoluteJoint(kick.anchor, kick.body, { 0,0 }, { 50,13 }, true, 25, -25, true, -30, 40);
	right_kickers.add(kick);

	return false;
}

bool PinballBoard::CreateTrigers()
{
	launch_triger = App->physics->CreateRectangleSensor(295, 152, 32, 1, LAUNCH);
	launch_triger->listener = App->pinball;

	x_lefttop_toTOP = App->physics->CreateRectangleSensor(212, 189, 1, 10);
	x_lefttop_toTOP->listener = App->pinball;
	x_lefttop_toBOARD = App->physics->CreateRectangleSensor(210, 189, 1, 10, TOP);
	x_lefttop_toBOARD->listener = App->pinball;

	x_righttop_toTOP = App->physics->CreateRectangleSensor(376, 189, 1, 10);
	x_righttop_toTOP->listener = App->pinball;
	x_righttop_toBOARD = App->physics->CreateRectangleSensor(378, 189, 1, 10, TOP);
	x_righttop_toBOARD->listener = App->pinball;

	x_rightbot_toTOP = App->physics->CreateRectangleSensor(370, 302, 1, 60, BOARD, BALL, 40);
	x_rightbot_toTOP->listener = App->pinball;
	x_rightbot_toBOARD = App->physics->CreateRectangleSensor(373, 305, 1, 60, TOP, BALL, 40);
	x_rightbot_toBOARD->listener = App->pinball;

	x_leftbot_toTOP = App->physics->CreateRectangleSensor(217, 302, 1, 60, BOARD, BALL, -40);
	x_leftbot_toTOP->listener = App->pinball;
	x_leftbot_toBOARD = App->physics->CreateRectangleSensor(215, 305, 1, 60, TOP, BALL, -40);
	x_leftbot_toBOARD->listener = App->pinball;

	return true;
}
