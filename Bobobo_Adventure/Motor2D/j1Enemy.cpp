#include "j1App.h"
#include "j1Enemy.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Audio.h"


j1Enemy::j1Enemy(int x, int y) : position(x, y)
{
}

j1Enemy::~j1Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* j1Enemy::GetCollider() const
{
	return collider;
}

void j1Enemy::Draw(SDL_Texture* sprites, float flip)
{
	if (collider != nullptr)
		collider->Set_Pos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()),SDL_FLIP_NONE, 1.0f, flip);
}

void j1Enemy::OnCollision(Collider* collider)
{

}