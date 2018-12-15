#include "j1Collisions.h"
#include "p2Defs.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "SDL\include\SDL.h"
#include "Brofiler/Brofiler.h"

j1Collisions::j1Collisions() : j1Module()
{

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i) {
		colliders[i] = nullptr;
	}
	name.create("collisions");

	matrix[PLAYER_COLLIDER][PLAYER_COLLIDER] = false;
	matrix[PLAYER_COLLIDER][GROUND_COLLIDER] = true;
	matrix[PLAYER_COLLIDER][NO_COLLIDER] = false;
	matrix[PLAYER_COLLIDER][ATTACK_COLLIDER] = false;
	matrix[PLAYER_COLLIDER][ENEMY_COLLIDER] = true;
	matrix[PLAYER_COLLIDER][COIN_COLLIDER] = true;
	

	matrix[GROUND_COLLIDER][PLAYER_COLLIDER] = true;
	matrix[GROUND_COLLIDER][GROUND_COLLIDER] = false;
	matrix[GROUND_COLLIDER][NO_COLLIDER] = false;
	matrix[GROUND_COLLIDER][ATTACK_COLLIDER] = false;
	matrix[GROUND_COLLIDER][ENEMY_COLLIDER] = true;
	matrix[GROUND_COLLIDER][COIN_COLLIDER] = false;

	matrix[NO_COLLIDER][NO_COLLIDER] = false;
	matrix[NO_COLLIDER][GROUND_COLLIDER] = false;
	matrix[NO_COLLIDER][PLAYER_COLLIDER] = false;
	matrix[NO_COLLIDER][ATTACK_COLLIDER] = false;
	matrix[NO_COLLIDER][ENEMY_COLLIDER] = false;
	matrix[NO_COLLIDER][COIN_COLLIDER] = false;

	matrix[ATTACK_COLLIDER][ATTACK_COLLIDER] = false;
	matrix[ATTACK_COLLIDER][GROUND_COLLIDER] = false;
	matrix[ATTACK_COLLIDER][PLAYER_COLLIDER] = false;
	matrix[ATTACK_COLLIDER][NO_COLLIDER] = false;
	matrix[ATTACK_COLLIDER][ENEMY_COLLIDER] = true;
	matrix[ATTACK_COLLIDER][COIN_COLLIDER] = false;
	
	matrix[ENEMY_COLLIDER][ENEMY_COLLIDER] = false;
	matrix[ENEMY_COLLIDER][GROUND_COLLIDER] = true;
	matrix[ENEMY_COLLIDER][PLAYER_COLLIDER] = true;
	matrix[ENEMY_COLLIDER][NO_COLLIDER] = false;
	matrix[ENEMY_COLLIDER][ATTACK_COLLIDER] = true;
	matrix[ENEMY_COLLIDER][COIN_COLLIDER] = false;

	matrix[COIN_COLLIDER][COIN_COLLIDER] = false;
	matrix[COIN_COLLIDER][ENEMY_COLLIDER] = false;
	matrix[COIN_COLLIDER][GROUND_COLLIDER] = false;
	matrix[COIN_COLLIDER][PLAYER_COLLIDER] = true;
	matrix[COIN_COLLIDER][NO_COLLIDER] = false;
	matrix[COIN_COLLIDER][ATTACK_COLLIDER] = false;
	

}

j1Collisions::~j1Collisions() {}

bool j1Collisions::PreUpdate(){

	BROFILER_CATEGORY("Collisions PreUpdate", Profiler::Color::MediumVioletRed)

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i) {
		
		if (colliders[i] != nullptr) {
			
			if (colliders[i]->to_delete) {

				delete colliders[i];
				colliders[i] = nullptr;
			
			}
		
		}
	}

	Collider* collider1;
	Collider* collider2;

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i) {
		if (colliders[i] == nullptr)
			continue;
		if (colliders[i]->type == PLAYER_COLLIDER || colliders[i]->type == NO_COLLIDER || colliders[i]->type == ENEMY_COLLIDER || colliders[i]->type == COIN_COLLIDER){

			collider1 = colliders[i];

			for (uint j = 0; j < MAX_NUM_COLLIDERS; ++j) {
				if (colliders[j] == nullptr || i == j)
					continue;

				collider2 = colliders[j];

				if (collider1->Check_Collision(collider2->rect) == true) {
					if (matrix[collider1->type][collider2->type] && collider1->callback)
						collider1->callback->OnCollision(collider1, collider2);
				}
			}
		}
	}

	return true;
}

bool j1Collisions::CleanUp(){

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}



bool j1Collisions::Update(float dt) {
	BROFILER_CATEGORY("Collision Update", Profiler::Color::Tomato)


	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		ShowColliders = !ShowColliders;
	}

	if (ShowColliders == false) {
		return true;
	}
	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;
		switch (colliders[i]->type)
		{
		case NO_COLLIDER:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 100, 75);  //yellow
			break;
		case GROUND_COLLIDER:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 200, 75); //green
			break;
		case PLAYER_COLLIDER:
			App->render->DrawQuad(colliders[i]->rect, 0, 50, 255, 75); //blue
			break;
		case ATTACK_COLLIDER:
			App->render->DrawQuad(colliders[i]->rect, 158, 103, 52, 75); //brown
			break;
		case ENEMY_COLLIDER:
			App->render->DrawQuad(colliders[i]->rect, 254, 60, 255, 75); //purpule
			break;
		case COIN_COLLIDER:
			App->render->DrawQuad(colliders[i]->rect, 255, 90, 71, 75); //orange
			break;
		}
	}



	if (ShowColliders == true) {
		for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i)
		{
			if (colliders[i] == nullptr)
				continue;
			switch (colliders[i]->type)
			{
			case NO_COLLIDER:
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, 30);  //yellow
				break;
			case GROUND_COLLIDER:
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, 75); //green
				break;
			case PLAYER_COLLIDER:
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, 75); //blue
				break;
			case ATTACK_COLLIDER:
				App->render->DrawQuad(colliders[i]->rect, 158, 103, 52, 75); //brown
				break;
			case ENEMY_COLLIDER:
				App->render->DrawQuad(colliders[i]->rect, 254, 60, 255, 75); //purpule
				break;
			case COIN_COLLIDER:
				App->render->DrawQuad(colliders[i]->rect, 254, 60, 255, 75); //purpule
				break;
			}

		}
	}





	return true;
}



Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback){

	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}
	return ret;
}


uint j1Collisions::Colliders_Count() const {

	int numberColl = 0;

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i)
	{

		if (colliders[i] != nullptr) { 
			numberColl++; 
		}
	}

	return numberColl;
}

bool j1Collisions::Delete_Collider(Collider* collider)
{
	if (collider != nullptr)
	{
		for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i)
		{
			if (colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}
	return false;
}

bool Collider::Check_Collision(const SDL_Rect& r) const {

	if (r.y + r.h <= rect.y || r.y >= rect.y + rect.h || r.x + r.w <= rect.x || r.x >= rect.x + rect.w) {

	
		return false;
	}
	else
		return true;
	
}

COLLISION_DIRECTION Collider::CheckDirection(const SDL_Rect& r) const
{
	//r wall
	//rect enemic

	if (r.x + r.w < rect.x)
	{
		return LEFT_COLLISION;
	}

	else if (r.x > rect.x + rect.w)
	{
		return RIGHT_COLLISION;
	}

	else if (r.y <= rect.y + rect.h && rect.x > r.x)
	{
		return UP_COLLISION;
	}



	else return NONE_COLLISION;
}

