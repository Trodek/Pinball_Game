#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "ModulePhysics.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	background.rect = walls.rect = { 0,0,590,500 };
	ball_sprite.rect = { 0,0,13,13 };

}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
	walls.image = App->tex->Load("Sprites/Walls.png");
	background.image = App->tex->Load("Sprites/background.png");
	ball_sprite.image = App->tex->Load("Sprites/ball.png");
	ball = App->physics->CreateCircle(100, 100, 6);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		int x, y;
		App->input->GetMousePosition(x, y);
		App->physics->CreateCircle(x, y, 6);
	}
	
	App->render->Blit(background.image, 0, 0, &background.rect);

	int ball_x, ball_y;
	ball->GetPosition(ball_x, ball_y);
	App->render->Blit(ball_sprite.image, ball_x, ball_y, &ball_sprite.rect, 1.0f, ball->GetRotation());
	App->render->Blit(walls.image, 0, 0, &walls.rect);

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
