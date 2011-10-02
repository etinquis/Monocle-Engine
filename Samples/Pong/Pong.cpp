#include "Pong.h"
#include <Input.h>
#include <Collision.h>
#include <Component/Entity/Collidable.h>
#include <Colliders/RectangleCollider.h>

#include <Component/Entity/Transform.h>

#include <stdlib.h>
#include <sstream>

namespace Pong
{    
    AudioAsset *sfxWall=NULL;

	Text::Text(const std::string& text, FontAsset* font)
		: Entity(), font(font), text(text)
	{
		transform = AddComponent<Transform>();
	}
	void Text::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(transform->position);
		Graphics::SetBlend(BLEND_ALPHA);
		Graphics::SetColor(Color::white);
		Graphics::BindFont(font);

		Graphics::RenderText(*font, text, 0, 0);
		Graphics::PopMatrix();
	}

    
	/*
	**************************************************
	* B a l l
	**************************************************
	*
	* Moves via velocity Vector2
	* Collides with entities tagged with "Paddle"
	* Sends a message to the scene when it goes offside
	* 
	*/

	Ball::Ball() : Entity(), texture(NULL)
	{
		transform = AddComponent<Transform>();
		collidable = AddComponent<Collidable>();
		collidable->AddTag("Ball");
		collidable->SetCollider(new RectangleCollider(25.0f, 25.0f));
		//Collision::AddRectangleCollider(this, 25.0f, 25.0f);
		velocity = Vector2::right * 200.0f;
	}

	void Ball::Update()
	{
		// store our last position
		Vector2 lastPosition = transform->position;

		// update our current position using velocity
		transform->position += velocity * Monocle::deltaTime;

		// check collisions against the paddles
		Collider* collider = collidable->Collide("Paddle");
		if (collider)
		{
			Debug::Log("Ball hit an entity tagged with 'Paddle'");
			transform->position = lastPosition;
			
			Vector2 otherPos = ((Transform*)(*collider->GetEntity())[MONOCLE_ENTITYCOMPONENT_TRANSFORM])->position;

			Vector2 diff = transform->position - otherPos;
			diff.Normalize();
			diff *= velocity.GetMagnitude();
			velocity = diff;
            
            // Calculate panning
            float pan = ((otherPos.x / Graphics::GetVirtualWidth()) - 0.5) * 2.0;

            if (sfxWall)
                sfxWall->Play(1,1.0,pan); // Play it with panning! (STEREO, baby :D)
		}

		// if we hit the top or bottom of the screen
		if (transform->position.y < 0 || transform->position.y > 600)
		{
			transform->position = lastPosition;
			velocity.y *= -1;
            
            // Calculate panning
            float pan = ((transform->position.x / Graphics::GetVirtualWidth()) - 0.5) * 2.0;
            
            if (sfxWall)
                sfxWall->Play(1,1.0,pan,2.0); // Play it higher, you won't even notice it's the same ;D
		}
	}

	void Ball::Render()
	{
		Graphics::BindTexture(texture);
		Graphics::PushMatrix();
		Graphics::Translate(transform->position);
        Graphics::SetColor(Color::white);
		Graphics::RenderQuad(25.0f, 25.0f);
		Graphics::PopMatrix();
	}

	
	/*
	**************************************************
	* P a d d l e
	**************************************************
	*
	* Moves up and down, controlled by user input
	* Is tagged with "Paddle"
	* 
	*/

	Paddle::Paddle()
		: Entity(), speed(0.0f)
	{
		collidable = AddComponent<Collidable>();
		transform = AddComponent<Transform>();

		collidable->AddTag("Paddle");
		collidable->SetCollider(new RectangleCollider(25.0f, 100.0f));
	}

	void Paddle::Update()
	{
		const float accel = 2000.0f;
		const float maxSpeed = 300.0f;
		const float friction = 500.0f;
		const float maxY = 600.0f;

		if (Input::IsKeyHeld(keyUp) || Input::IsTouchInRect(Vector2(transform->position.x-150,transform->position.y-500),
                                                            Vector2(transform->position.x+150,transform->position.y)))
		{
			speed += accel * Monocle::deltaTime;
			if (speed > maxSpeed) speed = maxSpeed;
		}
		else if (Input::IsKeyHeld(keyDown) || Input::IsTouchInRect(Vector2(transform->position.x-150,transform->position.y+1),
                                                                   Vector2(transform->position.x+150,transform->position.y+500)))
		{
			speed -= accel * Monocle::deltaTime;
			if (speed < -maxSpeed) speed = -maxSpeed;
		}
		else
		{
			if (speed > 0.0f)
			{
				speed -= friction * Monocle::deltaTime;
			}
			else if (speed < 0.0f)
			{
				speed += friction * Monocle::deltaTime;
			}

			if (abs(speed) < 0.05f)
			{
				speed = 0.0f;
			}
		}
		transform->position += Vector2::up * Monocle::deltaTime * speed;
		
		if (transform->position.y > maxY)
		{
			transform->position.y = maxY;
			speed = -1.0f;
		}
		else if (transform->position.y < 0)
		{
			transform->position.y = 0;
			speed = 1.0f;
		}
	}

	void Paddle::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(transform->position);
        Graphics::SetColor(Color::white);
		Graphics::RenderQuad(25, 100.0f);
		Graphics::PopMatrix();
	}


	/*
	**************************************************
	* G a m e S c e n e
	**************************************************
	*
	* Sets up the game's entities in Begin()
	* by creating a ball and two paddles
	* 
	* Handles messages sent to it indicating when the
	* ball has gone offside.
	* 
	*/

	void GameScene::Begin()
	{
		Debug::Log("Pong::GameScene::Begin()!");

		Scene::Begin();
        
        sfxWall = Assets::RequestAudio("AudioTest/Explosion.wav");
		
		p1Score = p2Score = 0;

		ball = new Ball();
		ResetBall();
		Add(ball);

		paddle1 = new Paddle();
		((Transform*)(*paddle1)[MONOCLE_ENTITYCOMPONENT_TRANSFORM])->position = Vector2(100, 300);
		paddle1->keyUp = KEY_W;
		paddle1->keyDown = KEY_S;
		Add(paddle1);

		paddle2 = new Paddle();
		((Transform*)(*paddle2)[MONOCLE_ENTITYCOMPONENT_TRANSFORM])->position = Vector2(700, 300);
		paddle2->keyUp = KEY_UP;
		paddle2->keyDown = KEY_DOWN;
		Add(paddle2);

		FontAsset* font = Assets::RequestFont("Pong/LiberationSans-Regular.ttf", 25.0f);
        scoreText = new Text(GetScoreString(), font);
        ((Transform*)(*scoreText)[MONOCLE_ENTITYCOMPONENT_TRANSFORM])->position = Vector2(50, 50);
        Add(scoreText);
	}

	void GameScene::ResetBall()
	{
		((Transform*)(*ball)[MONOCLE_ENTITYCOMPONENT_TRANSFORM])->position = Vector2(400, 300);
		ball->velocity = Vector2::Random() * 200.0f;
	}
    
    const std::string GameScene::GetScoreString()
    {
        std::stringstream str;
        str << "P1:  " << p1Score << " - " << " P2: " << p2Score;
        return str.str();
    }

	void GameScene::Update()
	{
		Scene::Update();

		// do pong specific update
		if (((Transform*)(*ball)[MONOCLE_ENTITYCOMPONENT_TRANSFORM])->position.x < 0)
		{
            p2Score++;
            scoreText->SetText(GetScoreString());
			ResetBall();
		}
		else if (((Transform*)(*ball)[MONOCLE_ENTITYCOMPONENT_TRANSFORM])->position.x > 800)
		{
            p1Score++;
            scoreText->SetText(GetScoreString());
			ResetBall();
		}
	}

	void GameScene::End()
	{
		Scene::End();
	}
}
