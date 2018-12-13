#include "j1Gui.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1UIItems.h"

j1UIItems::j1UIItems(Type guitype, Position ali, j1Module* auxmodule) : guitype(guitype), ali(ali), auxmodule(auxmodule) {}
j1UIItems::~j1UIItems() {}
bool j1UIItems::Start() { return true; }
bool j1UIItems::Update(float dt) { return true; }
bool j1UIItems::CleanUp() { return true; }

void j1UIItems::UpdatePosition()
{
	if (ali == NO_POSITIONING) {

	}
	else if (ali == POSITION_CENTER) {
		position.x = (-App->render->camera.x + App->render->camera.w / 2) - (rect.w / 2);

		position.y = -App->render->camera.y;
	}
	else if (ali == POSITION_RIGHT) {
		position.x = (-App->render->camera.x + App->render->camera.w) - rect.w;

		position.y = -App->render->camera.y;
	}
	else if (ali == POSITION_LEFT) {
		position.x = -App->render->camera.x;

		position.y = -App->render->camera.y;
	}
}

SDL_Texture* j1UIItems::TexLoad(p2SString path)
{
	SDL_Texture* ret = App->tex->Load(path.GetString());
	return ret;
}


void j1UIItems::ShowDebug() { App->render->DrawQuad({ position.x + movement.x, position.y + movement.y, rect.w, rect.h }, 255, 255, 0, 100, false); }