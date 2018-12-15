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




	return true;
}


bool j1MainMenuScene::PostUpdate()
{
	bool ret = true;

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
	switch (evnt)
	{
	case Event::LEFT_CLICK:
		
		if (elem == playbutt)
		{
			App->audio->PlayFx(Click_fx);
			change_scenes0();
			//ShellExecuteA(NULL, "open", "https://www.youtube.com/watch?v=7SRAIIkYyAo", NULL, NULL, SW_SHOWNORMAL);
			//window->CleanUp();
		}
		else if (elem == creditsbutt)
		{
			App->audio->PlayFx(Click_fx);
			playbutt->CleanUp();
			continuebutt->CleanUp();
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
			continuebutt->CleanUp();
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
			createmainmenu();

		}
		else if (elem = continuebutt)
		{
			App->audio->PlayFx(Click_fx);
			WantToLoad = true;
			change_scenes0();

		
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
	App->scene->Start();
	App->enty->active = true;
	
	App->fade->FadeToBlack(App->menuscene, App->scene, 0.8f);
	App->enty->player->position.x = 30;
	App->enty->player->position.y = 550;
	App->enty->player->XSpeed = 0;
	//App->enty->CreatePlayer();
	App->enty->Start();
	
}

void j1MainMenuScene::createmainmenu()
{
	playbutt = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,180 }, this);
	playbutt->SettleTextureToButton("textures/ui/MainMenu/Play.png", "textures/ui/MainMenu/PlayHov.png", "textures/ui/MainMenu/PlayClicked.png");
	playbutt->rect = { 0,0, 340, 120 };

	continuebutt = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,325 }, this);
	continuebutt->SettleTextureToButton("textures/ui/MainMenu/Continue.png", "textures/ui/MainMenu/ContinueHov.png", "textures/ui/MainMenu/ContinueClicked.png");
	continuebutt->rect = { 0,0, 255, 90 };

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
	
	creditstxt = App->gui->CreateText(POSITION_RIGHT, "hello", { -0,0 }, ALLER_LT, { 255, 255, 255, 255 });
	creditsback = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,626 }, this);
	creditsback->SettleTextureToButton("textures/ui/MainMenu/Settings/Back.png", "textures/ui/MainMenu/Settings/BackHov.png", "textures/ui/MainMenu/Settings/BackClicked.png");
	creditsback->rect = { 0,0, 50, 46 };
}

void j1MainMenuScene::createsettings()
{
	settingsback = App->gui->CreateButton(POSITION_RIGHT, nullptr, { -50,626 }, this);
	settingsback->SettleTextureToButton("textures/ui/MainMenu/Settings/Back.png", "textures/ui/MainMenu/Settings/BackHov.png", "textures/ui/MainMenu/Settings/BackClicked.png");
	settingsback->rect = { 0,0, 50, 46 };
	volumetitle = App->gui->CreateImage(POSITION_RIGHT, "textures/ui/MainMenu/Settings/Volume.png", { 0, 0, 336, 112 }, { -45, 250 });
}