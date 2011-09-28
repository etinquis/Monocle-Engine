#include "Transform.h"
#include "../../MonocleToolkit.h"
#include "../../Graphics.h"

namespace Monocle
{
	Transform::Transform() 
		: EntityComponent(), position(Vector3::zero), rotation(0.0f), scale(Vector2::one)
	{
	}

	Transform::Transform(const Transform &transform)
		: EntityComponent(transform), position(transform.position), rotation(transform.rotation), scale(transform.scale)
	{
	}
    
    Vector2 Transform::GetDirectionVector()
    {
        return Vector2(sin(rotation*pi / 180.0),cos(rotation*pi / 180.0)*-1.0);
    }

	void Transform::ApplyMatrix()
	{
		//if (followCamera == Vector2::zero /*|| (Debug::render && Debug::selectedEntity != this && IsDebugLayer())*/)
			Graphics::Translate(position.x, position.y, position.z);
		//else
		//{
			/*Camera *camera = scene->GetActiveCamera();
			if (!camera)
				camera = scene->GetMainCamera();
			if (camera != NULL)
				Graphics::Translate(camera->position * followCamera + position * (Vector2::one - followCamera));*/
		//}
        
		if (rotation != 0.0f)
			Graphics::Rotate(rotation, 0, 0, 1);
        
        if (scale != Vector2::one)
            Graphics::Scale(scale);
	}

	void Transform::LerpTransform(Transform *prev, Transform *next, float percent)
	{
		position = Vector3(next->position.xy()*percent + prev->position.xy()*(1.0f - percent), position.z);
		rotation = next->rotation*percent + prev->rotation*(1.0f - percent);
		scale = next->scale*percent + prev->scale*(1.0f-percent);
	}

	Vector2 Transform::GetWorldPosition(const Vector2 &position)
	{
		Vector2 returnPos;
        
        if (this->position.xy() == lastPositionWhenCached)
            return this->cachedWorldPosition;

		Graphics::PushMatrix();
		Graphics::IdentityMatrix();
        
        Graphics::Translate(position);

		returnPos = Graphics::GetMatrixPosition();

		Graphics::PopMatrix();

        this->cachedWorldPosition = returnPos;
        this->lastPositionWhenCached = this->position.xy();
        
		return returnPos;
	}
}