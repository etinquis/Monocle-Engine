#include "Transform.h"
#include "../../MonocleToolkit.h"
#include "../../Graphics.h"
#include "../../File/FileNode.h"

#include <cmath>

namespace Monocle
{
	const std::string Transform::ComponentName = "Transform";

	Transform::Transform() 
		: EntityComponent(), position(Vector2::zero), rotation(0.0f), scale(Vector2::one)
	{
	}

	Transform::Transform(const Transform &transform)
		: EntityComponent(transform), position(transform.position), rotation(transform.rotation), scale(transform.scale)
	{
	}
    
	void Transform::Update()
	{

	}

	void Transform::ParamInit(Entity *entity, const InitParams& params)
	{
		Init(entity);

		this->position = params.position;
		this->rotation = params.rotation;
		this->scale = params.scale;
	}

    Vector2 Transform::GetDirectionVector()
    {
        return Vector2(sin(rotation*pi / 180.0),cos(rotation*pi / 180.0)*-1.0);
    }

	void Transform::ApplyMatrix()
	{
		Graphics::Translate(position.x, position.y, 0);
		
		if (rotation != 0.0f)
			Graphics::Rotate(rotation, 0, 0, 1);
        
        if (scale != Vector2::one)
            Graphics::Scale(scale);
	}

	void Transform::LerpTransform(Transform *prev, Transform *next, float percent)
	{
		position = next->position*percent + prev->position*(1.0f - percent);
		rotation = next->rotation*percent + prev->rotation*(1.0f - percent);
		scale = next->scale*percent + prev->scale*(1.0f-percent);
	}

	Vector2 Transform::GetWorldPosition(const Vector2 &position)
	{
		Vector2 returnPos;
        
        if (this->position == lastPositionWhenCached)
            return this->cachedWorldPosition;

		Graphics::PushMatrix();
		Graphics::IdentityMatrix();
        
		// TODO : reimplement MatrixChain
        Graphics::Translate(this->position);
		Graphics::Translate(position);

		returnPos = Graphics::GetMatrixPosition();

		Graphics::PopMatrix();

        this->cachedWorldPosition = returnPos;
        this->lastPositionWhenCached = this->position;
        
		return returnPos;
	}

	void Transform::Save(FileNode *myNode)
	{
		myNode->Write("position", position);
		myNode->Write("rotation", rotation);
		myNode->Write("scale", scale);
	}

	void Transform::Load(FileNode *myNode)
	{
		myNode->Read("position", position);
		myNode->Read("rotation", rotation);
		myNode->Read("scale", scale);
	}
}