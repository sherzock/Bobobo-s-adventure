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
#include "j1Scene2.h"
#include "j1Map.h"
#include "j1Entity.h"


j1Player::j1Player(int x, int y, entitytypes type) : j1Entity(x, y, entitytypes::PLAYER)
{
		
	current_animation = NULL;

	

	idle.LoadAnimations("idle");
	run.LoadAnimations("run");
	jumpanim.LoadAnimations("jumpanim");
	falling.LoadAnimations("falling");
	dashanim.LoadAnimations("dash");
	wallgrab.LoadAnimations("wall");
	attackanim.LoadAnimations("attack");
	name.create("player");
	
}

j1Player::~j1Player() {}


bool j1Player::Awake(pugi::xml_node& config) {
	

	return true;
}


bool j1Player::Start() {
	
	graphics = App->tex->Load("textures/character.png");


	position.x = App->map->map_file.child("map").child("properties").child("property").attribute("value").as_int();
	position.y = App->map->map_file.child("map").child("properties").child("property").next_sibling("property").attribute("value").as_int();

	Initial_position.x = position.x;
	Initial_position.y = position.y;
	Sleeping();

	current_animation = &idle;
	
	collider = App->colls->AddCollider({ (int)position.x, (int)position.y, 50, 55 }, PLAYER_COLLIDER, App->enty);

	return true;
}


bool j1Player::PreUpdate() {

	return true;
}

bool j1Player::Update(float dt) {

	collider->Set_Pos(position.x, position.y);
	// Ground Collision // 
	if (GroundCollision == true) {
		isfalling = false;
		CanPlayerJump = true;
		CanPlayerDash = false;
		CanPlayerDash = true;
		airborne = false;
	}
	else if (GroundCollision == false) {
		
		isfalling = true;
		airborne = true;
		
	}


	// Movement //

	
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
		
		if (wallhitri == false || wallhitle == true && GroundCollision == true) {
			
				position.x += XSpeed;
				current_animation = &run;
				goingright = true;
			
		}
	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_IDLE || App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_UP) {
		wallhitri = false;
		wallhitle = false;
	
	}
	

	if (position.x >= 0) {
		if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT ) {
			
			if (wallhitle == false || wallhitri == true) {
				
					position.x -= XSpeed;
					current_animation = &run;
					goingright = false;
				
			}
			
			
		}

	}
	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_IDLE && App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_IDLE || App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_UP) {
		wallhitle = false;
		wallhitri = false;
		
	}
	
	if (isfalling == true && CanPlayerJump == false) {
		wallhitle = false;
		wallhitri = false;
	}
	
	
	if (current_animation == &run && GroundCollision == false) {
		current_animation = &falling;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_IDLE) {
		
			current_animation = &idle;
			
	}
	
	if (App->input->GetKey(SDL_SCANCODE_C) == j1KeyState::KEY_DOWN ) {
		
		if (GroundCollision == true && wallhitle == false && wallhitri == false) {
			attack = true;

		}
		
	}
	
	if (attack == true) {

		if (goingright == true) {

			current_animation = &attackanim;
			//attackcoll = new Collider;
			if (attackcoll == nullptr) {
				attackcoll = App->colls->AddCollider({ (int)position.x + 50 , (int)position.y, 34, 55 }, ATTACK_COLLIDER, App->scene);

			}
			
			if (attackanim.Finished() == true) {
				attackanim.Reset();
				
				App->colls->Delete_Collider(attackcoll);
				attackcoll->to_delete = true;
				attackcoll = nullptr;
				attack = false;

				

			}
		}
		if (goingright == false) {
			current_animation = &attackanim;
			//attackcoll = new Collider;
			if (attackcoll == nullptr) {
				attackcoll = App->colls->AddCollider({ (int)position.x - 25 , (int)position.y, 34, 55 }, ATTACK_COLLIDER, App->scene);

			}
		

			if (attackanim.Finished() == true) {
				attackanim.Reset();
				App->colls->Delete_Collider(attackcoll);
				attackcoll->to_delete = true;
				attackcoll = nullptr;
				attack = false;

			
				
			}
		}
		
	}

	
	// Debug //
	//God mode//

	if (App->input->GetKey(SDL_SCANCODE_F10) == j1KeyState::KEY_DOWN) {

		godmode = !godmode;
	}
	
	if (godmode == true) {

		collider->type = NO_COLLIDER;
	}
	else if (godmode == false) {
		collider->type = PLAYER_COLLIDER;
	}


	//Change level//
	if (App->input->GetKey(SDL_SCANCODE_F3) == j1KeyState::KEY_DOWN) {

		dead = false;
		if (App->scene->active == true) {

			App->scene->change_scenes1();

		}
		else if (App->scene2->active == true) {
			App->scene2->change_scenes2();
		}

	}

	//Restart level//

	if (App->input->GetKey(SDL_SCANCODE_F2) == j1KeyState::KEY_DOWN) {

		//App->fade->FadeToBlack(this, this);
		position.x = Initial_position.x;
		position.y = Initial_position.y;
		App->render->camera.x = 0;
		App->render->camera.y = 0;
		dead = false;


	}

	if (wallhitri == true || wallhitle == true) {
		CanPlayerJump = true;
	}

	//Restart Game//
	if (App->input->GetKey(SDL_SCANCODE_F1) == j1KeyState::KEY_DOWN) {

		if (App->scene->active == true) {

			//App->fade->FadeToBlack(this, this);
			position.x = Initial_position.x;
			position.y = Initial_position.y;
			App->render->camera.x = 0;
			App->render->camera.y = 0;
			dead = false;

		}
		else if (App->scene2->active == true) {
			App->scene2->change_scenes2();
			dead = false;
		}
	}

	//Dash
	if (App->input->GetKey(SDL_SCANCODE_A) != j1KeyState::KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) != j1KeyState::KEY_DOWN) {

		if (CanPlayerDash == true) {
			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == j1KeyState::KEY_DOWN) {

				dash = true;
				CanPlayerDash = false;

			}
		}
	}
	

	

	//Jump instructions//
	
	if (CanPlayerJump == true) {
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_DOWN && position.y < 1000) {

			jump = true;
			CanPlayerJump = false;
			
		}
	}
	//Conditions for movement
	
	if (isfalling == true && wallhitle == false && wallhitri == false /*|| GroundCollision == false*/) {
		
		current_animation = &falling;
		
		GroundCollision = false;
		
		
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
			
			jump = false;
			isfalling = true;
			
			Jumpforce = Jumpreset;
			
		}

	}	

	

		if (goingright == true && dash == true) {

			gravity = 0;
			current_animation = &dashanim;
			position.x += DashSpeed;
			DashSpeed += DashAcc;
			CanPlayerJump = false;
			CanPlayerDash = false;
			

			if (DashSpeed >= 10) {
				dash = false;
				CanPlayerDash = false;
				isfalling = true;
				DashSpeed = DashSpeedres;

			}


		}
		else if (goingright == false && dash == true) {

			gravity = 0;
			current_animation = &dashanim;
			position.x -= DashSpeed;
			DashSpeed += DashAcc;
			CanPlayerJump = false;
			CanPlayerDash = false;
			
			
			if (DashSpeed >= 10) {
				dash = false;
				CanPlayerDash = false;
				isfalling = true;
				DashSpeed = DashSpeedres;

			}



		
	}
	
	

	//Death and Win//
	if (godmode == false) {
		
		if (position.y >= App->map->map_file.child("map").child("properties").child("property").next_sibling("property").next_sibling("property").attribute("value").as_float()) {

			dead = true;
		}

		if (dead == true) {

			//App->fade->FadeToBlack(this,this,0.8f);
			position.x = Initial_position.x;
			position.y = Initial_position.y;
			dead = false;
		}
	}
	

	if (position.x >= App->map->map_file.child("map").child("properties").child("property").next_sibling("property").next_sibling("property").next_sibling("property").attribute("value").as_float()) {
		
		win = true;
	}

	if (win == true) {
		
		dead = false;
		if (App->scene->active == true) {

			App->scene->change_scenes1();

		}
		else if (App->scene2->active == true) {
			App->scene2->change_scenes2();
		}
		win = false;
	}


	//player->Set_Pos(position.x,position.y);

	SDL_Rect character = current_animation->GetCurrentFrame();
	
	//Blit animations
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

	if (data.child("position").attribute("level").as_int() == 2 && App->scene->active == true)
	{
		App->scene->change_scenes1();
	}
	if (data.child("position").attribute("level").as_int() == 1 && App->scene2->active == true)
	{
		App->scene2->change_scenes2();
	}

	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	return true;
}

// Save game 
bool j1Player::Save(pugi::xml_node& data) const {

	pugi::xml_node player = data.append_child("position");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;
	if (App->scene->active == true)
	{
		player.append_attribute("level") = 1;
	}
	else if (App->scene2->active == true)
	{
		player.append_attribute("level") = 2;
	}
	

	return true;
}


bool j1Player::CleanUp() {

	
	App->tex->UnLoad(graphics);

	return true;
}


 void j1Player::OnCollision(Collider* col_1, Collider* col_2) 
{
	 
	float ResXspeed = XSpeed;
	if ((col_1->type == PLAYER_COLLIDER && col_2->type == GROUND_COLLIDER) || (col_2->type == PLAYER_COLLIDER && col_1->type == GROUND_COLLIDER) 
		|| (col_2->type == NO_COLLIDER && col_1->type == GROUND_COLLIDER)  || (col_1->type == NO_COLLIDER && col_2->type == GROUND_COLLIDER) )
	{
			CanPlayerJump = true;
			GroundCollision = true;
			gravity = 0.0f;
			current_animation = &run;
		

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_UP && position.y < 1000) {

				jump = false;
				current_animation = &run;
			}
			
			
			if (jump == true && airborne == true && wallhitle == false && wallhitri == false) {
				GroundCollision = false;
				isfalling = true;
				Jumpforce = 0;
				position.y += 6;
			
				//gravity = 0.2f;
			}
		
			
			
			if (col_1->rect.x + col_1->rect.w >= col_2->rect.x && col_1->rect.x + col_1->rect.w <= col_2->rect.x + XSpeed + 5) {
				wallhitri = true;
				wallhitle = false;
				
				DashSpeed = 0;
				
				position.x -= (col_1->rect.x + col_1->rect.w) + 2 - col_2->rect.x;
				CanPlayerJump = true;
				
				XSpeed = 0;
				dash = false;
				current_animation = &wallgrab;
				
			}else if (col_1->rect.x <= col_2->rect.x + col_2->rect.w && col_1->rect.x >= col_2->rect.x + col_2->rect.w - XSpeed - 5) {
				wallhitle = true;
				wallhitri = false;
				
				DashSpeed = 0;
				position.x += (col_2->rect.x + col_2->rect.w) + 2 - col_1->rect.x;
				CanPlayerJump = true;
				XSpeed = 0;
				dash = false;
				current_animation = &wallgrab;
		
			}else{
				wallhitri = false;
				wallhitle = false;
				current_animation = &falling;
			}
			
	}

	
		
	XSpeed= ResXspeed;
};

 void j1Player::Sleeping() {
 
	 
	 pugi::xml_document config_file;
	 config_file.load_file("config.xml");
	 pugi::xml_node config;
	 config = config_file.child("config");
 
	 position.x = config.child("position").attribute("x").as_int();
	 position.y = config.child("position").attribute("y").as_int();

	 pugi::xml_node physics = config.child("player").child("physics");
	
	 XSpeed = physics.child("xspeed").attribute("atr").as_float();
	 initialspeed = physics.child("initialspeed").attribute("atr").as_float();
	 gravity = physics.child("gravity").attribute("atr").as_float();
	 Jumpforce = physics.child("jumpforce").attribute("atr").as_float();
	 Jumpreset = physics.child("jumpreset").attribute("atr").as_float();
	 gravityMax = physics.child("gravityMax").attribute("atr").as_float();;
	 gravityIteratior = physics.child("gravityIterator").attribute("atr").as_float();;
	 JumpforceIterator = physics.child("JumpforceIterator").attribute("atr").as_float();
	 DashSpeed = physics.child("DashSpeed").attribute("atr").as_float();
	 DashSpeedres = physics.child("DashSpeedres").attribute("atr").as_float();
	 DashAcc = physics.child("DashAcc").attribute("atr").as_float();
 
 
 };