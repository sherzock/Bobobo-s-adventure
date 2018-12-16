#include "j1PlayerUI.h"
#include "j1Gui.h"
#include "j1App.h"

#include "j1MainMenuScene.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"
#include "j1FadeToBlack.h"
#include "j1UIItems.h"


j1PlayerUI::j1PlayerUI(){}
j1PlayerUI::~j1PlayerUI(){}

bool j1PlayerUI::Start()
{
	heart = App->gui->CreateImage(POSITION_LEFT, "textures/Heart.png", { 0, 0, 44, 41 }, { 30, 30 });
	Coin = App->gui->CreateImage(POSITION_LEFT, "textures/Coin.png", { 0, 0, 38, 36 }, { 30, 700 });
	LifeCounter = App->gui->CreateText(POSITION_LEFT, "a", { 80, 25 }, SLKSCR, {255,255,255,255});
	CoinCounter = App->gui->CreateText(POSITION_LEFT, "a", { 80, 695 }, SLKSCR, { 255,255,255,255 });
	PointCounter = App->gui->CreateText(POSITION_RIGHT, "a", { -300, 25 }, SLKSCR, { 255,255,255,255 });
	TimeCounter = App->gui->CreateText(POSITION_RIGHT, "a", { -100, 75 }, SLKSCR, { 255,255,255,255 });
	/*window = App->gui->WindowCreate(POSITION_CENTER, 0, nullptr, { 0,0 });
	window->tex = window->TexLoad("textures/Ui/PauseSet.png");
	window->rect = { 0,0, 416, 588 };
	window->kinetic = false;
	window->movable = true;*/
	return true;
}

bool j1PlayerUI::Update(float dt)
{
	if (App->scene->active)
	{
		p2SString timescene = {"%i", App->scene->sctime};
		TimeCounter->UpdateText(timescene);
	}
	else if (App->scene2->active)
	{
		p2SString timescene = { "%i", App->scene2->sc2time };
		TimeCounter->UpdateText(timescene);
	}

	p2SString player_coins = { "%i", App->enty->player->numberofcoins };
	p2SString player_lifes = { "x %i", App->enty->player->playerlifes };
	p2SString player_points = { "Score %i", App->enty->player->playerpoints };
	LifeCounter->UpdateText(player_lifes);
	CoinCounter->UpdateText(player_coins);
	PointCounter->UpdateText(player_points);

	return true;
}

bool j1PlayerUI::CleanUp()
{
	if (heart != nullptr){
		heart->CleanUp();
	}
	if (Coin != nullptr){
		Coin->CleanUp();
	}
	if (LifeCounter != nullptr){
		LifeCounter->CleanUp();
	}
	if (TimeCounter != nullptr){
		TimeCounter->CleanUp();
	}
	if (CoinCounter != nullptr){
		CoinCounter->CleanUp();
	}
	if (PointCounter != nullptr){
		PointCounter->CleanUp();
	}
	if (window != nullptr){
		window->CleanUp();
	}
	return true;
}

bool j1PlayerUI::Load(pugi::xml_node& data)
{
	
	return true;
}

bool j1PlayerUI::Save(pugi::xml_node& data) const
{
	
	return true;
}

bool j1PlayerUI::OnEventChange(j1UIItems* item, Event evnt)
{
	/*window->ChangeEvent(item, evnt);*/

	switch (evnt)
	{
		case Event::LEFT_CLICK:
			
			if (item == window)
			{
				/*window->kinetic = true;*/
			}
		break;
		case Event::LEFT_CLICK_UP:
			if (item == window)
			{
				/*window->kinetic = false;*/
			}
		break;
	}
	
	return true;
}