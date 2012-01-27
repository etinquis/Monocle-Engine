#include "Transform.h"
#include "File/FileNode.h"

#include "MonocleToolkit.h"
#include <math.h>
#include <stdio.h>

namespace Monocle
{
	Transform::Transform()
		: position(Vector2::zero), rotation(0.0f), scale(Vector2::one)
	{
	}

	Transform::Transform(const Transform &transform)
		: position(transform.position), rotation(transform.rotation), scale(transform.scale)
	{
	}

	void Transform::LerpTransform(Transform *prev, Transform *next, float percent)
	{
		position = next->position*percent + prev->position*(1.0f - percent);
		rotation = next->rotation*percent + prev->rotation*(1.0f - percent);
		scale = next->scale*percent + prev->scale*(1.0f-percent);
	}

	void Transform::SaveTo(FileNode *parentNode)
	{
		FileNode *myNode = parentNode->InsertEndChildNode("Transform");
		if (position != Vector2::zero)
			myNode->Write("position", position);
		if (rotation != 0)
			myNode->Write("rotation", rotation);
		if (scale != Vector2::one)
			myNode->Write("scale", scale);
	}

	void Transform::LoadFrom(FileNode *myNode)
	{
		myNode->Read("position", position);
		myNode->Read("rotation", rotation);
		myNode->Read("scale", scale);
	}
    
    Vector2 Transform::GetDirectionVector()
    {
        return Vector2(sin(rotation*pi / 180.0),cos(rotation*pi / 180.0)*-1.0);
    }
}