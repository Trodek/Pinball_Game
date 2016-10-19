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
	name.create("scene");
	background.rect = walls.rect = { 0,0,590,500 };
	ball_sprite.rect = { 0,0,13,13 };
	yellowsticker.rect = { 0,0,30,31 };
	bluesticker.rect = { 0,0,37,34 };
	greysticker.rect = { 0,0,28,28 };

}

// Destructor
PinballBoard::~PinballBoard()
{}

// Called before render is available
bool PinballBoard::Awake(pugi::xml_node& node)
{
	LOG("Loading Pinball Board");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool PinballBoard::Start()
{
	CreateBoardPhyisics();
	CreateStickersCollisions();

	walls.image = App->tex->Load("Sprites/Walls.png");
	background.image = App->tex->Load("Sprites/background.png");

	ball_sprite.image = App->tex->Load("Sprites/ball.png");
	ball = App->physics->CreateCircle(100, 100, 6, BALL, BOARD);

	yellowsticker.image = App->tex->Load("Sprites/yellowsticker.png");
	greysticker.image = App->tex->Load("Sprites/greysticker.png");
	bluesticker.image = App->tex->Load("Sprites/bluesticker.png");

	return true;
}


bool PinballBoard::Draw()
{
	App->render->Blit(background.image, 0, 0, &background.rect);

	int ball_x, ball_y;
	ball->GetPosition(ball_x, ball_y);
	App->render->Blit(ball_sprite.image, ball_x, ball_y, &ball_sprite.rect, 1.0f, ball->GetRotation());

	App->render->Blit(yellowsticker.image, 371, 89, &yellowsticker.rect);  // yellow stickers
	App->render->Blit(yellowsticker.image, 401, 126, &yellowsticker.rect);
	App->render->Blit(yellowsticker.image, 414, 84, &yellowsticker.rect);

	App->render->Blit(greysticker.image, 144, 88, &greysticker.rect);  // grey sticker

	App->render->Blit(bluesticker.image, 274, 315, &bluesticker.rect); //blue stickers
	App->render->Blit(bluesticker.image, 256, 279, &bluesticker.rect);
	App->render->Blit(bluesticker.image, 250, 243, &bluesticker.rect);
	App->render->Blit(bluesticker.image, 304, 282, &bluesticker.rect);
	App->render->Blit(bluesticker.image, 306, 244, &bluesticker.rect);

	App->render->Blit(walls.image, 0, 0, &walls.rect);

	return true;
}

// Called before quitting
bool PinballBoard::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool PinballBoard::CreateBoardPhyisics()
{
	int wallshape[240] = {			//Wall Shape
		136, 498,
		212, 498,
		212, 478,
		214, 474,
		229, 467,
		231, 456,
		281, 432,
		287, 425,
		290, 417,
		294, 412,
		297, 417,
		299, 424,
		305, 432,
		355, 455,
		358, 467,
		375, 478,
		375, 498,
		455, 497,
		455, 487,
		472, 476,
		531, 447,
		531, 372,
		522, 354,
		523, 347,
		531, 338,
		538, 336,
		545, 328,
		544, 318,
		534, 314,
		525, 317,
		522, 324,
		512, 332,
		505, 330,
		506, 325,
		527, 300,
		536, 292,
		542, 279,
		542, 266,
		541, 243,
		545, 230,
		532, 221,
		530, 214,
		522, 228,
		510, 227,
		480, 179,
		479, 174,
		497, 147,
		505, 124,
		508, 104,
		507, 77,
		496, 50,
		477, 30,
		453, 19,
		433, 14,
		407, 14,
		385, 18,
		365, 24,
		349, 32,
		330, 50,
		325, 56,
		324, 46,
		322, 27,
		314, 18,
		303, 13,
		288, 13,
		272, 24,
		265, 39,
		267, 48,
		261, 51,
		231, 26,
		195, 15,
		146, 12,
		118, 22,
		99, 39,
		85, 55,
		77, 79,
		80, 108,
		82, 125,
		88, 135,
		105, 151,
		116, 165,
		125, 184,
		122, 188,
		102, 201,
		76, 224,
		73, 224,
		65, 211,
		59, 184,
		57, 162,
		58, 129,
		57, 117,
		51, 112,
		45, 112,
		38, 117,
		37, 122,
		34, 170,
		37, 201,
		43, 235,
		50, 259,
		58, 276,
		58, 281,
		53, 283,
		55, 290,
		71, 309,
		84, 324,
		87, 328,
		84, 333,
		75, 330,
		70, 325,
		66, 316,
		54, 314,
		47, 319,
		45, 328,
		51, 335,
		58, 338,
		68, 348,
		68, 355,
		58, 372,
		58, 448,
		137, 486
	};
	int size = 240;
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
		int left_bot[18] = {
			73, 379,
			73, 437,
			126, 463,
			129, 460,
			128, 455,
			80, 433,
			80, 378,
			78, 375,
			75, 375
		};
		size = 18;
		App->physics->CreateStaticChain(0, 0, left_bot, size);

		int right_bot[24] = {
			511, 379,
			513, 376,
			516, 376,
			518, 379,
			518, 435,
			516, 439,
			469, 462,
			465, 462,
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
			378, 73,
			381, 74,
			383, 71,
			383, 50,
			381, 48,
			377, 48
		};
		size = 16;
		App->physics->CreateStaticChain(0, 0, right_leftseparator, size);

		int right_midseparator[16] = {
			396, 44,
			398, 41,
			401, 41,
			403, 44,
			403, 64,
			401, 65,
			398, 66,
			396, 64
		};
		App->physics->CreateStaticChain(0, 0, right_midseparator, size);

		int right_rightseparator[16] = {
			416, 39,
			418, 36,
			422, 36,
			424, 39,
			424, 58,
			422, 60,
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
	App->physics->CreateStaticChain(0, 0, launcher_tub, size,LAUNCH,LAUNCH);
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
