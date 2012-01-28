#include "Server.h"

namespace NetworkServer
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
}