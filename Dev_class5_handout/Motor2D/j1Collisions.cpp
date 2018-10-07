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

bool j1Collisions::PreUpdate()
{
	return false;
}

bool j1Collisions::CleanUp()
{
	return false;
}

bool j1Collisions::Update(float dt) {

	return false;
}

void j1Collisions::Collider_to_debug() {

	
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	return false;
}
uint j1Collisions::Colliders_Count() const {

	return 1;
}
bool j1Collisions::Delete_Collider(Collider* collider)
{
	return false;
}

bool Collider::Check_Collision(const SDL_Rect& r) const {

	return false;
}
/*
CollisionDirection Collider::Check_Direction(const SDL_Rect& r) const {

	return 0;
}*/

