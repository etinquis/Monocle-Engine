#include "ImageBrowser.h"
#include "../Input.h"
#include "../MonocleToolkit.h"
#include "../Graphics/Sprite.h"
#include "../Game.h"
#include "Component/Entity/Transform.h"
#include "Camera.h"

namespace Monocle
{
	SelectionImage::SelectionImage(const std::string &image, int size)
		: Entity()
	{
		Sprite *sprite = NULL;
		SetGraphic(sprite = new Sprite(image));
		if (sprite->width > sprite->height)
		{
			sprite->height = (sprite->height/sprite->width) * size;
			sprite->width = size;
		}
		else
		{
			sprite->width = (sprite->width/sprite->height) * size;
			sprite->height = size;
		}
	}

	void SelectionImage::Update()
	{
		Entity::Update();

		Vector2 diff = Input::GetWorldMousePosition() - ((Transform*)(*this)["Transform"])->GetWorldPosition();//position - Graphics::GetScreenCenter();
		float mag = (1.0f-(diff.GetMagnitude()/256.0f)) * 1.0f;
		if (mag < 0.5f) mag = 0.5f;
		if (mag > 1.25f) mag = 1.25f;
		((Transform*)(*this)["Transform"])->scale = Vector2::one * mag;
		printf("scale %f, %f\n", ((Transform*)(*this)["Transform"])->scale.x, ((Transform*)(*this)["Transform"])->scale.y);
	}

	/// IMAGE BROWSER
	ImageBrowser::ImageBrowser()
		: Entity(), selectionImageSize(128), hasContent(false)
	{
		SetLayer(-85);
		followCamera = Vector2::one;
	}

	void ImageBrowser::Update()
	{
		((Transform*)(*this)["Transform"])->scale = ((Transform*)(*Game::GetScene()->GetCamera())["Transform"])->scale;
		((Transform*)(*this)["Transform"])->scale = Vector2(1/((Transform*)(*this)["Transform"])->scale.x, 1/((Transform*)(*this)["Transform"])->scale.y);

		Entity::Update();

		if (hasContent)
		{
			for (int key = (int)KEY_A; key <= (int)KEY_Z; key++)
			{
				if (Input::IsKeyPressed((KeyCode)key))
				{
					// select next image with name starting with first letter "key"
					// i.e. if key A is pressed selected Aardvark
					// if key A is pressed again, select Aardvark2
				}
			}

			((Transform*)(*grid)["Transform"])->position = Vector3( ((Transform*)(*grid)["Transform"])->position.xy() + (Platform::mouseScroll * Vector2::down * 0.25f));
			printf("position %f, %f\n", ((Transform*)(*grid)["Transform"])->position.x, ((Transform*)(*grid)["Transform"])->position.y);
		}
	}

	void ImageBrowser::ScanDirectory(const std::string &directory)
	{
		hasContent = false;
		//DestroyChildren();
		selectionImages.clear();

		Entity *bg = new Entity();
		scene->Add(bg);
		bg->SetParent(this);
		bg->SetGraphic(new Sprite("", 128, 600));
		bg->color = Color::black;
		bg->color.a = 0.75f;
		

		grid = new Entity();
		scene->Add(grid);
		grid->SetParent(this);

		ForEachFile(directory, "png", &ImageBrowser::FileCallback, (intptr_t)this);
	}

	void ImageBrowser::FileCallback(const std::string &filename, intptr_t pointer)
	{
		ImageBrowser *imageBrowser = (ImageBrowser*)pointer;
		imageBrowser->NewSelectionImage(filename);
	}

	void ImageBrowser::SetImages(const std::vector<std::string> &images)
	{
		//DestroyChildren();
		selectionImages.clear();

		for (int i = 0; i < images.size(); i++)
		{
			NewSelectionImage(images[i]);
		}
	}

	void ImageBrowser::NewSelectionImage(const std::string &filename)
	{
		SelectionImage *selectionImage = new SelectionImage(filename, selectionImageSize);
		scene->Add(selectionImage);
		selectionImage->SetParent(this);
		//grid->Add(selectionImage);
		((Transform*)(*selectionImage)["Transform"])->position = Vector2(0.0f, selectionImageSize * selectionImages.size());
		
		selectionImages.push_back(selectionImage);

		hasContent = true;
	}
}
