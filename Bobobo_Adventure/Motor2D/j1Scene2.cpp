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
bool j1Scene2::PostUpdate()
{
	BROFILER_CATEGORY("Scene2 PostUpdate", Profiler::Color::MediumVioletRed)

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
bool j1Scene2::CleanUp()
{
	LOG("Freeing scene");
	//destroyescwindow();
	App->map->CleanUp();
	App->colls->CleanUp();
	App->tex->CleanUp();
	App->enty->CleanUp();
	return true;
}

bool j1Scene2::OnEventChange(j1UIItems* item, Event evnt)
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

void j1Scene2::change_scenes2() {
	
	deadrestart = false;	
	App->menuscene->active = true;
	App->scene2->active = false;
	CleanUp();
	App->enty->CleanUp();
	App->gui->CleanUp();
	App->fade->FadeToBlack(App->scene2, App->menuscene, 0.8f);
	App->enty->player->XSpeed = 0;
	App->enty->Start();
	App->enty->player->position.x = 30;
	App->enty->player->position.y = 550;
	App->scene->playeron = false;
	App->menuscene->Start();
	
	
	
}

void j1Scene2::change_scenesmainmenu()
{
	App->GamePause = false;
	deadrestart = false;
	App->menuscene->active = true;
	App->scene2->active = false;
	CleanUp();
	App->fade->FadeToBlack(App->scene2, App->menuscene, 0.8f);
	App->enty->player->CleanUp();
	App->menuscene->Start();



}

void j1Scene2::AddAllEnemies2() {


	App->enty->AddEnemy(400, 200, FLYINGENEMY);
	App->enty->AddEnemy(1000, 600, FLYINGENEMY);
	App->enty->AddEnemy(2300, 50, FLYINGENEMY);
	App->enty->AddEnemy(4000, 200, FLYINGENEMY);
	App->enty->AddEnemy(5000, 400, FLYINGENEMY);

}

void j1Scene2::createescwindow() {
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
	volslider->limit = { volrail->position.x + volrail->movement.x,volrail->position.x + volrail->rect.w + volrail->movement.x };
	escwindow->AddItemToWindow(volslider);

}

void j1Scene2::destroyescwindow() {
	App->GamePause = false,
		iswindowon = false;
	if (escwindow != nullptr) {
		escwindow->CleanUp();
	}
	if (escresume != nullptr) {
		escresume->CleanUp();
	}
	if (escmainmenu != nullptr) {
		escmainmenu->CleanUp();
	}

}