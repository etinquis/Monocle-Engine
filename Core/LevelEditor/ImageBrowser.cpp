//#include "ImageBrowser.h"
//#include "../Input.h"
//#include "../MonocleToolkit.h"
//#include "Component/Entity/Sprite.h"
//#include "../Game.h"
//#include "Component/Entity/Transform.h"
//#include "Camera.h"
//
//namespace Monocle
//{
//	SelectionImage::SelectionImage(const std::string &image, int size)
//		: Entity()
//	{
//		GetComponent<Sprite>()->Load(image);
//		if (GetComponent<Sprite>()->width > GetComponent<Sprite>()->height)
//		{
//			GetComponent<Sprite>()->height = (GetComponent<Sprite>()->height/GetComponent<Sprite>()->width) * size;
//			GetComponent<Sprite>()->width = size;
//		}
//		else
//		{
//			GetComponent<Sprite>()->width = (GetComponent<Sprite>()->width/GetComponent<Sprite>()->height) * size;
//			GetComponent<Sprite>()->height = size;
//		}
//	}
//
//	void SelectionImage::Update()
//	{
//		Entity::Update();
//
//		Vector2 diff = Input::GetWorldMousePosition() - GetComponent<Transform>()->GetWorldPosition();//position - Graphics::GetScreenCenter();
//		float mag = (1.0f-(diff.GetMagnitude()/256.0f)) * 1.0f;
//		if (mag < 0.5f) mag = 0.5f;
//		if (mag > 1.25f) mag = 1.25f;
//		GetComponent<Transform>()->scale = Vector2::one * mag;
//		printf("scale %f, %f\n", GetComponent<Transform>()->scale.x, GetComponent<Transform>()->scale.y);
//	}
//
//	/// IMAGE BROWSER
//	ImageBrowser::ImageBrowser()
//		: Entity(), selectionImageSize(128), hasContent(false)
//	{
//		SetLayer(-85);
//		//followCamera = Vector2::one;
//	}
//
//	void ImageBrowser::Update()
//	{
//		GetComponent<Transform>()->scale = ((Transform*)(*Game::GetScene()->GetCamera())["Transform"])->scale;
//		GetComponent<Transform>()->scale = Vector2(1/GetComponent<Transform>()->scale.x, 1/GetComponent<Transform>()->scale.y);
//
//		Entity::Update();
//
//		if (hasContent)
//		{
//			for (int key = (int)KEY_A; key <= (int)KEY_Z; key++)
//			{
//				if (Input::IsKeyPressed((KeyCode)key))
//				{
//					// select next image with name starting with first letter "key"
//					// i.e. if key A is pressed selected Aardvark
//					// if key A is pressed again, select Aardvark2
//				}
//			}
//
//			((Transform*)(*grid)["Transform"])->position = ((Transform*)(*grid)["Transform"])->position + (Platform::mouseScroll * Vector2::down * 0.25f);
//			printf("position %f, %f\n", ((Transform*)(*grid)["Transform"])->position.x, ((Transform*)(*grid)["Transform"])->position.y);
//		}
//	}
//
//	void ImageBrowser::ScanDirectory(const std::string &directory)
//	{
//		hasContent = false;
//		//DestroyChildren();
//		selectionImages.clear();
//
//		/*Entity *bg = new Entity();
//		scene->Add(bg);
//		bg->SetParent(this);
//		bg->color = Color::black;
//		bg->color.a = 0.75f;*/
//		
//
//		grid = new Entity();
//		scene->Add(grid);
//		grid->SetParent(this);
//
//		ForEachFile(directory, "png", &ImageBrowser::FileCallback, (intptr_t)this);
//	}
//
//	void ImageBrowser::FileCallback(const std::string &filename, intptr_t pointer)
//	{
//		ImageBrowser *imageBrowser = (ImageBrowser*)pointer;
//		imageBrowser->NewSelectionImage(filename);
//	}
//
//	void ImageBrowser::SetImages(const std::vector<std::string> &images)
//	{
//		//DestroyChildren();
//		selectionImages.clear();
//
//		for (int i = 0; i < images.size(); i++)
//		{
//			NewSelectionImage(images[i]);
//		}
//	}
//
//	void ImageBrowser::NewSelectionImage(const std::string &filename)
//	{
//		SelectionImage *selectionImage = new SelectionImage(filename, selectionImageSize);
//		scene->Add(selectionImage);
//		selectionImage->SetParent(this);
//		//grid->Add(selectionImage);
//		((Transform*)(*selectionImage)["Transform"])->position = Vector2(0.0f, selectionImageSize * selectionImages.size());
//		
//		selectionImages.push_back(selectionImage);
//
//		hasContent = true;
//	}
//}
