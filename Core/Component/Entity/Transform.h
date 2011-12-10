#pragma once

#include "../EntityComponent.h"
#include "../../Vector2.h"
#include "../../Vector3.h"

namespace Monocle
{
	//!
	//! \brief Describes a transformation in 2D space, via position (horizontal (x) position, vertical (y) position), rotation (an angle) and scale (horizontal (x) size, vertical (y) size).
	//!
	class Transform : public EntityComponent
	{
	public:
		typedef class TransformInitParams
		{
		private:
			Vector2 position;
			float rotation;
			Vector2 scale;
		public:
			TransformInitParams(Vector2 &position = Vector2::zero, float rotation = 0, Vector2 &scale = Vector2::zero);
		} InitParams;

		static const std::string ComponentName;

		Transform();
		Transform(const Transform &transform);

		Vector2 position;
		float rotation;
		Vector2 scale;
        
		std::string GetName() { return Transform::ComponentName; }

		void ParamInit(InitParams params);

		void Update();
		Transform* Clone() const { return new Transform(*this); }

		Vector2 lastPositionWhenCached;
		Vector2 cachedWorldPosition;

        //! Retrieves a direction vector based on the rotation value.
        Vector2 GetDirectionVector();

		void LerpTransform(Transform *prev, Transform *next, float percent);

		void ApplyMatrix();
		Vector2 GetWorldPosition(const Vector2 &position = Vector2::zero);
	protected:
		void Save(FileNode *myNode);
		void Load(FileNode *myNode);
	};
}