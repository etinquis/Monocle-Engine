#include <Network/ServerSocket.h>
#include <Network/Types/TCP.h>

#include <WinSock2.h>
#include <Serializable.h>

#include <iostream>

class Message : public Monocle::Serializable
{
public:
	std::string msg;
	int num;

	static const std::string NodeName;

	Message() : Serializable()
	{

	}

	void SaveTo(Monocle::FileNode *parentNode) const
	{
		Monocle::FileNode *myNode = parentNode->InsertEndChildNode("Message");
		myNode->Write("msg", msg);
		myNode->Write("num", num);
	}

	void LoadFrom(Monocle::FileNode *myNode)
	{
		myNode->Read("msg", msg);
		myNode->Read("num", num);
	}
};

const std::string Message::NodeName = "Message";

int main()
{
	Monocle::ServerSocket<Monocle::NetworkType::TCP> server;
	server.Bind(8080);
	server.Listen();
	Monocle::SocketStream *stream = server.Accept();

	//while(true)
	//{
		Message a;
		*stream >> a;
		std::cout << a.msg;
		std::cout << a.num;
	//}

	return 0;
}