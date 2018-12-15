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
#include "j1PlayerUI.h"
#include "Brofiler/Brofiler.h"


j1Player::j1Player(int x, int y, entitytypes type) : j1Entity(x, y, entitytypes::PLAYER)
{
		
	animation = NULL;

	

	idle.LoadAnimations("idle");
	run.LoadAnimations("run");
	jumpanim.LoadAnimations("jumpanim");
	falling.LoadAnimations("falling");
	dashanim.LoadAnimations("dash");
	wallgrab.LoadAnimations("wall");
	attackanim.LoadAnimations("attack");
	deadanim.LoadAnimations("death");
	name.create("player");
	
}

j1Player::~j1Player() {}


bool j1Player::Awake(pugi::xml_node& config) {
	

	return true;
}


bool j1Player::Start() {
	
	graphics = App->tex->Load("textures/character.png");
	LoadXML();

	
	Initial_position.x = position.x;
	Initial_position.y = position.y;
	

	animation = &idle;
	
	collider = App->colls->AddCollider({ (int)position.x, (int)position.y, playerwidth, playerheight }, PLAYER_COLLIDER, App->enty);

	playerUI = new j1PlayerUI();
	playerUI->Start();
	playerlifes = 3;
	numberofcoins = 0;
	playerpoints = 0;

	return true;
}


bool j1Player::PreUpdate() {
	BROFILER_CATEGORY("Player PreUpdate", Profiler::Color::MediumVioletRed)

	return true;
}

bool j1Player::Update(float dt) {

	BROFILER_CATEGORY("Player Update", Profiler::Color::Tomato)
		

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
			
				position.x += XSpeed* dt * FSpeed;
				animation = &run;
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
				
					position.x -= XSpeed* dt * FSpeed;
					animation = &run;
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
	
	
	if (animation == &run && GroundCollision == false) {
		animation = &falling;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_IDLE) {
		
		animation = &idle;
			
	}
	
	if (App->input->GetKey(SDL_SCANCODE_C) == j1KeyState::KEY_DOWN ) {
		
		if (GroundCollision == true && wallhitle == false && wallhitri == false) {
			attack = true;

		}
		
	}
	
	if (attack == true) {

		if (goingright == true) {

			animation = &attackanim;
		
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
			animation = &attackanim;
			
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
		gravity = 0;
		if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT) {
		
			position.x += 8;
			
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT) {

			position.x -= 8;

		}
		if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT) {

			position.y += 8;

		}
		if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT) {

			position.y -= 8;

		}

	}
	else if (godmode == false) {
		collider->type = PLAYER_COLLIDER;
	}



	if (wallhitri == true || wallhitle == true) {
		CanPlayerJump = true;
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
	
	if (isfalling == true && wallhitle == false && wallhitri == false ) {
		
		animation = &falling;
		
		GroundCollision = false;
		
		
	}

	
	GroundCollision = false;
	

	if (GroundCollision == false && jump == false) {
		position.y += gravity * dt * FSpeed;
		
		if (gravity < gravityMax*0.8) {
			gravity += gravityIteratior * dt* FSpeed;
			
		}
	}

	if (jump == true) {
		
		position.y -= Jumpforce * dt * FSpeed*1.5;
		Jumpforce -= JumpforceIterator * dt * FSpeed * 1.5;
		animation = &jumpanim;
		CanPlayerJump = false;
		

		if (Jumpforce <= 0) { 
			
			jump = false;
			isfalling = true;
			
			Jumpforce = Jumpreset;
			
		}

	}	

	

		if (goingright == true && dash == true) {

			gravity = 0;
			animation = &dashanim;
			position.x += DashSpeed * dt * FSpeed;
			DashSpeed += DashAcc * dt * FSpeed;
			CanPlayerJump = false;
			CanPlayerDash = false;
			collider->type = ATTACK_COLLIDER;

			if (DashSpeed >= 10) {
				dash = false;
				CanPlayerDash = false;
				isfalling = true;
				DashSpeed = DashSpeedres;
				collider->type = PLAYER_COLLIDER;
			}


		}
		else if (goingright == false && dash == true) {

			gravity = 0;
			animation = &dashanim;
			position.x -= DashSpeed* dt * FSpeed;
			DashSpeed += DashAcc * dt * FSpeed;
			CanPlayerJump = false;
			CanPlayerDash = false;
			collider->type = ATTACK_COLLIDER;
			
			if (DashSpeed >= 10) {
				dash = false;
				CanPlayerDash = false;
				isfalling = true;
				DashSpeed = DashSpeedres;
				collider->type = PLAYER_COLLIDER;
			}



		
	}
	
	

	//Death and Win//
	if (godmode == false) {
		
		if (position.y >= App->map->map_file.child("map").child("properties").child("property").next_sibling("property").next_sibling("property").attribute("value").as_float()) {

				dead = true;
			
		}

		if (dead == true) {
		
			playerlifes--;
			animation = &deadanim;
			numberofcoins = 0;
			if(App->scene->active == true ){
				if (playerlifes > 0) {
					App->fade->FadeToBlack(App->scene, App->scene,0.8f);
					position.x = 30;
					position.y = 520;
					dead = false;
					App->enty->DestroyEnemies();
					App->scene->AddAllEnemies();
					deadanim.Reset();
				}
				else{
					App->scene->deadrestart = true;
				}
				
			}
			if (App->scene2->active == true ) {
				if (playerlifes > 0) {
					App->fade->FadeToBlack(App->scene2, App->scene2, 0.8f);
					position.x = 30;
					position.y = 350;
					dead = false;
					App->enty->DestroyEnemies();
					App->scene2->AddAllEnemies2();
					deadanim.Reset();
				}
				else{
					App->scene2->deadrestart = true;
				}
			}
			
		}
	}
	
	
	if(win1 == false || win2 == false){
		SDL_Rect character = animation->GetCurrentFrame(dt);

		//Blit animations
		if (goingright == false) {

			App->render->Blit(graphics, (int)position.x, (int)position.y, &character, SDL_FLIP_HORIZONTAL);

		}
		else if (goingright == true) {

			App->render->Blit(graphics, (int)position.x, (int)position.y, &character, SDL_FLIP_NONE);

		}
	}
	
	
	
	playerUI->Update(dt);

	

	return true;
}

bool j1Player::PostUpdate() {

	BROFILER_CATEGORY("Player PostUpdate", Profiler::Color::MediumVioletRed)
		
	return true;
}

// Load game 
bool j1Player::Load(pugi::xml_node& data) {

	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	if (data.child("position").attribute("level").as_int() == 2 && App->scene->active == true)
	{
		App->scene->change_scenes1();
	}
	if (data.child("position").attribute("level").as_int() == 1 && App->scene2->active == true)
	{
		App->scene2->change_scenes2();
	}

	
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
			animation = &run;
			

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == j1KeyState::KEY_UP && position.y < 1000) {

				jump = false;
				animation = &run;
			}
			
			
			if (jump == true && airborne == true && wallhitle == false && wallhitri == false) {
				GroundCollision = false;
				isfalling = true;
				Jumpforce = 0;
				position.y += 10;
			
				
			}
		
			
			
			if (col_1->rect.x + col_1->rect.w >= col_2->rect.x && col_1->rect.x + col_1->rect.w <= col_2->rect.x + XSpeed + 5) {
				wallhitri = true;
				wallhitle = false;
				
				DashSpeed = 0;
				
				position.x -= (col_1->rect.x + col_1->rect.w) + 2 - col_2->rect.x;
				CanPlayerJump = true;
				
				XSpeed = 0;
				dash = false;
				animation = &wallgrab;
				
			}else if (col_1->rect.x <= col_2->rect.x + col_2->rect.w && col_1->rect.x >= col_2->rect.x + col_2->rect.w - XSpeed - 5) {
				wallhitle = true;
				wallhitri = false;
				
				DashSpeed = 0;
				position.x += (col_2->rect.x + col_2->rect.w) + 2 - col_1->rect.x;
				CanPlayerJump = true;
				XSpeed = 0;
				dash = false;
				animation = &wallgrab;
		
			}else{
				wallhitri = false;
				wallhitle = false;
				animation = &falling;
			}
			
	}
	
	if ((col_1->type == PLAYER_COLLIDER && col_2->type == ENEMY_COLLIDER) || (col_2->type == PLAYER_COLLIDER && col_1->type == ENEMY_COLLIDER))
	{
			dead = true;
	}
	/*if ((col_1->type == PLAYER_COLLIDER && col_2->type == COIN_COLLIDER) || (col_2->type == PLAYER_COLLIDER && col_1->type == COIN_COLLIDER))
	{
		numberofcoins += 1;
		playerpoints += 10;
	}*/

	XSpeed= ResXspeed;
};

 void j1Player::LoadXML() {
 
	 
	 pugi::xml_document config_file;
	 config_file.load_file("config.xml");
	 pugi::xml_node config;
	 config = config_file.child("config");
 
	 position.x = config.child("position").attribute("x").as_int();
	 position.y = config.child("position").attribute("y").as_int();

	 pugi::xml_node physics = config.child("player").child("physics");
	 gravityMax = physics.child("gravityMax").attribute("atr").as_float();

	 position.x = App->map->map_file.child("map").child("properties").child("property").attribute("value").as_int();
	 position.y = App->map->map_file.child("map").child("properties").child("property").next_sibling("property").attribute("value").as_int();
	 XSpeed = physics.child("xspeed").attribute("atr").as_float();
	 initialspeed = physics.child("initialspeed").attribute("atr").as_float();
	 gravity = physics.child("gravity").attribute("atr").as_float();
	 Jumpforce = physics.child("jumpforce").attribute("atr").as_float();
	 Jumpreset = physics.child("jumpreset").attribute("atr").as_float();
	 
	 gravityIteratior = physics.child("gravityIterator").attribute("atr").as_float();;
	 JumpforceIterator = physics.child("JumpforceIterator").attribute("atr").as_float();
	 DashSpeed = physics.child("DashSpeed").attribute("atr").as_float();
	 DashSpeedres = physics.child("DashSpeedres").attribute("atr").as_float();
	 DashAcc = physics.child("DashAcc").attribute("atr").as_float();
	 FSpeed = physics.child("Fspeed").attribute("atr").as_float();
 
 };

 bool j1Player::OnEventChange(j1UIItems* item, Event evnt) {
	 bool ret = true;
	 if (playerUI) {
		 ret = playerUI->OnEventChange(item, evnt);
	 }
	 return ret;
 }
