#include "ImageUI.h"
#include "j1Gui.h"
#include "j1UIItems.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Brofiler\Brofiler.h"

ImageUI::ImageUI(Position alig) : j1UIItems(Type::IMAGE, Position::NO_POSITIONING)
{
	ali = alig;
}

ImageUI::~ImageUI()
{}

bool ImageUI::Start()
{
	if (texture == nullptr) { texture = App->gui->GetAtlas(); }

	return true;
}

bool ImageUI::CleanUp()
{
	if (texture != nullptr)
		App->tex->UnLoad(texture);
	deleting = true;
	return true;
}

bool ImageUI::Update(float dt)
{
	BROFILER_CATEGORY("GuiImage Update", Profiler::Color::MediumVioletRed);

	UpdatePosition();
	if (show) { App->render->Blit(texture, position.x + movement.x, position.y + movement.y, &rect); }

	return true;
}

void ImageUI::Move(iPoint displace)
{
	movement.x += displace.x;
	movement.y += displace.y;

}