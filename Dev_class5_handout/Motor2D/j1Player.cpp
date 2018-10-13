#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"


j1Player::j1Player() : j1Module(){
		
	current_animation = NULL;

	

	idle.LoadAnimations("idle");
	run.LoadAnimations("run");
	jumpanim.LoadAnimations("jumpanim");
	falling.LoadAnimations("falling");

	name.create("player");
}

j1Player::~j1Player() {}


bool j1Player::Awake(pugi::xml_node& config) {
	
	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	pugi::xml_node physics = config.child("physics");
	XSpeed = physics.child("xspeed").attribute("atr").as_float();
	initialspeed = physics.child("initialspeed").attribute("atr").as_float();
	JumpSpeed = physics.child("jumpspeed").attribute("atr").as_float();
	gravity = physics.child("gravity").attribute("atr").as_float();
	Jumpforce = physics.child("jumpforce").attribute("atr").as_float();
	Jumpreset = physics.child("jumpreset").attribute("atr").as_float();
	gravityMax = physics.child("gravityMax").attribute("atr").as_float();;
	gravityIteratior = physics.child("gravityIterator").attribute("atr").as_float();;
	JumpforceIterator = physics.child("JumpforceIterator").attribute("atr").as_float();

	return true;
}


bool j1Player::Start() {
	
	graphics = App->tex->Load("textures/character.png");

	position.x = 100;
	position.y = 300;

	current_animation = &idle;
	
	player = App->colls->AddCollider({ (int)position.x, (int)position.y, 50, 55 }, PLAYER_COLLIDER, this);

	return true;
}


bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	
	// Ground Collision // 
	if (GroundCollision == true) {

		isfalling = false;
	}
	else if (GroundCollision == false) {
		
		isfalling = true;
	}


	// Movement //

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

	
	
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_IDLE) {
		
			current_animation = &idle;
			
	}
	
	// Debug //
	//God mode//

	if (App->input->GetKey(SDL_SCANCODE_F10) == j1KeyState::KEY_DOWN) {

		godmode = !godmode;
	}
	
	if (godmode == true) {

		player->type = NO_COLLIDER;
	}
	//Restart game//
	if (App->input->GetKey(SDL_SCANCODE_F1) == j1KeyState::KEY_DOWN) {

		App->fade->FadeToBlack(this, App->scene); // Propiietats del mapa pls
		
		position.x = 300; // start map position x
		position.y = 300; // start map position y
		App->render->camera.x = 0; // start camera x
		App->render->camera.y = 0; // start camera y
		dead = false;
		
	}
	//Restart level//
	if (App->input->GetKey(SDL_SCANCODE_F2) == j1KeyState::KEY_DOWN) {

		App->fade->FadeToBlack(this, this); // Propietats del mapa pls
		
		
			position.x = 300;
			position.y = 300;
			App->render->camera.x = 0;
			App->render->camera.y = 0;
			dead = false;
		
		
	}

	if (App->input->GetKey(SDL_SCANCODE_C) == j1KeyState::KEY_DOWN) {

		dash = true;
	}

	if (dash == true) {
		float save = DashSpeed;
		position.x += DashSpeed;
		DashSpeed += 0.5f;

		if (DashSpeed >= 8) {
			dash = false;
			isfalling = true;
			DashSpeed = DashSpeedres;
		}
	}

	//Jump instructions//
	
	if (CanPlayerJump == true) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && position.y < 1000) {

			jump = true;
			CanPlayerJump = false;

		}
	}

	if (isfalling == true) {
		
		current_animation = &falling;
		CanPlayerJump = false;
		GroundCollision = false;
	}
	
	if (GroundCollision == true) {
		CanPlayerJump = true;
		isfalling = false;
	}

	GroundCollision = false;
	
	if (GroundCollision == false && jump == false  ) {
		position.y += gravity;
		
		if (gravity < gravityMax) {
			gravity += gravityIteratior;
		}
	}

	if (jump == true) {
		position.y -= Jumpforce; 
		Jumpforce -= JumpforceIterator;
		current_animation = &jumpanim;
		CanPlayerJump = false;

		if (Jumpforce <= 0) { 
			//JumpSpeed -= 0.002f;
			jump = false;
			isfalling = true;
			//GroundCollision = false;
			Jumpforce = Jumpreset;
		}

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
		|| (col_2->type == PLAYER_COLLIDER && col_1->type == GROUND_COLLIDER) 
		|| (col_2->type == NO_COLLIDER && col_1->type == GROUND_COLLIDER) 
		|| (col_1->type == NO_COLLIDER && col_2->type == GROUND_COLLIDER))
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