#include <Network/ClientSocket.h>
#include <Network/Types/TCP.h>

#include <Serializable.h>

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

int main()
{
	Monocle::ClientSocket<Monocle::NetworkType::TCP> socket;
	Monocle::SocketStream *stream = socket.Connect(8080, "127.0.0.1");
	Message m;
	m.msg = "Test";
	m.num = 123;
	//while(true)
	//{
		*stream << m;
	//}
}