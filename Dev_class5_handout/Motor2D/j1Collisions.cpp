#include "j1Collisions.h"
#include "p2Defs.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Map.h"
#include "SDL\include\SDL.h"

j1Collisions::j1Collisions() : j1Module()
{

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i) {
		colliders[i] = nullptr;
	}
	name.create("collisions");

	matrix[PLAYER_COLLIDER][PLAYER_COLLIDER] = false;
	matrix[PLAYER_COLLIDER][GROUND_COLLIDER] = true;
	
	matrix[GROUND_COLLIDER][PLAYER_COLLIDER] = true;
	matrix[GROUND_COLLIDER][GROUND_COLLIDER] = false;
	
}

j1Collisions::~j1Collisions() {}

bool j1Collisions::PreUpdate(){

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i) {
		
		if (colliders[i] != nullptr) {
			
			if (colliders[i]->to_delete) {

				delete colliders[i];
				colliders[i] = nullptr;
			
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

void j1Collisions::Collider_to_debug() {

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		debug = !debug;
	}

	if (debug == false) {
	}
		
	
	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case NO_COLLIDER:														
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0 , 30);  //yellow
			break;
		case GROUND_COLLIDER:																
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, 75); //green
			break;
		case PLAYER_COLLIDER:																
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, 75); //blue
			break;

		}
	}
}

bool j1Collisions::Update(float dt) {

	Collider* collider1;
	Collider* collider2;

	for (uint i = 0; i < MAX_NUM_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;
		if (colliders[i]->type == PLAYER_COLLIDER)
		{

			collider1 = colliders[i]; 

			for (uint j = 0; j < MAX_NUM_COLLIDERS; ++j)
			{
				/*if (colliders[j] == nullptr || i == j)  // if collider is nullptr or the player collider itself LATER Aixo es un tema de optimitzacio pero per ara res
					continue;*/

				collider2 = colliders[j];

				if (collider1->Check_Collision(collider2->rect) == true)
				{
					//if (matrix[collider1->type][collider2->type] && collider1->callback)   NO VA
						//collider1->callback->OnCollision(collider1, collider2); // What the  LATER
				}
			}
		}
	}

	Collider_to_debug();

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

	int close = 0; // NO VA

	return true;
}


/*
CollisionDirection Collider::Check_Direction(const SDL_Rect& r) const {
}*/

