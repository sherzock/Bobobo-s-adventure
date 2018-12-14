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
	Coin = App->gui->CreateImage(POSITION_RIGHT, "textures/Coin.png", { 0, 0, 38, 36 }, { -30, 30 });
	LifeCounter = App->gui->CreateText(POSITION_LEFT, "a", { 80, 25 }, SLKSCR, {255,255,255,255});
	return true;
}

bool j1PlayerUI::Update(float dt)
{
	p2SString player_lifes = { "x %i", App->enty->player->playerlifes };
	LifeCounter->UpdateText(player_lifes);

	return true;
}

bool j1PlayerUI::CleanUp()
{
	

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