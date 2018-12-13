#include "ButtonUI.h"
#include "j1Gui.h"
#include "j1UIItems.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"
//#include "Brofiler\Brofiler.h"

ButtonUI::ButtonUI(Position alig) : j1UIItems(Type::BUTTON, Position::NO_POSITIONING) { ali = alig; }
ButtonUI::~ButtonUI() {}

bool ButtonUI::Start()
{
	rect = { 0,0, 339, 63 };
	tex = None;
	return true;
}



bool ButtonUI::CleanUp()
{
	//App->tex->UnLoad(tex);

	deleting = true;
	if (tex != nullptr)
		App->tex->UnLoad(tex);
	App->tex->UnLoad(None);
	App->tex->UnLoad(Over);
	App->tex->UnLoad(Pressed);


	return true;
}



bool ButtonUI::Update(float dt)
{
	//BROFILER_CATEGORY("ButtonUI_Update", Profiler::Color::OrangeRed);
	if (active == false) { tex = Pressed; }
	UpdatePosition();
	App->render->Blit(tex, position.x + movement.x, position.y + movement.y, &rect);
	return true;
}



void ButtonUI::MouseInput(Input status)
{
	if (status == IDLE) {
		tex = None;
		inp = status;
	}
	else if (status == HOVER) {
		tex = Over;
		inp = status;
	}
	else if (status == PRESSED_L) {
		if (Pressed != NULL)
			tex = Pressed;
		inp = status;
	}
	else if (status == UP_L) {
		tex = None;
	}
}



void ButtonUI::Move(iPoint movem)
{
	movement.x += movem.x;
	movement.y += movem.y;
}



void ButtonUI::SettleTextureToButton(p2SString idle, p2SString hover, p2SString pressed) {

	None = TexLoad(idle);
	Over = TexLoad(hover);
	Pressed = TexLoad(pressed);
	tex = None;

}



void ButtonUI::ChangeEvent(j1UIItems* item, Event event)
{
	switch (event)
	{
	case Event::MOUSE_INSIDE:
		item->MouseInput(HOVER);
		break;
	case Event::MOUSE_OUTSIDE:
		item->MouseInput(IDLE);
		break;
	case Event::RIGHT_CLICK:
		item->MouseInput(PRESSED_R);
		break;
	case Event::LEFT_CLICK:
		item->MouseInput(PRESSED_L);
		break;
	case Event::LEFT_CLICK_UP:
		item->MouseInput(UP_L);
		break;
	case Event::RIGHT_CLICK_UP:
		item->MouseInput(UP_R);
		break;

	}
}