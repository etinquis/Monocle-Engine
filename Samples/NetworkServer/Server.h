#pragma once

#include <Input.h>
#include <Scene.h>
#include <Serializable.h>

namespace NetworkServer
{
	class KeyStateChangeMessage : public Monocle::Serializable
	{
	public:
		Monocle::KeyCode key;

		static const std::string NodeName;

		void SaveTo(Monocle::FileNode *parentNode) const;
		void LoadFrom(Monocle::FileNode *myNode);
	};

	
}