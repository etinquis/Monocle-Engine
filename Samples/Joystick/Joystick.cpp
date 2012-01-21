#include "Joystick.h"
#include <Input.h>
#include <Collision.h>

#include <stdlib.h>
#include <sstream>

namespace Joystick
{
	JoystickAxis::JoystickAxis(const Vector2& pos, int axisId, Monocle::Joystick *js)
	{
		this->basePos = pos;
		this->position = pos;
		this->axis = axisId;
		this->js = js;
		this->stick = Assets::RequestTexture("Joystick/stick.png");
	}

	void JoystickAxis::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::SetBlend(Monocle::BLEND_ALPHA);
		Graphics::BindTexture(stick);
		Graphics::RenderQuad(stick->width, stick->height);
		Graphics::PopMatrix();
	}

	void JoystickAxis::Update()
	{
		Vector2 axis = js->GetAxis(this->axis);
		this->position =
			Vector2(
				this->basePos.x + (10 * axis.x),
				this->basePos.y + (10 * axis.y)
			);
	}


	JoystickButton::JoystickButton(const Vector2& pos, int btnId, Monocle::Joystick *js)
	{
		this->position = pos;
		this->button = btnId;
		std::stringstream str;
		str << "Joystick/btn" << btnId << ".png";
		this->btn = Assets::RequestTexture(str.str());
		this->js = js;
		this->pressed = false;
	}

	void JoystickButton::Update()
	{
		if(js->isButtonHeld(button))
		{
			pressed = true;
		}
		else
		{
			pressed = false;
		}
	}

	void JoystickButton::Render()
	{
		if(pressed)
		{
			Graphics::PushMatrix();
			Graphics::Translate(position);
			Graphics::SetBlend(Monocle::BLEND_ALPHA);
			Graphics::BindTexture(btn);
			Graphics::RenderQuad(btn->width, btn->height);
			Graphics::PopMatrix();
		}
	}


    Joystick::Joystick()
	{
		sprite = Assets::RequestTexture("Joystick/joystick.png");
		js = Input::GetJoystick(0);
		LStick = new JoystickAxis(Vector2( -75,35 ), 0, js);
		RStick = new JoystickAxis(Vector2( 75,35 ), 1, js);

		Square = new JoystickButton(Vector2( 115, -42), Monocle::Joystick::JSBUTTON_PS_SQUARE, js);
		Triangle = new JoystickButton(Vector2( 152, -80), 3, js);
		Cross = new JoystickButton(Vector2( 152, -4), 1, js);
		Circle = new JoystickButton(Vector2( 190, -42), 2, js);

		Start = new JoystickButton(Vector2( 43,-34 ), 9, js);
		Select = new JoystickButton(Vector2( -46,-34 ), 8, js);
	}

	void Joystick::Render()
	{
		Graphics::PushMatrix();
		Graphics::IdentityMatrix();
		Graphics::Translate(Graphics::GetScreenCenter());
		Graphics::BindTexture(sprite);
		Graphics::RenderQuad(sprite->width, sprite->height);
		Graphics::BindTexture(NULL);
		LStick->Render();
		RStick->Render();
		Square->Render();
		Triangle->Render();
		Cross->Render();
		Circle->Render();
		Start->Render();
		Select->Render();
		Graphics::PopMatrix();
	}

	void Joystick::Update()
	{
		LStick->Update();
		RStick->Update();
		
		Square->Update();
		Triangle->Update();
		Cross->Update();
		Circle->Update();

		Start->Update();
		Select->Update();
	}

	void GameScene::Begin()
	{
		js = new Joystick();
		Add(js);
	}

	void GameScene::End()
	{
		delete js;
	}
}
