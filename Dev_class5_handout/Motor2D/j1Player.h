#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Animations.h"

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
	
	Animations* current_animation = nullptr;

	// Animations of the player
	Animations idle;
	Animations run;
	Animations jumpanim;
	Animations falling;
	
	
	fPoint position;

	float XSpeed;
	float initialspeed;
	float JumpSpeed;
	float gravity;
	float Jumpforce;
	float Jumpreset;
	float gravityMax;
	float gravityIteratior;
	float JumpforceIterator;
	float DashSpeed = 1.0f;
	float DashSpeedres = 1.0f;
	uint currentTime;
	uint lastTime;

	bool updatedTime = false;
	bool dead = false;
	bool GroundCollision = false;
	bool jump = false;
	bool CanPlayerJump = false;
	bool isfalling = false;
	bool goingright = true;
	bool godmode = false;
	bool dash;
};

#endif // __jPLAYER_H__
