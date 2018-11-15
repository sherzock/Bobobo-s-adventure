
#include "j1Entity.h"
#include "j1Collisions.h"
#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"

j1Entity::j1Entity(EntityTypes type) : type(type)
{
}
void j1Entity::Draw()
{
	position = { (int)position.x, (int)position.y };
	App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()),SDL_FLIP_NONE);
}

