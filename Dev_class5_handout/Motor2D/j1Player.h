#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Module.h"

struct SDL_Texture;


class j1Player : public j1Module
{
public:

	j1Player();

	Collider* player;
	SDL_Texture* graphics = nullptr;
	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	//oncollision checks if its colliding
	void OnCollision(Collider* col_1, Collider* col_2);

public:
	// To know the last direction the character was moving to
	fPoint position;

	float XSpeed = 0.25f;
	float initialspeed = 0.02f;
	float JumpSpeed = -0.72f;
	float gravity = 0.0f;

	uint currentTime;
	uint lastTime;

	bool updatedTime = false;
	bool dead = false;
	bool GroundCollision = false;
	bool jump = false;

	
};

#endif // __jPLAYER_H__
