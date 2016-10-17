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

	walls.image = App->tex->Load("Sprites/Walls.png");
	background.image = App->tex->Load("Sprites/background.png");
	ball_sprite.image = App->tex->Load("Sprites/ball.png");
	ball = App->physics->CreateCircle(100, 100, 6);

	return true;
}


bool PinballBoard::Draw()
{
	App->render->Blit(background.image, 0, 0, &background.rect);

	int ball_x, ball_y;
	ball->GetPosition(ball_x, ball_y);
	App->render->Blit(ball_sprite.image, ball_x, ball_y, &ball_sprite.rect, 1.0f, ball->GetRotation());

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
	int wallshape[240] = {
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
	int left_top[24] = {
		239, 64,
		235, 66,
		236, 72,
		239, 81,
		240, 90,
		239, 106,
		247, 105,
		254, 100,
		260, 95,
		260, 91,
		253, 78,
		244, 63
	};
	size = 24;
	App->physics->CreateStaticChain(0, 0, left_top, size);
	int mid_top[84] = {
		264, 110,
		251, 118,
		234, 123,
		230, 134,
		223, 144,
		213, 156,
		201, 165,
		195, 169,
		195, 176,
		205, 180,
		217, 180,
		235, 181,
		248, 183,
		259, 185,
		271, 188,
		282, 191,
		296, 197,
		303, 193,
		318, 188,
		339, 184,
		354, 181,
		383, 180,
		393, 175,
		397, 169,
		398, 163,
		394, 158,
		382, 152,
		373, 144,
		366, 133,
		361, 125,
		350, 123,
		340, 119,
		332, 114,
		328, 111,
		322, 111,
		317, 127,
		303, 173,
		285, 173,
		279, 149,
		274, 129,
		269, 116,
		268, 110
	};
	size = 84;
	App->physics->CreateStaticChain(0, 0, mid_top, size);  // Points need revision
	int left_elefant[122] = {
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
	App->physics->CreateStaticChain(0, 0, left_elefant, size);
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
	int right_top[30] = {
		352, 52,
		359, 52,
		362, 60,
		362, 78,
		360, 88,
		359, 97,
		360, 102,
		360, 109,
		358, 110,
		353, 110,
		341, 105,
		329, 95,
		329, 89,
		340, 69,
		346, 59
	};
	size = 30;
	App->physics->CreateStaticChain(0, 0, right_top, size);
	int right_leftseparator[16] = {
		375, 50,
		375, 71,
		378, 74,
		381, 74,
		383, 71,
		383, 50,
		381, 48,
		377, 48
	};
	size = 16;
	App->physics->CreateStaticChain(0, 0, right_leftseparator, size);

	return true;
}
