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
#include "PinballBoard.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");

}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	motor_speed = node.child("motor_speed").attribute("value").as_int();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	launch_down = App->audio->LoadFx("Sounds/sound 179.ogg");
	launch_up = App->audio->LoadFx("Sounds/sound 533 (launch_ball).ogg");
	kicker_down = App->audio->LoadFx("Sounds/sound 519 (flipper_down).ogg");
	Kicker_up = App->audio->LoadFx("Sounds/sound 518 (flipper_up).ogg");

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
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		App->pinball->Restart();
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		p2List_item<kicker_info>* left_kick = App->pinball->left_kickers.start;
		while (left_kick != NULL) {
			left_kick->data.joint->SetMotorSpeed(-motor_speed);

			left_kick = left_kick->next;
		}
		App->pinball->top_kicker.joint->SetMotorSpeed(motor_speed);
		App->audio->PlayFx(kicker_down);
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		p2List_item<kicker_info>* left_kick = App->pinball->left_kickers.start;
		while (left_kick != NULL) {
			left_kick->data.joint->SetMotorSpeed(-motor_speed);

			left_kick = left_kick->next;
		}
		App->pinball->top_kicker.joint->SetMotorSpeed(motor_speed);
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		p2List_item<kicker_info>* left_kick = App->pinball->left_kickers.start;
		while (left_kick != NULL) {
			left_kick->data.joint->SetMotorSpeed(motor_speed);

			left_kick = left_kick->next;
		}
		App->audio->PlayFx(Kicker_up);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		p2List_item<kicker_info>* right_kick = App->pinball->right_kickers.start;
		while (right_kick != NULL) {
			right_kick->data.joint->SetMotorSpeed(motor_speed);

			right_kick = right_kick->next;
		}
		App->pinball->top_kicker.joint->SetMotorSpeed(-motor_speed);
		App->audio->PlayFx(kicker_down);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		p2List_item<kicker_info>* right_kick = App->pinball->right_kickers.start;
		while (right_kick != NULL) {
			right_kick->data.joint->SetMotorSpeed(motor_speed);

			right_kick = right_kick->next;
		}
		App->pinball->top_kicker.joint->SetMotorSpeed(-motor_speed);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		p2List_item<kicker_info>* right_kick = App->pinball->right_kickers.start;
		while (right_kick != NULL) {
			right_kick->data.joint->SetMotorSpeed(-motor_speed);

			right_kick = right_kick->next;
		}
		App->audio->PlayFx(Kicker_up);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		
		App->pinball->launcher.joint->SetMotorSpeed(1);
		App->pinball->launcher.joint->SetMaxMotorForce(2);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {

		App->pinball->launcher.joint->SetMotorSpeed(1);
		App->pinball->launcher.joint->SetMaxMotorForce(2);
		App->audio->PlayFx(launch_down);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) {
		App->pinball->launcher.joint->SetMotorSpeed(-15);
		App->pinball->launcher.joint->SetMaxMotorForce(20);
		App->audio->PlayFx(launch_up);
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		if(App->pinball->losed_balls>2)
			App->pinball->Reset();
	}

	App->pinball->Draw();
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
