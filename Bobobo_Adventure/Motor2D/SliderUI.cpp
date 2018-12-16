#include "SliderUI.h"
#include "ButtonUI.h"
#include "j1Gui.h"
#include "j1UIItems.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Textures.h"

SliderUI::SliderUI(Position alig) : j1UIItems(Type::SLIDER, Position::NO_POSITIONING) { ali = alig; }
SliderUI::~SliderUI() {}

bool SliderUI::Start()
{
	rect = { 0,0, 339, 63 };
	texture = None;
	return true;
}

bool SliderUI::CleanUp()
{

	deleting = true;

	App->tex->UnLoad(texture);
	App->tex->UnLoad(None);
	App->tex->UnLoad(Over);
	App->tex->UnLoad(Pressed);



	return true;
}

bool SliderUI::Update(float dt)
{
	if (kinetic) { Drag(); }
	if (active == false) { texture = Pressed; }
	UpdatePosition();
	App->render->Blit(texture, position.x + movement.x, position.y + movement.y, &rect);
	pos = { position.x + movement.x, position.y + movement.y };
	return true;
}

bool SliderUI::PostUpdate()
{
	return true;
}

void SliderUI::MouseInput(Input status)
{
	if (status == IDLE) {
		texture = None;
		inp = status;
	}
	else if (status == HOVER) {
		texture = Over;
		inp = status;
	}
	else if (status == PRESSED_L) {
		/*if (movable)*/
			StartDrag();
		
		if (Pressed != NULL)
			texture = Pressed;
		
		inp = status;
	}
	else if (status == UP_L) {
		/*if (movable)*/
			Enddrag();
		
		texture = None;
	}
}

void SliderUI::Drag(/*iPoint point*/)
{
	if (movable) {
		iPoint mouse_position;
		
		App->input->GetMousePosition(mouse_position.x, mouse_position.y);
		if (pos.x >= limit.x && pos.x <= limit.y)
		{
			movement.x += (mouse_position.x - OnClick.x);
			OnClick = mouse_position;
		}	
	}
}

void SliderUI::StartDrag()
{
	iPoint mouse_position;
	//movement = { 0,0 };
	App->input->GetMousePosition(mouse_position.x, mouse_position.y);
	OnClick = mouse_position;

}

void SliderUI::Enddrag() { OnClick = { 0,0 }; }




void SliderUI::Move(iPoint movem)
{
	movement.x += movem.x;
	movement.y += movem.y;
}

void SliderUI::SettleTextureToSlider(p2SString idle, p2SString hover, p2SString pressed) {

	None = TexLoad(idle);
	Over = TexLoad(hover);
	Pressed = TexLoad(pressed);
	texture = None;

}

void SliderUI::ChangeEvent(j1UIItems* item, Event event)
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