#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1MainMenuScene.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1Collisions.h"
#include "Brofiler/Brofiler.h"
#include "j1Gui.h"


j1Scene2::j1Scene2() : j1Module()
{
	name.create("scene2");
	

}

// Destructor
j1Scene2::~j1Scene2()
{}

// Called before render is available
bool j1Scene2::Awake()
{
	
	LOG("Loading Scene2");
	bool ret = true;
	
	

	return ret;
}

// Called before the first frame
bool j1Scene2::Start()
{
	
	
	if (App->scene->active == true || App->menuscene->active == true) {
		active = false;
	}
		
	
	if (active == true) {
	App->map->Load("Level2map.tmx");
	App->audio->PlayMusic("audio/music/level2.ogg");
	//App->map->Load("Testmap2.tmx");
	}
	
	if (active == true) {
		AddAllEnemies2();
		timer.Start();
	}
	
	
	return true;
}

// Called each loop iteration
bool j1Scene2::PreUpdate()
{
	BROFILER_CATEGORY("Scene2 PreUpdate", Profiler::Color::MediumVioletRed)

	return true;
}

// Called each loop iteration
bool j1Scene2::Update(float dt)
{
	BROFILER_CATEGORY("Scene2 Update", Profiler::Color::Tomato)

		sc2time = timer.ReadSec();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");


	if (App->render->camera.x > -7500)
	{
		App->render->camera.x = -App->enty->player->position.x + 400;
		
		if (App->render->camera.x > 0)
		{
			App->render->camera.x = 0;
		}

	}


	

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN || App->enty->player->position.x >= App->map->map_file.child("map").child("properties").child("property").next_sibling("property").next_sibling("property").next_sibling("property").attribute("value").as_float() - 100) {
	
		change_scenes2();
		
	}


	if (App->input->GetKey(SDL_SCANCODE_F2) == j1KeyState::KEY_DOWN) {

		App->fade->FadeToBlack(this, this);
		App->enty->player->position.x = 30;
		App->enty->player->position.y = 300;
		App->render->camera.x = 0;



	}
	if (App->enty->player->win1 == true) {
		App->scene->change_scenes1();

	}
	else if (App->enty->player->win2 == true) {
		App->scene2->change_scenes2();
	}


	App->map->Draw();

		if (deadrestart == true) {
			App->enty->player->playerlifes = 3;
			App->enty->player->playerpoints = 0;
			change_scenes2();
		}


	return true;
}

// Called each loop iteration
bool j1Scene2::PostUpdate()
{
	BROFILER_CATEGORY("Scene2 PostUpdate", Profiler::Color::MediumVioletRed)

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");
	App->map->CleanUp();
	App->colls->CleanUp();
	App->tex->CleanUp();
	App->enty->CleanUp();
	return true;
}

void j1Scene2::change_scenes2() {
	
	deadrestart = false;	
	App->menuscene->active = true;
	App->scene2->active = false;
	CleanUp();
	App->enty->CleanUp();
	App->gui->CleanUp();
	App->fade->FadeToBlack(App->scene2, App->scene, 0.8f);
	App->enty->player->XSpeed = 0;
	App->enty->Start();
	App->enty->player->position.x = 30;
	App->enty->player->position.y = 550;
	App->menuscene->Start();
	
	
	
}

void j1Scene2::AddAllEnemies2() {


	App->enty->AddEnemy(400, 200, FLYINGENEMY);
	App->enty->AddEnemy(1000, 600, FLYINGENEMY);
	App->enty->AddEnemy(2300, 50, FLYINGENEMY);
	App->enty->AddEnemy(4000, 200, FLYINGENEMY);
	App->enty->AddEnemy(5000, 400, FLYINGENEMY);

}