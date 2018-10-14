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
#include "j1FadeToBlack.h"
#include "j1Collisions.h"


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
	
	if (App->scene->active == true)
		active = false;
	
	if (active == true) {
	App->map->Load("Level2map.tmx");
	//App->map->Load("Testmap2.tmx");
	}
	
		
	

	
	return true;
}

// Called each loop iteration
bool j1Scene2::PreUpdate()
{
	

	return true;
}

// Called each loop iteration
bool j1Scene2::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");


	if (App->render->camera.x > -6500)
	{
		App->render->camera.x = -App->play->position.x + 400;
		if (App->render->camera.x > 0)
		{
			App->render->camera.x = 0;
		}

	}


	App->map->Draw();



	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene2::PostUpdate()
{
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
	App->play->CleanUp();
	return true;
}

void j1Scene2::change_scenes2() {
	
	App->scene->active = true;
	App->scene2->active = false;
	CleanUp();
	App->scene2->CleanUp();
	App->fade->FadeToBlack(App->scene2, App->scene, 0.8f);
	App->scene->Start();
	App->play->Start();

}