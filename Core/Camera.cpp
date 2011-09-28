#include "Camera.h"
#include "Graphics.h"
#include "Component/Entity/Transform.h"

namespace Monocle
{

	Viewport::Viewport()
	{
		x = y = 0.0f;
		width = height = 1.0f;
	}

	Viewport::Viewport(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
	{
	}

	void Viewport::Set(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}


	Camera::Camera(const Camera& camera)
		: isVisible(camera.isVisible), frontLayer(camera.frontLayer), backLayer(camera.backLayer)
	{

	}

	Camera::Camera()
		: isVisible(true), frontLayer(-100), backLayer(100)
	{
		((Transform*)(*this)["Transform"])->position = Graphics::GetScreenCenter();
	}

	void Camera::SetLayers(int backLayer, int frontLayer)
	{
		this->frontLayer = frontLayer;
		this->backLayer = backLayer;
	}

	int Camera::GetBackLayer()
	{
		return backLayer;
	}

	int Camera::GetFrontLayer()
	{
		return frontLayer;
	}

	//void Camera::SetActive(bool on)
	//{
	//	isActive = on;
	//}

	//bool Camera::IsActive()
	//{
	//	return isActive;
	//}
}