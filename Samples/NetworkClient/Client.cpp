#include "Client.h"

namespace NetworkClient
{
	const std::string KeyStateChangeMessage::NodeName = "KeyStateChangeMessage";

	void KeyStateChangeMessage::LoadFrom(Monocle::FileNode *myNode)
	{
		int k;
		myNode->Read("key", k);
		key = (Monocle::KeyCode)k;
	}

	void KeyStateChangeMessage::SaveTo(Monocle::FileNode *parentNode) const
	{
		int k = (int)key;
		Monocle::FileNode *myNode = parentNode->InsertEndChildNode("KeyStateChangeMessage");
		myNode->Write("key", k);
	}

	ClientScene::~ClientScene()
	{
		socket.Close();
	}

	void ClientScene::Begin()
	{
		stream = socket.Connect(8080, "127.0.0.1");
		Monocle::Input::AddHandler(this);
	}

	void ClientScene::OnKeyPress(Monocle::KeyCode key)
	{
		KeyStateChangeMessage msg;
		msg.key = key;
		*stream << msg;
	}
}