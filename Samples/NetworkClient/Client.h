#pragma once

#include <Input.h>
#include <Scene.h>
#include <Network/ClientSocket.h>
#include <Network/Types/TCP.h>
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
		void Begin();
		void End();

		void OnKeyPress(Monocle::KeyCode key);
	private:
		Monocle::ClientSocket<Monocle::NetworkType::TCP> socket;
		Monocle::SocketStream *stream;
	};
}