#include "LevelEditor.h"
#include "FringeTile.h"
#include "../Level/Level.h"
#include "../Scene.h"
#include "../Input.h"
#include "../MonocleToolkit.h"
#include "../Game.h"
#include <cstdio>
#include <cmath>
#include "ImageBrowser.h"
#include "Component/Entity/Transform.h"
#include "Camera.h"

namespace Monocle
{
	/*
	Cursor::Cursor()
		: Entity()
	{
		SetLayer(-50);
		color = Color::orange;
		color.a = 0.5f;
	}

	void Cursor::Update()
	{
		position = Input::GetWorldMousePosition();
	}

	void Cursor::Render()
	{
		Graphics::Translate(position);
		Graphics::BindTexture(NULL);
		//Debug::Log("cursor, render");
		//Graphics::RenderQuad(50, 50);
		Graphics::SetColor(color);
		Graphics::Rotate(Monocle::timeSinceStart * 180, 0, 0, 1);
		Graphics::RenderLineRect(0, 0, 10,10);
		Graphics::RenderLineRect(0, 0, 2,2);
	}
	*/

	LevelEditor::LevelEditor()
		: Editor(), selectedEntity(NULL), selectedFringeTile(NULL),
		state(FTES_NONE), waitForLMBRelease(false), moveAxisLock(0), imageBrowser(NULL)
		//, cursor(NULL)
	{
		keyMove = KEY_Q;
		keyRotate = KEY_W;
		keyScale = KEY_E;
		keySelect = KEY_SPACE;
		keyFocus = KEY_Z;
		keyClone = KEY_T;
		keyColor = KEY_C;
		keyFlip = KEY_F;
		keyFlipH = KEY_H;
		keyFlipV = KEY_V;
		keyPrevTile = KEY_LEFTBRACKET;
		keyNextTile = KEY_RIGHTBRACKET;
		keyDelete = KEY_BACKSPACE;
		keyScaleDown = KEY_MINUS;
		keyScaleUp = KEY_EQUALS;
		keyOpenImageBrowser = KEY_F5;

		SetState(FTES_NONE);

		Enable();
	}

	void LevelEditor::Enable()
	{
		Editor::Enable();

		//Debug::showBounds = true;
		//Debug::render = true;

		((Transform*)(*Game::GetScene()->GetCamera())["Transform"])->rotation = 0;
	}

	void LevelEditor::Disable()
	{
		Editor::Disable();

		Select(NULL);

		//Debug::showBounds = false;
		//Debug::render = false;
	}

	// main level editor update function
	void LevelEditor::Update()
	{
		Editor::Update();

		if (isEnabled)
		{
			if (state == FTES_BROWSER)
			{
				//temphack
				if (imageBrowser)
				{
					if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
					{
						scene->Remove(imageBrowser);
						imageBrowser = NULL;
						SetState(FTES_NONE);
					}
				}
			}
			else
			{
				UpdateCamera();
				UpdateCommands();

				if (state == FTES_NONE)
				{
					UpdateSelect();
				}

				if (selectedEntity)
				{
					switch (state)
					{
					case FTES_NONE:
						UpdateOpportunity();
						break;

						//case FTES_COMMAND:
						//	UpdateCommand();
						//	break;

					case FTES_MOVE:
						UpdateMove();
						break;

					case FTES_ROTATE:
						UpdateRotate();
						break;

					case FTES_SCALE:
						UpdateScale();
						break;

					case FTES_COLOR:
						UpdateColor();
						break;
                            
                    default:
                        break;
					}
				}
			}
		}
	}

	void LevelEditor::UpdateCommands()
	{
		if (Input::IsKeyPressed(keyOpenImageBrowser))
		{
			SetState(FTES_BROWSER);

			if (imageBrowser != NULL)
			{
				scene->Remove(imageBrowser);
				imageBrowser = NULL;
			}
			imageBrowser = new ImageBrowser();
			scene->Add(imageBrowser);
			imageBrowser->ScanDirectory("graphics");
		}
	}

	// update entity selection
	void LevelEditor::UpdateSelect()
	{
		if (Input::IsKeyPressed(KEY_ESCAPE))
		{
			// toggle rendering extra stuff if we hit escape
			if (selectedEntity == NULL)
			{
				//Debug::showBounds = !Debug::showBounds;
				//Debug::render = !Debug::render;
			}
		}
		else
		{
			// only update selection if bounds are visible
			/*if (Debug::showBounds == true)
			{*/
				// if users presses LMB or the selection key
				if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keySelect))
				{
					// get the nearest entity by checking for the world mouse position in control points, from scene
					Entity *entity = scene->GetNearestEntityByControlPoint(Input::GetWorldMousePosition(), "", selectedEntity);
					if (entity)
					{
						Select(entity);
						if(Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
							waitForLMBRelease = true;
					}
				}
			//}
		}
	}

	/*
	template <class T>
	void LevelEditor::CloneEntity(T *t)
	{
		T *newT = new T(*t);
		Entity *entity = (Entity*)newT;
		entity->position = Input::GetWorldMousePosition();
		scene->Add(entity);
		Select(entity);
	}
	*/

	void LevelEditor::CloneEntity(Entity *entity, const Vector2 &position)
	{
		//Entity *newEntity = (Entity*)(new typeid(T));
		Entity *newEntity = entity->Clone();
		scene->Add(newEntity);

		if (entity->GetParent())
		{
			((Transform*)(*newEntity)["Transform"])->position = position - ((Transform*)(*entity->GetParent())["Transform"])->GetWorldPosition();
		}
		else
		{
			((Transform*)(*newEntity)["Transform"])->position = position;
		}

		// special case code
		Node *node = dynamic_cast<Node*>(newEntity);
		if (node && selectedNode)
		{
			selectedNode->InsertNext(node);
		}

		newEntity->SetParent(entity->GetParent());

		Select(newEntity);
	}

	//// cloning a node is a special case for now
	//void LevelEditor::CloneNode()
	//{
	//	if (selectedEntity && selectedNode)
	//	{
	//		Node *newNode = new Node(Input::GetWorldMousePosition());
	//		newNode->Copy(selectedNode);
	//		//if (selectedNode->GetParent())
	//		//{
	//		//	///HACK: replace with "GetLocalPosition" function
	//		//	newNode->position = Input::GetWorldMousePosition() - selectedNode->GetParent()->position;
	//		//	selectedNode->GetParent()->Add(newNode);
	//		//}
	//		//else
	//		//	scene->Add(newNode);
	//		scene->Add(newNode);
	//		selectedNode->InsertNext(newNode);
	//		Select(newNode);
	//	}
	//}

	void LevelEditor::Select(Entity *entity)
	{
		if (entity == NULL)
			Debug::Log("select none");

		selectedEntity = entity;

		//Debug::selectedEntity = selectedEntity;

		if (entity)
		{
			selectedFringeTile = dynamic_cast<FringeTile*>(entity);
			selectedNode = dynamic_cast<Node*>(entity);
		}
		else
		{
			selectedFringeTile = NULL;
		}

		if (entity != NULL)
		{
//			printf("entity type: %s and %s\n", typeid(entity).name(), typeid(entity).raw_name());
			printf("entity selected: p(%f, %f) s(%f, %f) r(%f)\n", ((Transform*)(*entity)["Transform"])->position.x, ((Transform*)(*entity)["Transform"])->position.y, ((Transform*)(*entity)["Transform"])->scale.x, ((Transform*)(*entity)["Transform"])->scale.y, ((Transform*)(*entity)["Transform"])->rotation);

			printf("(Q) move (W) rotate (E) scale\n");
		}
	}

	void LevelEditor::ApplyGrid(Entity *entity, int gridSize)
	{
		int x = (((Transform*)(*entity)["Transform"])->position.x / gridSize);
		((Transform*)(*entity)["Transform"])->position.x = (x * gridSize) + gridSize*0.5f;
		int y = (((Transform*)(*entity)["Transform"])->position.y / gridSize);
		((Transform*)(*entity)["Transform"])->position.y = (y * gridSize) + gridSize*0.5f;

		printf("applied grid %d now (%d, %d)\n", gridSize, (int)((Transform*)(*entity)["Transform"])->position.x, (int)((Transform*)(*entity)["Transform"])->position.y);
	}

	void LevelEditor::UpdateOpportunity()
	{
		if (waitForLMBRelease && Input::IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			waitForLMBRelease = false;
		}

		if (Input::IsKeyPressed(KEY_ESCAPE))
		{
			Select(NULL);
			return;
		}

		if (Input::IsKeyHeld(KEY_G))
		{
			if (Input::IsKeyPressed(KEY_1))
			{
				ApplyGrid(selectedEntity, 32);
			}
			if (Input::IsKeyPressed(KEY_2))
			{
				ApplyGrid(selectedEntity, 64);
			}
			if (Input::IsKeyPressed(KEY_3))
			{
				ApplyGrid(selectedEntity, 128);
			}
			if (Input::IsKeyPressed(KEY_4))
			{
				ApplyGrid(selectedEntity, 256);
			}
			if (Input::IsKeyPressed(KEY_5))
			{
				ApplyGrid(selectedEntity, 512);
			}
		}


		if (Input::IsKeyPressed(keyFocus))
		{
			//Graphics::MoveCameraPosition(selectedEntity->position, 0.125f, EASE_OUTSIN);
			((Transform*)(*scene->GetMainCamera())["Transform"])->position = ((Transform*)(*selectedEntity)["Transform"])->position;
		}

		if (Input::IsKeyHeld(keyFlip))
		{
			if (Input::IsKeyPressed(keyFlipH))
			{
				((Transform*)(*selectedEntity)["Transform"])->scale.x *= -1;
			}
			if (Input::IsKeyPressed(keyFlipV))
			{
				((Transform*)(*selectedEntity)["Transform"])->scale.y *= -1;
			}
		}

		if (selectedNode)
		{
			/*
			if (Input::IsKeyPressed(keyClone))
			{
				CloneNode();
			}
			*/
			if (Input::IsKeyPressed(keyDelete))
			{
				Node *lastSelectedNode = selectedNode;
				Select(selectedNode->GetPrev());
				lastSelectedNode->TakeOut();
				lastSelectedNode->RemoveSelf();
				return;
			}

			if (Input::IsKeyPressed(KEY_LEFTBRACKET))
			{
				selectedNode->variant--;
			}
			if (Input::IsKeyPressed(KEY_RIGHTBRACKET))
			{
				selectedNode->variant++;
			}

			/*
			const float sizeSpeed = 32.0f;
			if (Input::IsKeyHeld(keyScaleDown))
			{
				selectedNode->scale -= sizeSpeed * Monocle::deltaTime;
			}
			if (Input::IsKeyHeld(keyScaleUp))
			{
				selectedNode->size += sizeSpeed * Monocle::deltaTime;
			}
			*/

			//printf("selectedNode->size: %f\n", selectedNode->scale.y);
		}
		if (selectedFringeTile)
		{
			if (Input::IsKeyPressed(KEY_B))
			{
				selectedFringeTile->PrevBlend();
			}
			if (Input::IsKeyPressed(KEY_N))
			{
				selectedFringeTile->NextBlend();
			}

			if (Input::IsKeyPressed(KEY_LEFTBRACKET))
			{
				selectedFringeTile->PrevTile();
			}
			if (Input::IsKeyPressed(KEY_RIGHTBRACKET))
			{
				selectedFringeTile->NextTile();
			}
			//if (Input::IsKeyPressed(keyClone))
			//{
			//	CloneEntity(selectedFringeTile);
			//}
			if (Input::IsKeyPressed(KEY_M))
			{
				std::string path = Monocle::GetWorkingDirectory() + selectedFringeTile->sprite->texture->filename;
				Debug::Log("Opening: " + path + " ...");
				Monocle::OpenURL(path);
			}
			if (Input::IsKeyPressed(KEY_R))
			{
				selectedFringeTile->sprite->texture->Reload();
			}

			if (Input::IsKeyHeld(KEY_LSHIFT) && Input::IsKeyPressed(KEY_0))
			{
				((Transform*)(*selectedEntity)["Transform"])->rotation = 0;
			}

		}

		if (Input::IsKeyPressed(keyClone))
		{
			CloneEntity(selectedEntity, Input::GetWorldMousePosition());
		}

		if (!selectedNode)
		{
			if (Input::IsKeyPressed(keyDelete))
			{
				scene->Remove(selectedEntity);
				Select(NULL);
				return;
			}
		}


		if (Input::IsKeyPressed(keyScaleUp))
		{
			Vector2 add = Vector2(SIGNOF(((Transform*)(*selectedEntity)["Transform"])->scale.x), SIGNOF(((Transform*)(*selectedEntity)["Transform"])->scale.y)) * 0.125f;
			((Transform*)(*selectedEntity)["Transform"])->scale += add;
			printf("add(%f, %f)\n", add.x, add.y);
		}
		if (Input::IsKeyPressed(keyScaleDown))
		{
			((Transform*)(*selectedEntity)["Transform"])->scale -= Vector2(SIGNOF(((Transform*)(*selectedEntity)["Transform"])->scale.x), SIGNOF(((Transform*)(*selectedEntity)["Transform"])->scale.y)) * 0.125f;
		}


		if (Input::IsKeyPressed(KEY_I))
		{
			selectedEntity->AdjustLayer(1);
			printf("layer is now: %d\n", selectedEntity->GetLayer());
		}
		if (Input::IsKeyPressed(KEY_K))
		{
			selectedEntity->AdjustLayer(-1);
			printf("layer is now: %d\n", selectedEntity->GetLayer());
		}

		if (!waitForLMBRelease)
		{
			if (Input::IsKeyHeld(keyRotate))
			{
				moveStartPosition = Input::GetWorldMousePosition();
				startRotation = ((Transform*)(*selectedEntity)["Transform"])->rotation;
				SetState(FTES_ROTATE);
				return;
			}

			if (Input::IsKeyHeld(keyScale))
			{
				Debug::Log("scale start");
				moveStartPosition = Input::GetWorldMousePosition();
				moveStartMagnitude = (Input::GetWorldMousePosition() - ((Transform*)(*selectedEntity)["Transform"])->position.xy()).GetMagnitude();
				startScale = ((Transform*)(*selectedEntity)["Transform"])->scale;
				SetState(FTES_SCALE);
				return;
			}

			if (Input::IsKeyPressed(keyMove))//Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) //
			{
				Debug::Log("move start");
				moveStartPosition = ((Transform*)(*selectedEntity)["Transform"])->position.xy();
				moveOffset = ((Transform*)(*selectedEntity)["Transform"])->position.xy() - Input::GetWorldMousePosition();
				moveAxisLock = 0;
				SetState(FTES_MOVE);
				return;
			}

			if (Input::IsKeyPressed(keyColor))
			{
				Debug::Log("color start");
				startColor = selectedEntity->color;
				changeColorValue = 0;
				SetState(FTES_COLOR);
				return;
			}
		}
	}

	void LevelEditor::UpdateMove()
	{
		if (Input::IsKeyPressed(KEY_X))
		{
			if (moveAxisLock == 1)
				moveAxisLock = 0;
			else
				moveAxisLock = 1;
		}
		else if (Input::IsKeyPressed(KEY_Y))
		{
			if (moveAxisLock == 2)
				moveAxisLock = 0;
			else
				moveAxisLock = 2;
		}

		((Transform*)(*selectedEntity)["Transform"])->position = Input::GetWorldMousePosition() + moveOffset;

		switch(moveAxisLock)
		{
		case 1:
			((Transform*)(*selectedEntity)["Transform"])->position.y = moveStartPosition.y;
			break;
		case 2:
			((Transform*)(*selectedEntity)["Transform"])->position.x = moveStartPosition.x;
			break;
		}
		
		/*
		if (Input::IsKeyPressed(KEY_0))
		{
			selectedEntity->position = Vector2::zero;
			SetState(FTES_NONE);
			return;
		}
		*/

		// move to inside UpdateMove()
		/*
		const float moveSpeed = 10.0f;
		if (Input::IsKeyHeld(KEY_LEFT))
		{
			selectedEntity->position += Vector2::left * moveSpeed * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(KEY_RIGHT))
		{
			selectedEntity->position += Vector2::right * moveSpeed * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(KEY_UP))
		{
			selectedEntity->position += Vector2::up * moveSpeed * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(KEY_DOWN))
		{
			selectedEntity->position += Vector2::down * moveSpeed * Monocle::deltaTime;
		}
		*/

		// cancel out of move by hitting escape or rmb
		if (Input::IsKeyPressed(KEY_ESCAPE) || Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			((Transform*)(*selectedEntity)["Transform"])->position = moveStartPosition;
			SetState(FTES_NONE);
			return;
		}

		// if let go, return to none state
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keyMove) || Input::IsKeyPressed(keySelect))
		{
			SetState(FTES_NONE);
			printf("position: (%d, %d)\n", (int)((Transform*)(*selectedEntity)["Transform"])->position.x, (int)((Transform*)(*selectedEntity)["Transform"])->position.y);
			return;
		}

		UpdateNumberEntry();

		if (isNumberEntry)
		{
			switch(moveAxisLock)
			{
			case 1:
				((Transform*)(*selectedEntity)["Transform"])->position.x = numberEntryValue;
				break;
			case 2:
				((Transform*)(*selectedEntity)["Transform"])->position.y = numberEntryValue;
				break;
			default:
				((Transform*)(*selectedEntity)["Transform"])->position = Vector2::one * numberEntryValue;
				break;
			}
		}
	}

	void LevelEditor::UpdateRotate()
	{
		// let go, return to none state
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keyRotate) || Input::IsKeyPressed(keySelect))
		{
			SetState(FTES_NONE);
		}
		else
		{
			if (Input::IsKeyPressed(KEY_0))
			{
				((Transform*)(*selectedEntity)["Transform"])->rotation = 0;
				SetState(FTES_NONE);
				return;
			}
			else if (Input::IsKeyPressed(KEY_ESCAPE) || Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				((Transform*)(*selectedEntity)["Transform"])->rotation = startRotation;
				SetState(FTES_NONE);
				return;
			}
			else
			{
				float add = (Input::GetWorldMousePosition().x - moveStartPosition.x)/2.4f;
				if (Input::IsKeyHeld(KEY_LCTRL))
				{
					int grads = (startRotation + add) / (45.0f/2.0f);
					((Transform*)(*selectedEntity)["Transform"])->rotation = grads * (45.0f/2.0f);
				}
				else
				{
					((Transform*)(*selectedEntity)["Transform"])->rotation = startRotation + add;
				}
			}
		}
	}

	void LevelEditor::UpdateScale()
	{
		if (Input::IsKeyPressed(KEY_X))
		{
			if (moveAxisLock == 1)
				moveAxisLock = 0;
			else
				moveAxisLock = 1;
		}
		else if (Input::IsKeyPressed(KEY_Y))
		{
			if (moveAxisLock == 2)
				moveAxisLock = 0;
			else
				moveAxisLock = 2;
		}

		Vector2 diff = (Input::GetWorldMousePosition() - ((Transform*)(*selectedEntity)["Transform"])->position.xy());
		float mag = diff.GetMagnitude();

		Vector2 add = Vector2(SIGNOF(((Transform*)(*selectedEntity)["Transform"])->scale.x), SIGNOF(((Transform*)(*selectedEntity)["Transform"])->scale.y)) * (mag - moveStartMagnitude);

		//printf("mag: %f add: (%f, %f) scale: (%f, %f)\n", mag, add.x, add.y, selectedEntity->scale.x, selectedEntity->scale.y);

		float scaleSpeed = 1.0f/moveStartMagnitude;

		((Transform*)(*selectedEntity)["Transform"])->scale = startScale + add * scaleSpeed;

		((Transform*)(*selectedEntity)["Transform"])->scale.x = MAX(((Transform*)(*selectedEntity)["Transform"])->scale.x, 0.0f);
		((Transform*)(*selectedEntity)["Transform"])->scale.y = MAX(((Transform*)(*selectedEntity)["Transform"])->scale.y, 0.0f);

		switch(moveAxisLock)
		{
		case 1:
			((Transform*)(*selectedEntity)["Transform"])->scale.y = startScale.y;
			break;
		case 2:
			((Transform*)(*selectedEntity)["Transform"])->scale.x = startScale.x;
			break;
		}
		
		if (Input::IsKeyPressed(KEY_0))
		{
			((Transform*)(*selectedEntity)["Transform"])->scale = Vector2::one;
			SetState(FTES_NONE);
			return;
		}

		// cancel out of move by hitting escape or rmb
		if (Input::IsKeyPressed(KEY_ESCAPE) || Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			((Transform*)(*selectedEntity)["Transform"])->scale = startScale;
			SetState(FTES_NONE);
			return;
		}

		// if let go, return to none state
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || Input::IsKeyPressed(keyScale) || Input::IsKeyPressed(keySelect))
		{
			SetState(FTES_NONE);
			return;
		}
	}

	void LevelEditor::UpdateColor()
	{
		if (Input::IsKeyPressed(KEY_ESCAPE) || Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			selectedEntity->color = startColor;
			SetState(FTES_NONE);
			isNumberEntry = false;
			return;
		}

		if (Input::IsKeyPressed(KEY_RETURN) || Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			SetState(FTES_NONE);
			isNumberEntry = false;
			return;
		}

		if (Input::IsKeyPressed(KEY_R))
		{
			if (changeColorValue == 1)
				changeColorValue = 0;
			else
				changeColorValue = 1;
		}
		else if (Input::IsKeyPressed(KEY_G))
		{
			if (changeColorValue == 2)
				changeColorValue = 0;
			else
				changeColorValue = 2;
		}
		else if (Input::IsKeyPressed(KEY_B))
		{
			if (changeColorValue == 3)
				changeColorValue = 0;
			else
				changeColorValue = 3;
		}
		else if (Input::IsKeyPressed(KEY_A))
		{

			if (changeColorValue == 4)
				changeColorValue = 0;
			else
				changeColorValue = 4;
		}

		float change = 0.0f;
		float changeSpeed = 1.0f;
		if (Input::IsKeyHeld(KEY_UP))
			change = changeSpeed * Monocle::deltaTime;
		else if (Input::IsKeyHeld(KEY_DOWN))
			change = -changeSpeed * Monocle::deltaTime;

		if (change != 0.0f)
		{
			if (changeColorValue > 0)
			{
				selectedEntity->color[changeColorValue-1] += change;
			}
			else
			{
				for (unsigned int i = 0; i < 3; i++)
				{
					selectedEntity->color[i] += change;
				}
			}
		}

		UpdateNumberEntry();

		if (isNumberEntry)
		{
			if (changeColorValue > 0)
			{
				selectedEntity->color[changeColorValue-1] = numberEntryValue;
			}
			else
			{
				for (unsigned int i = 0; i < 3; i++)
				{
					selectedEntity->color[i] = numberEntryValue;
				}
			}
		}

		selectedEntity->color.Clamp();
	}

	//void LevelEditor::UpdateText()
	//{
	//	if (Input::IsKeyPressed(KEY_BACKSPACE))
	//	{
	//		if (!text.empty())
	//			text.resize(text.size()-1);
	//	}
	//	else
	//	{
	//		for (int key = KEY_A; key < KEY_Z; key++)
	//		{
	//			if (Input::IsKeyPressed((KeyCode)key))
	//			{
	//				text += 'a' + (char)key;
	//			}
	//		}
	//	}

	//	if (Input::IsKeyPressed(KEY_RETURN))
	//	{
	//		// mark textEntry as finished
	//		isTextEntered = true;
	//	}
	//}

	void LevelEditor::UpdateNumberEntry()
	{
		bool changed = false;
		if (Input::IsKeyPressed(KEY_BACKSPACE))
		{
			numberEntryString="";
			changed = true;
		}
		else
		{
			std::string oldNumberEntryString = numberEntryString;

			if (Input::IsKeyPressed(KEY_0))
				numberEntryString += "0";
			if (Input::IsKeyPressed(KEY_1))
				numberEntryString += "1";
			if (Input::IsKeyPressed(KEY_2))
				numberEntryString += "2";
			if (Input::IsKeyPressed(KEY_3))
				numberEntryString += "3";
			if (Input::IsKeyPressed(KEY_4))
				numberEntryString += "4";
			if (Input::IsKeyPressed(KEY_5))
				numberEntryString += "5";
			if (Input::IsKeyPressed(KEY_6))
				numberEntryString += "6";
			if (Input::IsKeyPressed(KEY_7))
				numberEntryString += "7";
			if (Input::IsKeyPressed(KEY_8))
				numberEntryString += "8";
			if (Input::IsKeyPressed(KEY_9))
				numberEntryString += "9";

			if (Input::IsKeyPressed(KEY_PERIOD))
				numberEntryString += ".";

			changed = (oldNumberEntryString != numberEntryString);
		}
		if (changed)
		{
			if (numberEntryString.empty())
			{
				isNumberEntry = false;
			}
			else
			{
				std::istringstream is(numberEntryString);
				is >> numberEntryValue;
				isNumberEntry = true;

				Debug::Log("numberEntryString: " + numberEntryString);
				Debug::Log(numberEntryValue);
			}
		}
	}


	void LevelEditor::SetState(FringeTileEditorState state)
	{
		this->state = state;

		if (state == FTES_NONE)
		{
			isNumberEntry = false;
			numberEntryString = "";
			moveAxisLock = 0;
		}
	}
}
