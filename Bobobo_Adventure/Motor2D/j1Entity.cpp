
#include "j1Entity.h"
#include "j1Collisions.h"
#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"

j1Entity::j1Entity(int x, int y, entitytypes type) : position(x, y), type(type){
}

void j1Entity::Draw(){
	if (goingright == true) {
		App->render->Blit(sprites, (int)position.x, (int)position.y , &(animation->GetCurrentFrame()), SDL_FLIP_NONE);
	}
		
	else {
		App->render->Blit(sprites, (int)position.x , (int)position.y , &(animation->GetCurrentFrame()), SDL_FLIP_HORIZONTAL);
	}
		
}

