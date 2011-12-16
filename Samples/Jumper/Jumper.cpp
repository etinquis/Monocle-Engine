#include "Jumper.h"
#include <Input.h>
#include <Level/Level.h>
#include <Colliders/RectangleCollider.h>
#include <Component/Entity/Collidable.h>
#include <Component/Entity/Transform.h>
#include <Component/Entity/Sprite.h>

namespace Jumper
{
	/// PLAYER
	Player::Player(Vector2 pos) 
		: Entity()
	{
		Input::Events.AddHandler(this);

		transform = AddComponent<Transform>(Transform::InitParams(pos));
		collidable = AddComponent<Collidable>();
		sprite = AddComponent<Sprite>(Sprite::InitParams("Graphics/Player.png", FILTER_NONE, 64, 64));

		SetLayer(-10);

		collidable->AddTag("Player");
		collidable->SetCollider(new RectangleCollider(40, 64));

		speed = 4000.0f;
		gravity = 2000.0f;
		velocity = Vector2(0.0f, 0.0f);
		jump = gravity * 0.4f;
		maxSpeed = 400.0f;
		leanAmount = 0.05f;
	}

	void Player::Update()
	{
		// increase acceleration
		if (Input::IsKeyMaskHeld("left"))
		{
			if(velocity.x > -maxSpeed) { velocity.x -= speed * Monocle::deltaTime; }
		}
		else if (Input::IsKeyMaskHeld("right"))
		{
			if(velocity.x < maxSpeed) { velocity.x += speed * Monocle::deltaTime; }
		}
		else
		{
			velocity.x = 0;
		}

		// lean
		transform->rotation = velocity.x * leanAmount;

		// jump
		if(Input::IsKeyMaskHeld("jump") && state.onGround)
		{
			velocity.y = -jump;
			state.jumping = true;
			state.onGround = false;

			// stretch a bit when jumping
			sprite->width = 64 * 1.0f;
			sprite->height = 64 * 1.1f;
			transform->rotation = 0;
		}

		// friction

		// gravity
		velocity.y += gravity * Monocle::deltaTime;

		//move
		Vector2 lastPosition = transform->position;
		float temp = 0.001f;

		transform->position.x += velocity.x * Monocle::deltaTime;
		if(collidable->Collide("Wall") || collidable->Collide("Player"))
		{
			transform->position.x = lastPosition.x;
			velocity.x = 0.0f;
		}

		transform->position.y += velocity.y * Monocle::deltaTime;

		state.onGround = false;

		if (collidable->Collide("Wall") || collidable->Collide("Player"))
		{
			// small ground collision problem here if falling fast (warps back up too far)
			// could do a line intersection with the collider we hit
			// collider->IntersectsLine()
			// ^ if this gave us the intersection point, we could snap right to the ground instead

			transform->position.y = lastPosition.y;
			velocity.y = 0;
			state.onGround = true;
			
			// get fat when we're landing
			if (state.jumping)
			{
				sprite->width = 64 * 1.1f;
				sprite->height = 64 * 1.0f;
				transform->rotation = 0;
			}

			state.jumping = false;
		}
	}

	float Player::Sign(float i, float to)
	{
		return i < 0 ? - to : (i > 0 ? to : 0);
	}

	/// WALL
	Wall::Wall(Vector2 pos, float w, float h)
		: Entity()
	{
		transform = AddComponent<Transform>(Transform::InitParams(pos));
		collidable = AddComponent<Collidable>();

		collidable->AddTag("Wall");
		collidable->SetCollider(new RectangleCollider(w, h));

		this->width = w;
		this->height = h;
	}

	void Wall::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(transform->position);
		Graphics::BindTexture(NULL);
		Graphics::RenderQuad(width, height);
		Graphics::PopMatrix();
	}

	GameScene::GameScene()
	{
		Input::Events.AddHandler(this);
	}

	/// GAME SCENE
	void GameScene::Begin()
	{
		Debug::Log("Jumper::GameScene::Begin()!");
		Scene::Begin();

		Input::DefineMaskKey("jump", KEY_UP);
		Input::DefineMaskKey("jump", KEY_Z);
		Input::DefineMaskKey("jump", KEY_A);

		Input::DefineMaskKey("left", KEY_LEFT);
		Input::DefineMaskKey("right", KEY_RIGHT);

		Graphics::Set2D(800,600);
		Graphics::SetBackgroundColor(Color::blue * 0.1f);

		Add(new Wall(Vector2(400.0f, 500.0), 200.0f, 10.0f));
		Add(new Wall(Vector2(600.0f, 400.0), 100.0f, 10.0f));
		Add(new Wall(Vector2(200.0f, 400.0), 20.0f, 150.0f));

		SpawnPlayer(Vector2(400, 300));
	}

	void GameScene::SpawnPlayer(Vector2 pos)
	{
		player = new Player(pos);
		Add(player);
	}

	void GameScene::OnKeyPress(KeyCode key)
	{
		if(key == KEY_SPACE)
		{
			SpawnPlayer(Vector2(400.0f, 300.0f));
		}
	}

	void GameScene::OnMousePress(const Input::EventHandler::MouseButtonEventArgs& args)
	{
		if(args.button == MouseButton::MOUSE_BUTTON_LEFT)
		{
			SpawnPlayer(args.mousePosition);
		}
	}

	void GameScene::Update()
	{
		Platform::Sleep(5);

		Scene::Update();

		const std::list<Entity *> *entities = GetEntities();
		for(std::list<Entity *>::const_iterator it = entities->begin(); it != entities->end(); it++)
		{
			Player *plyr = dynamic_cast<Player*>(*it);
			if(plyr)
			{
				if ( plyr->transform->position.y > Graphics::GetVirtualHeight() )
				{
					this->Remove( plyr );
				}
			}
			
		}
	}

	void GameScene::End()
	{
		Level::End();
		delete player;
		//walls.clear();
	}
}
