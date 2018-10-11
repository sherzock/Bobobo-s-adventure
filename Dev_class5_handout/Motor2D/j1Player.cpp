#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"



j1Player::j1Player() : j1Module(){
		
	current_animation = NULL;

	// Idle 
	idle.PushBack({ 2, 21, 42, 61 });
	idle.PushBack({ 60, 21, 44, 60 });
	idle.PushBack({ 125, 21, 42, 59 });
	idle.PushBack({ 181, 21, 42, 59 });

	idle.PushBack({ 125, 21, 42, 59 });
	idle.PushBack({ 60, 21, 44, 60 });

	idle.loop = true;
	idle.speed = 0.02f;
	
	//Run
	run.PushBack({ 0, 101, 44, 57 });
	run.PushBack({ 60, 94, 52, 64 });
	run.PushBack({ 128, 100, 52, 59 });
	run.PushBack({ 184, 98, 53, 61 });
	run.PushBack({ 240, 101, 56, 58 });
	run.PushBack({ 307, 95, 52, 64 });
	run.PushBack({ 373, 99, 59, 60 });
	run.PushBack({ 438, 101, 58, 62 });

	run.loop = true;
	run.speed = 0.023f;
	//jump 
	
	jumpanim.PushBack({ 2, 244, 63, 69 });
	jumpanim.PushBack({ 78, 244, 63, 69 });
	jumpanim.PushBack({ 156, 247, 63, 69 });

	jumpanim.loop = false;
	
	//falliig

	falling.PushBack({ 2, 244, 63, 69 });
	falling.PushBack({ 78, 244, 63, 69 });

	falling.loop = true;
	falling.speed = 0.009f;


	name.create("player");
}

j1Player::~j1Player() {}


bool j1Player::Awake(pugi::xml_node& config) {
	
	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	return true;
}


bool j1Player::Start() {
	
	graphics = App->tex->Load("textures/character.png");

	position.x = 300;
	position.y = 300;

	current_animation = &idle;

	 XSpeed = 0.25f;
	 initialspeed = 0.02f;
	 JumpSpeed = -0.22f;
	 gravity = 0.0f;
	
	player = App->colls->AddCollider({ (int)position.x, (int)position.y, 50, 55 }, PLAYER_COLLIDER, this);

	return true;
}


bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	
	// Direction controls
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
		position.x += XSpeed;

		current_animation = &run;
		goingright = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT) {
		position.x -= XSpeed;

		current_animation = &run;
		goingright = false;
	}

	if (CanPlayerJump == true) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && position.y < 1000) {
		
			jump = true;
			CanPlayerJump = false;

		}
	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_IDLE) {
		
			current_animation = &idle;
		
			
	}
	
	if (GroundCollision == true) {

		isfalling = false;
	}

	if (isfalling == true) {
		
		current_animation = &falling;
		CanPlayerJump = false;
	}

	GroundCollision = false;
	
	if (GroundCollision == false && jump == false) {
		position.y += gravity;
		if (gravity < 0.2) {
			gravity += 0.02f;
		}
	}

	if (jump) {
		position.y -= JumpSpeed; 
		JumpSpeed += 0.002f;
		current_animation = &jumpanim;

		if (JumpSpeed > 0.8f) { 
			JumpSpeed -= 0.002f;
			jump = false;
			isfalling = true;
		}

	}	
	if (jump == false) {
		JumpSpeed = -0.0002f;
	}

	player->Set_Pos(position.x,position.y);

	SDL_Rect character = current_animation->GetCurrentFrame();
	
	if (goingright == false) {

		App->render->Blit(graphics, (int)position.x, (int)position.y, &character, SDL_FLIP_HORIZONTAL);

	}
	else if(goingright == true) {

		App->render->Blit(graphics, (int)position.x, (int)position.y, &character, SDL_FLIP_NONE);

	}


	

	return true;
}

bool j1Player::PostUpdate() {

	return true;
}

// Load game 
bool j1Player::Load(pugi::xml_node& data) {

	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	return true;
}

// Save game 
bool j1Player::Save(pugi::xml_node& data) const {

	pugi::xml_node player = data.append_child("position");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	return true;
}


bool j1Player::CleanUp() {

	
	App->tex->UnLoad(graphics);

	return true;
}


void j1Player::OnCollision(Collider* col_1, Collider* col_2)
{
	if ((col_1->type == PLAYER_COLLIDER && col_2->type == GROUND_COLLIDER)
		|| (col_2->type == PLAYER_COLLIDER && col_1->type == GROUND_COLLIDER))
	{
			CanPlayerJump = true;
			JumpSpeed = initialspeed;
			GroundCollision = true;
			gravity = 0.0f;
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_UP && position.y < 1000) {

				jump = false;

			}



	}
};