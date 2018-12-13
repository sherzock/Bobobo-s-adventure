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

	if (active == true) {

		MainMenubg = App->gui->CreateImage(POSITION_CENTER, "textures/Ui/IntroScene.png", { 0, 0, 1024, 768 }, { 0, 0 });
		MainMenuTitle = App->gui->CreateImage(POSITION_CENTER, "textures/ui/MainMenu/Title.png", { 0, 0, 635, 120 }, { 0, 30 });
		
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
	playbutt->ChangeEvent(elem, evnt);
	switch (evnt)
	{
	case Event::LEFT_CLICK:

		if (elem == playbutt)
		{
			change_scenes0();
			//ShellExecuteA(NULL, "open", "https://www.youtube.com/watch?v=7SRAIIkYyAo", NULL, NULL, SW_SHOWNORMAL);
			//window->CleanUp();
		}
		break;
	}
	return true;
}

void j1MainMenuScene::change_scenes0() {
	App->scene->active = true;
	App->menuscene->active = false;
	CleanUp();
	App->fade->FadeToBlack(App->menuscene, App->scene, 0.8f);
	App->scene->Start();
	App->enty->CreatePlayer();
	App->enty->player->XSpeed = 0;
	App->enty->player->position.x = 30;
	App->enty->player->position.y = 550;
}