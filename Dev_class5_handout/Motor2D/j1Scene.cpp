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
	}
	
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


	if (App->render->camera.x > -5350)
	{
		App->render->camera.x = -App->play->position.x+ 400;
		if (App->render->camera.x > 0)
		{
			App->render->camera.x = 0;
		}
			
	}

	/*if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;*/

	//App->render->Blit(img, 0, 0);
	App->map->Draw();


	// TODO 7: Set the window title like
	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

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
	App->play->CleanUp();

	return true;
	
}



void change_scenes1(){
	App->scene2->active = true;
	App->scene->active = false;
	
	App->map->CleanUp();
	App->colls->CleanUp();
	App->tex->CleanUp();
	App->play->CleanUp();

	App->scene2->CleanUp();
	App->fade->FadeToBlack(App->scene2, App->scene, 0.8f);
	App->play->Start();
	App->render->camera = { 0,0 };
	App->scene2->Start();

}