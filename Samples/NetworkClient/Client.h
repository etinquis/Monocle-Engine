#pragma once

#include <Input.h>
#include <Scene.h>
#include <Network/SocketStream.h>
#include <Serializable.h>

namespace NetworkClient
{
	class KeyStateChangeMessage : public Monocle::Serializable
	{
	public:
		Monocle::KeyCode key;

		static const std::string NodeName;

		void SaveTo(Monocle::FileNode *parentNode) const;
		void LoadFrom(Monocle::FileNode *myNode);
	};

	class ClientScene : public Monocle::Scene, Monocle::Input::EventHandler
	{
	public:
		ClientScene(Monocle::SocketStream *ss);
		virtual ~ClientScene();
		void Begin();

		void OnKeyPress(Monocle::KeyCode key);
	private:
		Monocle::SocketStream *stream;
	};
}