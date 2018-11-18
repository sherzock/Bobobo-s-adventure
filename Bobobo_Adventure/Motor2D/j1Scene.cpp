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
#include "j1EntityManager.h"
#include "Brofiler/Brofiler.h"

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


	App->enty->AddEnemy(300, 50, FLYINGENEMY);
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene1 PreUpdate", Profiler::Color::MediumVioletRed)
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene1 Update", Profiler::Color::Tomato)

		if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
			
			App->LoadGame("save_game.xml");
		}
		

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN){
		
		App->fade->FadeToBlack(this, App->scene);
		App->enty->player->position.x = App->enty->player->Initial_position.x;
		App->enty->player->position.y = App->enty->player->Initial_position.y;
		App->render->camera.x = 0;
	}
	
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {	
		change_scenes1();
			
	}
		

	if (App->render->camera.x > -7500)
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
	BROFILER_CATEGORY("Scene1 PostUpdate", Profiler::Color::MediumVioletRed)

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
	App->enty->CleanUp();

	return true;
	
}



void j1Scene::change_scenes1(){
	

	App->scene2->active = true;
	App->scene->active = false;
	CleanUp();
	App->enty->CleanUp();
	App->fade->FadeToBlack(App->scene, App->scene2, 0.8f);
	App->enty->CreatePlayer();
	App->enty->Start();
	App->scene2->Start();
	App->enty->player->position.x = 30;
	App->enty->player->position.y = 300;

}