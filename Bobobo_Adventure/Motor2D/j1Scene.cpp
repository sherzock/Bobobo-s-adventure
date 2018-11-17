#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
	//App->map->Load("Testmap2.tmx");

	if (active == true)	{
	
		App->map->Load("Level1map.tmx");
		App->audio->PlayMusic("audio/music/level1.ogg", 1.0f);
	}

	if (playeron == false)
	{
		App->enty->CreatePlayer();
		playeron = true;
	}

	App->enty->AddEnemy(20, 20, FLYINGENEMY);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");


	if (App->render->camera.x > -6500)
	{
		App->render->camera.x = -App->enty->player->position.x+ 400;
		if (App->render->camera.x > 0)
		{
			App->render->camera.x = 0;
		}
			
	}

	
	App->map->Draw();


	
	
	p2SString title("Bobobo Adventure");

	App->win->SetTitle(title.GetString());
	return true;
	
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->map->CleanUp();
	App->colls->CleanUp();
	App->tex->CleanUp();
	App->enty->player->CleanUp();
	App->enty->CleanUp();

	return true;
	
}



void j1Scene::change_scenes1(){
	App->scene2->active = true;
	App->scene->active = false;
	CleanUp();
	App->scene->CleanUp();
	App->fade->FadeToBlack(App->scene, App->scene2, 0.8f);
	App->scene2->Start();
	App->enty->player->Start();
	
}