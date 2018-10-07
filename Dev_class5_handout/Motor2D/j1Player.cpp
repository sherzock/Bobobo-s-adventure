#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Render.h"



j1Player::j1Player() : j1Module(){
	name.create("player");
}

j1Player::~j1Player() {}


bool j1Player::Awake(pugi::xml_node& config) {
	
	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	return true;
}

// Load assets
bool j1Player::Start() {
	
	graphics = App->tex->Load("textures/magenta.jpg");

	player->x = position.x;
	player->y = position.y;
	player->h = 10;
	player->w = 10;
	return true;
}


bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	
	// Direction controls
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
		position.x += XSpeed;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT) {
		position.x -= XSpeed;
	}

	if (position.y > 100) {
		position.y = 100;
	}
	else

		
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_REPEAT && position.y < 100) {
			position.y += YSpeed;
			YSpeed += 0.001f;

			


	
	App->render->Blit(graphics, (int)position.x, (int)position.y, player);

	return true;
}
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

	
	
	delete(&player);
	App->tex->UnLoad(graphics);

	return true;
}
