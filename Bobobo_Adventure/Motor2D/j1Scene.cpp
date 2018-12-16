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
#include "j1MainMenuScene.h"
#include "j1Player.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Gui.h"
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
	
	if (App->scene2->active == true || App->menuscene->active == true) {
		active = false;
	}
	
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

		App->audio->PlayMusic("audio/music/level1.ogg");
		
		if (playeron == false || App->enty->player->dead == true)
		{
			App->enty->CreatePlayer();
			playeron = true;
		}


		
		timer.Start();
	}

	if (active == true) {
	AddAllEnemies();
	}
	
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene1 PreUpdate", Profiler::Color::MediumVioletRed)
	
		if (WantToLoad == true) {
			App->LoadGame("save_game.xml");
			WantToLoad = false;
		}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene1 Update", Profiler::Color::Tomato)

		sctime = timer.ReadSec();

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
	
	
		if(deadrestart == true) {
			App->enty->player->playerlifes = 3;
			App->enty->player->playerpoints = 0;
			change_scenesmainmenu();
		}
	

		if (volslider != nullptr) {
			if (volslider->lastpos < volslider->pos.x)
			{
				if (App->audio->volume_music < App->audio->max_volume)
					App->audio->volume_music = App->audio->volume_music + 3;
				App->audio->setup_volume_fx = true;
				Mix_VolumeMusic(App->audio->volume_music);
				LOG("Volume = %d", Mix_VolumeMusic(App->audio->volume_music));
			}
			else if (volslider->lastpos > volslider->pos.x)
			{
				if (App->audio->volume_music > 2)
					App->audio->volume_music = App->audio->volume_music - 3;
				else if (App->audio->volume_music > 1)
					App->audio->volume_music = App->audio->volume_music - 2;
				else if (App->audio->volume_music > 0)
					App->audio->volume_music = App->audio->volume_music - 1;
				Mix_VolumeMusic(App->audio->volume_music);
				App->audio->setdown_volume_fx = true;
				LOG("Volume = %d", Mix_VolumeMusic(App->audio->volume_music));
			}
		}

		if (volslider != nullptr) {
			volslider->lastpos = volslider->pos.x;
			volslider->limit = { volrail->position.x + volrail->movement.x,volrail->position.x + volrail->rect.w + volrail->movement.x };
		}

	return true;
	
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene1 PostUpdate", Profiler::Color::MediumVioletRed)

	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (!iswindowon) {
			createescwindow();
		}
		else if (iswindowon) {
			destroyescwindow();
		}
		
	}
		

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	//destroyescwindow();
	App->map->CleanUp();
	App->colls->CleanUp();
	App->tex->CleanUp();
	App->enty->CleanUp();
	

	return true;
	
}

bool j1Scene::OnEventChange(j1UIItems* item, Event evnt) 
{
	escwindow->ChangeEvent(item, evnt);
	escmainmenu->ChangeEvent(item, evnt);
	escresume->ChangeEvent(item, evnt);
	volslider->ChangeEvent(item, evnt);
	switch (evnt)
	{
	case Event::LEFT_CLICK:
		if (item == volslider)
		{
			notonthewindow = true;
			escwindow->kinetic = false;
			volslider->movable = true;
		}
		else if (item == escwindow) {
			if (!notonthewindow) {
				escwindow->kinetic = true;
			}
		}
		else if (item == escmainmenu) {
			change_scenesmainmenu();
		}
		else if (item == escresume) {
			destroyescwindow();
		}
		break;
	case Event::LEFT_CLICK_UP:
		if (item == escwindow) {
			escwindow->kinetic = false;
		}
		break;
	case Event::MOUSE_OUTSIDE:
		if (item == volslider)
		{
			volslider->movable = false;
			notonthewindow = false;
		}
		break;
	}
	return true;
}

void j1Scene::change_scenes1(){
	
	App->GamePause = false;
	deadrestart = false;
	App->scene2->active = true;
	App->scene->active = false;
	CleanUp();
	App->gui->CleanUp();
	App->enty->CleanUp();
	App->fade->FadeToBlack(App->scene, App->scene2, 0.8f);
	App->enty->CreatePlayer();
	App->scene2->Start();
	App->enty->Start();
	
	
	

}

void j1Scene::change_scenesmainmenu()
{
	App->GamePause = false;
	deadrestart = false;
	App->menuscene->active = true;
	App->scene->active = false;
	CleanUp();
	App->fade->FadeToBlack(App->scene, App->menuscene, 0.8f);
	App->enty->player->CleanUp();
	App->menuscene->Start();
	
	
	
}

void j1Scene::AddAllEnemies() {

	App->enty->AddEnemy(300, 530, COIN);
	App->enty->AddEnemy(500, 530, WALKINGENEMY);
	App->enty->AddEnemy(800, 470, FLYINGENEMY);
	App->enty->AddEnemy(1500, 100, FLYINGENEMY);
	App->enty->AddEnemy(3500, 100, FLYINGENEMY);
	App->enty->AddEnemy(6500, 500, WALKINGENEMY);
	App->enty->AddEnemy(7500, 400, FLYINGENEMY);
	App->enty->AddEnemy(4500, 400, FLYINGENEMY);
	App->enty->AddEnemy(4600, 450, WALKINGENEMY);
}

void j1Scene::createescwindow() {
	App->GamePause = true;
	iswindowon = true;
	escwindow = App->gui->WindowCreate(POSITION_CENTER, 0, nullptr, { 0,50 }, this);
	escwindow->texture = escwindow->TexLoad("textures/Ui/PauseSet.png");
	escwindow->rect = { 0,0, 416, 588 };
	escwindow->kinetic = false;
	escmainmenu;

	escresume = App->gui->CreateButton(POSITION_CENTER, nullptr, { 0,200 }, this);
	escresume->SettleTextureToButton("textures/ui/Resume.png", "textures/ui/ResumeHov.png", "textures/ui/ResumeClicked.png");
	escresume->rect = { 0,0, 337, 113 };
	escwindow->AddItemToWindow(escresume);

	escmainmenu = App->gui->CreateButton(POSITION_CENTER, nullptr, { 0,330 }, this);
	escmainmenu->SettleTextureToButton("textures/ui/MainMenu.png", "textures/ui/MainMenuHov.png", "textures/ui/MainMenuClicked.png");
	escmainmenu->rect = { 0,0, 337, 113 };
	escwindow->AddItemToWindow(escmainmenu);
	
	volrail = App->gui->CreateImage(POSITION_CENTER, "textures/ui/MainMenu/Settings/VolumeBar.png", { 0, 0, 308, 9 }, { 0, 500 });
	escwindow->AddItemToWindow(volrail);

	volslider = App->gui->CreateSlider(POSITION_CENTER, nullptr, { 0, 475 }, this);
	volslider->SettleTextureToSlider("textures/ui/MainMenu/Settings/VolumeBarMovingPart.png", "textures/ui/MainMenu/Settings/VolumeBarMovingPart.png", "textures/ui/MainMenu/Settings/VolumeBarMovingPart.png");
	volslider->rect = { 0,0, 42, 50 };
	volslider->movable = false;
	volslider->limit = { volrail->position.x+volrail->movement.x,volrail->position.x+volrail->rect.w+volrail->movement.x };
	escwindow->AddItemToWindow(volslider);
}

void j1Scene::destroyescwindow() {
	App->GamePause = false,
	iswindowon = false;
	if (escwindow != nullptr) {
		escwindow->CleanUp();
	}
	if (escresume!=nullptr) {
		escresume->CleanUp();
	}
	if (escmainmenu  != nullptr) {
		escmainmenu->CleanUp();
	}
	if (volrail != nullptr) {
		volrail->CleanUp();
	}
	if (volslider != nullptr) {
		volslider->CleanUp();
	}
}