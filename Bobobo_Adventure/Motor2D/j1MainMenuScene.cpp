#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1UIItems.h"
#include "shellapi.h"
#include "j1UIItems.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1MainMenuScene.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"
#include "Brofiler/Brofiler.h"


j1MainMenuScene::j1MainMenuScene() : j1Module()
{
	name.create("Menuscene");
}


j1MainMenuScene::~j1MainMenuScene()
{}


bool j1MainMenuScene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool j1MainMenuScene::Start()
{
	Click_fx = App->audio->LoadFx("audio/fx/Click.wav");
	if (active == true) {
		App->audio->PlayMusic("audio/music/Menu.ogg");
		MainMenubg = App->gui->CreateImage(POSITION_CENTER, "textures/Ui/IntroScene.png", { 0, 0, 1024, 768 }, { 0, 0 });
		MainMenuTitle = App->gui->CreateImage(POSITION_CENTER, "textures/ui/MainMenu/Title.png", { 0, 0, 635, 120 }, { 0, 30 });
		CanLoadGame();
		createmainmenu();
	}
	return true;
}


bool j1MainMenuScene::PreUpdate()
{
	


	return true;
}


bool j1MainMenuScene::Update(float dt)
{
	if (volslider != nullptr) {
			if(volslider->lastpos < volslider->pos.x)
			{
				if (App->audio->volume_music < App->audio->max_volume)
					App->audio->volume_music++;
				App->audio->setup_volume_fx = true;
				Mix_VolumeMusic(App->audio->volume_music);
				LOG("Volume = %d", Mix_VolumeMusic(App->audio->volume_music));
			}
			else if (volslider->lastpos > volslider->pos.x)
			{
				if (App->audio->volume_music > 0)
					App->audio->volume_music--;
				Mix_VolumeMusic(App->audio->volume_music);
				App->audio->setdown_volume_fx = true;
				LOG("Volume = %d", Mix_VolumeMusic(App->audio->volume_music));
			}
	}
	

	return true;
}


bool j1MainMenuScene::PostUpdate()
{
	bool ret = true;
	if (volslider != nullptr) {
		volslider->lastpos = volslider->pos.x;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


bool j1MainMenuScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool j1MainMenuScene::OnEventChange(j1UIItems* elem, Event evnt)
{
	bool ret = true;
	playbutt->ChangeEvent(elem, evnt);
	continuebutt->ChangeEvent(elem, evnt);
	settingsbutt->ChangeEvent(elem, evnt);
	creditsbutt->ChangeEvent(elem, evnt);
	exitbutt->ChangeEvent(elem, evnt);
	creditsback->ChangeEvent(elem, evnt);
	volslider->ChangeEvent(elem, evnt);
	switch (evnt)
	{
	case Event::LEFT_CLICK:
		
		if (elem == playbutt)
		{
			App->audio->PlayFx(Click_fx);
			change_scenes0();
		}
		else if (elem == creditsbutt)
		{
			App->audio->PlayFx(Click_fx);
			playbutt->CleanUp();
			if (coninuebuttoff != nullptr) {
				coninuebuttoff->CleanUp();
			}
			if (continuebutt != nullptr) {
				continuebutt->CleanUp();
			}
			settingsbutt->CleanUp();
			creditsbutt->CleanUp();
			exitbutt->CleanUp();
			createcredits();
		}
		else if (elem == creditsback)
		{
			App->audio->PlayFx(Click_fx);
			creditstxt->CleanUp();
			creditsback->CleanUp();
			github->CleanUp();
			createmainmenu();
		
		}
		else if (elem == exitbutt)
		{
			App->audio->PlayFx(Click_fx);
			ret = false;

		}
		else if (elem == settingsbutt)
		{
			App->audio->PlayFx(Click_fx);
			playbutt->CleanUp();
			if (coninuebuttoff != nullptr) {
				coninuebuttoff->CleanUp();
			}
			if (continuebutt != nullptr) {
				continuebutt->CleanUp();
			}
			settingsbutt->CleanUp();
			creditsbutt->CleanUp();
			exitbutt->CleanUp();
			createsettings();

		}
		else if (elem == settingsback)
		{
			App->audio->PlayFx(Click_fx);
			volumetitle->CleanUp();
			settingsback->CleanUp();
			volslider->CleanUp();
			volrail->CleanUp();
			createmainmenu();

		}
		else if (elem == continuebutt)
		{
			App->audio->PlayFx(Click_fx);
			App->scene->WantToLoad = true;
			change_scenes0();

		
		}
		else if (elem == volslider)
		{
			volslider->movable = true;
		}
		else if (elem == github)
		{
			ShellExecuteA(NULL, "open", "https://github.com/sherzock/Bobobo-s-bizarre-adventure", NULL, NULL, SW_SHOWNORMAL);
		}
		break;
	case Event::LEFT_CLICK_UP:
		if (elem == volslider)
		{
			volslider->movable = false;
		}
		break;
	case Event::MOUSE_OUTSIDE:
		if (elem == volslider)
		{
			volslider->movable = false;
		}
		break;
	}
	return ret;
}

void j1MainMenuScene::change_scenes0() {
	App->scene->active = true;
	App->menuscene->active = false;
	if (MainMenubg != nullptr) {
		MainMenubg->CleanUp();
	}
	if (MainMenuTitle != nullptr) {
		MainMenuTitle->CleanUp();
	}
	if (playbutt != nullptr) {
		playbutt->CleanUp();
	}
	if (continuebutt != nullptr) {
		continuebutt->CleanUp();
	}
	if (settingsbutt != nullptr) {
		settingsbutt->CleanUp();
	}
	if (creditsbutt != nullptr) {
		creditsbutt->CleanUp();
	}
	if (exitbutt != nullptr) {
		exitbutt->CleanUp();
	}
	if (settingsback != nullptr) {
		settingsback->CleanUp();
	}
	if (volumetitle != nullptr) {
		volumetitle->CleanUp();
	}
	if (creditsback != nullptr) {
		creditsback->CleanUp();
	}
	if (creditstxt != nullptr) {
		creditstxt->CleanUp();
	}
	if (volrail != nullptr) {
		volrail->CleanUp();
	}
	if (volslider != nullptr) {
		volslider->CleanUp();
	}
	if (coninuebuttoff != nullptr) {
		coninuebuttoff->CleanUp();
	}
	if (github != nullptr) {
		github->CleanUp();
	}
	App->scene->Start();
	App->enty->active = true;
	
	App->fade->FadeToBlack(App->menuscene, App->scene, 0.8f);
	App->enty->player->position.x = 30;
	App->enty->player->position.y = 550;
	App->enty->player->XSpeed = 0;
	App->enty->Start();
	
}

void j1MainMenuScene::createmainmenu()
{
	playbutt = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,180 }, this);
	playbutt->SettleTextureToButton("textures/ui/MainMenu/Play.png", "textures/ui/MainMenu/PlayHov.png", "textures/ui/MainMenu/PlayClicked.png");
	playbutt->rect = { 0,0, 340, 120 };

	if (candocontinue == true) {
		continuebutt = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,325 }, this);
		continuebutt->SettleTextureToButton("textures/ui/MainMenu/Continue.png", "textures/ui/MainMenu/ContinueHov.png", "textures/ui/MainMenu/ContinueClicked.png");
		continuebutt->rect = { 0,0, 255, 90 };
	}
	else {
		coninuebuttoff = App->gui->CreateImage(POSITION_RIGHT, "textures/ui/MainMenu/ContinueDisable.png", { 0,0, 255, 90 }, { -50,325 });
		
	}

	settingsbutt = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,440 }, this);
	settingsbutt->SettleTextureToButton("textures/ui/MainMenu/Settings.png", "textures/ui/MainMenu/SettingsHov.png", "textures/ui/MainMenu/SettingsClicked.png");
	settingsbutt->rect = { 0,0, 200, 68 };

	creditsbutt = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,533 }, this);
	creditsbutt->SettleTextureToButton("textures/ui/MainMenu/Credits.png", "textures/ui/MainMenu/CreditsHov.png", "textures/ui/MainMenu/CreditsClicked.png");
	creditsbutt->rect = { 0,0, 200, 68 };

	exitbutt = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,626 }, this);
	exitbutt->SettleTextureToButton("textures/ui/MainMenu/Exit.png", "textures/ui/MainMenu/ExitHov.png", "textures/ui/MainMenu/ExitClicked.png");
	exitbutt->rect = { 0,0, 200, 68 };

}

void j1MainMenuScene::createcredits()
{
	
	creditstxt = App->gui->CreateImage(POSITION_RIGHT, "textures/Ui/MainMenu/Credits/CreditsInfo.png", { 0, 0, 631, 492 }, { -30, 150 });
	creditsback = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,676 }, this);
	creditsback->SettleTextureToButton("textures/ui/MainMenu/Settings/Back.png", "textures/ui/MainMenu/Settings/BackHov.png", "textures/ui/MainMenu/Settings/BackClicked.png");
	creditsback->rect = { 0,0, 50, 46 };
	github = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -20,20 }, this);
	github->SettleTextureToButton("textures/Ui/MainMenu/Credits/Github.png", "textures/Ui/MainMenu/Credits/Github.png", "textures/Ui/MainMenu/Credits/Github.png");
	github->rect = { 0,0, 102, 102 };
}

void j1MainMenuScene::createsettings()
{
	settingsback = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,676 }, this);
	settingsback->SettleTextureToButton("textures/ui/MainMenu/Settings/Back.png", "textures/ui/MainMenu/Settings/BackHov.png", "textures/ui/MainMenu/Settings/BackClicked.png");
	settingsback->rect = { 0,0, 50, 46 };
	volumetitle = App->gui->CreateImage(POSITION_RIGHT, "textures/ui/MainMenu/Settings/Volume.png", { 0, 0, 336, 112 }, { -45, 250 });
	volrail = App->gui->CreateImage(POSITION_RIGHT, "textures/ui/MainMenu/Settings/VolumeBar.png", { 0, 0, 308, 9 }, { -50, 500 });
	volslider = App->gui->CreateSlider(POSITION_RIGHT, nullptr, { -220, 475 }, this);
	volslider->SettleTextureToSlider("textures/ui/MainMenu/Settings/VolumeBarMovingPart.png", "textures/ui/MainMenu/Settings/VolumeBarMovingPart.png", "textures/ui/MainMenu/Settings/VolumeBarMovingPart.png");
	volslider->rect = { 0,0, 42, 50 };
	volslider->movable = false;
	volslider->limit = {657,950};
}

void j1MainMenuScene::CanLoadGame() {

	p2SString			load_game;
	load_game.create("save_game.xml");
	pugi::xml_document data;
	pugi::xml_parse_result result = data.load_file(load_game.GetString());
	if (result == NULL) {
		candocontinue = false;
	}
	else {
		candocontinue = true;
	}

}