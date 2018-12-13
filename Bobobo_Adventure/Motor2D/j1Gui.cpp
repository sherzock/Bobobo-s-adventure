#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
//#include "Brofiler/Brofiler.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	//	BROFILER_CATEGORY("GUI_PreUpdate", Profiler::Color::OrangeRed);
	bool ret = true;

	for (p2List_item<j1UIItems*>* item = items.start; item != NULL; item = item->next)
	{
		item->data->PreUpdate();

		if (item->data->deleting)
			ItemDestory(item->data);
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debug = !debug;
	}

	ItemEventUpdate();

	ret = ItemEventManager();

	return ret;
}

bool j1Gui::Update(float dt)
{
	//BROFILER_CATEGORY("GUI_Update", Profiler::Color::OrangeRed);
	for (p2List_item<j1UIItems*>* item = items.start; item != NULL; item = item->next)
	{
		item->data->Update(dt);

		if (debug)
		{
			item->data->ShowDebug();
		}
	}
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	App->tex->UnLoad(atlas);
	p2List_item<j1UIItems*>* item = items.start;

	if (item != NULL) {

		for (; item != NULL; item = item->next) {

			item->data->CleanUp();

			ItemDestory(item->data);
		}

		items.clear();

	}

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

void j1Gui::ItemEventUpdate() const
{
	iPoint auxposition;
	App->input->GetMousePosition(auxposition.x, auxposition.y);
	auxposition.x -= App->render->camera.x;

	p2List_item<j1UIItems*>* item;
	for (item = items.start; item != NULL; item = item->next)
	{
		iPoint item_pos = { item->data->position.x + item->data->movement.x,item->data->position.y + item->data->movement.y };
		if ((auxposition.x > item_pos.x && auxposition.x < item_pos.x + item->data->rect.w) && (auxposition.y > item_pos.y && auxposition.y < item_pos.y + item->data->rect.h))
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) { item->data->event = Event::RIGHT_CLICK; }
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP) { item->data->event = Event::RIGHT_CLICK_UP; break; }
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) { item->data->event = Event::LEFT_CLICK; }
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) { item->data->event = Event::LEFT_CLICK_UP; break; }
			else { item->data->event = Event::MOUSE_INSIDE; }
		}
		else { item->data->event = Event::MOUSE_OUTSIDE; }
	}
}



bool j1Gui::ItemEventManager()
{
	bool ret = true;
	for (p2List_item<j1UIItems*>* item = items.end; item != NULL && ret != false; item = item->prev)
	{
		if ((item->data->event != item->data->event_before) && item->data->auxmodule != nullptr) {

			ret = item->data->auxmodule->OnEventChange(item->data, item->data->event);
			item->data->event_before = item->data->event;
		}
	}
	return ret;
}



void j1Gui::ItemDestory(j1UIItems* item)
{
	int aux = items.find(item);

	RELEASE(items.At(aux)->data);

	items.del(items.At(aux));
}



j1UIItems* j1Gui::CreateItem(Type guitype, Position ali)
{
	j1UIItems* ret = nullptr;


	if (guitype == IMAGE) {

		ret = new ImageUI(ali);
		items.add(ret);

	}
	else if (guitype == BUTTON) {

		ret = new ButtonUI(ali);
		items.add(ret);

	}
	else if (guitype == LABEL) {

		ret = new LabelUI(ali);
		items.add(ret);

	}
	else if (guitype == WINDOW) {

		ret = new WindowUI(ali);
		items.add(ret);

	}
	return ret;
}



ImageUI* j1Gui::CreateImage(Position ali, char* path, SDL_Rect texture, iPoint movement, j1Module* auxmodule)
{
	ImageUI* image = (ImageUI*)App->gui->CreateItem(IMAGE, ali);
	image->rect = texture;
	image->movement = movement;
	image->auxmodule = auxmodule;
	image->Start();
	if (path != nullptr)
	{
		p2SString pat = path;
		image->tex = image->TexLoad(pat.GetString());
	}
	return image;
}



ButtonUI* j1Gui::CreateButton(Position ali, p2SString text, iPoint movement, j1Module* auxmodule)
{
	ButtonUI* button = (ButtonUI*)App->gui->CreateItem(BUTTON, ali);
	button->movement = movement;
	button->auxmodule = auxmodule;
	button->Start();
	return button;
}



LabelUI* j1Gui::CreateText(Position ali, p2SString text, iPoint movement, Fonts font, SDL_Color color, j1Module* auxmodule)
{
	LabelUI* tex = (LabelUI*)App->gui->CreateItem(LABEL, ali);
	tex->CreateText(text, color, font);
	tex->movement = movement;
	tex->auxmodule = auxmodule;
	tex->Start();
	return tex;
}



WindowUI* j1Gui::WindowCreate(Position ali, uint ButtonCount_, p2SString title, iPoint movement, j1Module* auxmodule)
{
	WindowUI* window = (WindowUI*)App->gui->CreateItem(WINDOW, ali);
	window->movement = movement;
	window->ButtonCount = ButtonCount_;
	window->title = title;
	window->auxmodule = auxmodule;
	window->Start();
	return window;
}