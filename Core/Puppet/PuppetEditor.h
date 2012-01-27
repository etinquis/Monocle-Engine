#pragma once

#include "../Editor.h"
#include "../Input.h"
#include "Puppet.h"

namespace Monocle
{
    class Timeline : public Entity
    {
    public:
        Timeline();
        Animation *currentAnimation;
        void Render();

		virtual void LoadFrom(FileNode *myNode);
		virtual void SaveTo(FileNode *parentNode);
    };

	class PuppetEditor : public Editor, public CameraEditor
	{
	public:
		PuppetEditor();
		void Load(const std::string &filename);
		void Enable();
		void Disable();
		void Update();
		void Added();

		KeyCode keyTogglePause;

		KeyCode keyMoveLeft;
		KeyCode keyMoveRight;
		KeyCode keyMoveUp;
		KeyCode keyMoveDown;

		KeyCode keyRotateLeft;
		KeyCode keyRotateRight;

		KeyCode keyBackwards;
		KeyCode keyForwards;

		KeyCode keySetKeyFrame;

		KeyCode keyZero;

		KeyCode keyOffset;

		Timeline *timeline;

	private:
		Puppet *puppet;
	};

}
