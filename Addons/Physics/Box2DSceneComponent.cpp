#include "Box2DSceneComponent.h"
#include <Vector2.h>
#include <MonocleToolkit.h>

namespace Monocle
{
	const std::string Box2DSceneComponent::ComponentName = "Box2DScene";

	Box2DSceneComponent::Box2DSceneComponent()
		: world(gravity)
	{
		world.SetAllowSleeping(false);
	}

	Box2DSceneComponent::~Box2DSceneComponent()
	{

	}

	void Box2DSceneComponent::Update()
	{
		world.Step(Monocle::deltaTime, 6, 2);
	}

	void Box2DSceneComponent::Unload()
	{

	}

	void Box2DSceneComponent::SetGravity(Vector2 grav)
	{
		world.SetGravity(b2Vec2(grav.x, grav.y));
	}

	Box2DSceneComponent *Box2DSceneComponent::Clone() const
	{
		return NULL;
	}

	void Box2DSceneComponent::Save(FileNode *myNode)
	{

	}

	void Box2DSceneComponent::Load(FileNode *myNode)
	{

	}

	b2Body *Box2DSceneComponent::CreateBody(b2BodyDef &bodydef)
	{
		return world.CreateBody(&bodydef);
	}

	void Box2DSceneComponent::DestroyBody(b2Body *body)
	{
		return world.DestroyBody(body);
	}
}