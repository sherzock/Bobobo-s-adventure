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
	
	
	if (active == true)	{
	
		if (App->map->Load("Level1map.tmx"))
		{
			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data))
			{
				App->path->SetMap(w, h, data);
			}

			RELEASE_ARRAY(data);
		}
		debug_tex = App->tex->Load("maps/path2.png");

		App->audio->PlayMusic("audio/music/level1.ogg", 1.0f);
	}

	if (playeron == false)
	{
		App->enty->CreatePlayer();
		playeron = true;
	}
	AddAllEnemies();

	
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene1 PreUpdate", Profiler::Color::MediumVioletRed)

		static iPoint origin;
	static bool origin_selected = false;

	if (App->colls->ShowColliders) {
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (origin_selected == true)
			{
				App->path->CreatePath(origin, p);
				origin_selected = false;
			}
			else
			{
				origin = p;
				origin_selected = true;
			}
		}
	}

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
	
	
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN || App->enty->player->position.x >= App->map->map_file.child("map").child("properties").child("property").next_sibling("property").next_sibling("property").next_sibling("property").attribute("value").as_float() - 300) {
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


	if (App->colls->ShowColliders) {
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);

		// Debug pathfinding ------------------------------
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);
		p = App->map->MapToWorld(p.x, p.y);

		App->render->Blit(debug_tex, p.x, p.y);

		const p2DynArray<iPoint>* path = App->path->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}
	


	
	
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

void j1Scene::AddAllEnemies() {


	App->enty->AddEnemy(500, 400, WALKINGENEMY);
}