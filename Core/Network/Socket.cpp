#include "Socket.h"
#include "Debug.h"

#include "Serializable.h"

namespace Monocle
{
	struct SocketMessageHeader
	{
	public:
		int startOffset; //the size of the header -- the number of bytes between the beginning of the header and the beginning of the socket data
		unsigned long dataLength;

		//Anything above is required for server compatibility.
		//Add new fields here

	};

	SocketStream::SocketStream(SOCKETHANDLE sHandle) : handle(sHandle), closed(false)
	{

	}

	bool SocketStream::isOpen()
	{
		return !closed;
	}

	SocketStream &SocketStream::operator<<(const Serializable &obj)
	{
		std::ostringstream str;

		FileNode *node = new FileNode();
		obj.SaveTo(node);
		
		fType.WriteTo(str, node);
		std::string outstr = str.str();

		//Construct header
		SocketMessageHeader header;
			header.dataLength = str.str().length();
			header.startOffset = sizeof(header);

		send(handle, (char*)&header, header.startOffset, NULL);
		send(handle, outstr.c_str(), outstr.length(), NULL);

	#ifdef DEBUG
		Debug::Log("Message sent!");
		Debug::Log("Header length:");
		Debug::Log(header.startOffset);
		Debug::Log("Data length:");
		Debug::Log((long)header.dataLength);
	#endif
		
		return *this;
	}

	SocketStream &SocketStream::operator>>(Serializable &obj)
	{
		//clear buffer
		memset(buffer, 0, BUFF_SIZE);

		//read header
		SocketMessageHeader header;
		recv(handle, (char*)&header, sizeof(SocketMessageHeader),NULL);

	#ifdef DEBUG
		Debug::Log("Message recieved!");
		Debug::Log("Header length:");
		Debug::Log(header.startOffset);
		Debug::Log("Data length:");
		Debug::Log((long)header.dataLength);
	#endif

		if(header.dataLength == 0)
		{
			Debug::Log("Empty message");
			return *this;
		}

		if(header.startOffset > sizeof(SocketMessageHeader))
		{
			//The header we recieved is larger than the one we have compiled.
			//This could mean that the client is using a newer version of the
			//SocketMessageHeader struct with more fields.

			//we can discard the extra data since we don't know what to do with it.
			recv(handle, (char*)buffer, header.startOffset - sizeof(SocketMessageHeader), NULL);
		}

		//read data
		std::stringstream str;
		
		int readtotal = 0;
		int readstep = 0;
		while(readtotal + BUFF_SIZE < header.dataLength)
		{
			readstep = recv(handle, buffer, BUFF_SIZE, NULL);

			if(readstep < 0)
			{
				//May be recoverable, may be stream error -- Needs to be checked more thoroughly
				Debug::Log("Error reading from stream");
				closed = true;
				return *this;
			}
			else if(readstep == 0)
			{
				Debug::Log("Stream closed prematurely");
				return *this;
			}

			str << buffer;
			readtotal += readstep;
		}

		readstep = recv(handle, buffer, header.dataLength - readtotal, NULL);
		
		if(readstep < 0)
		{
			//May be recoverable, may be stream error -- Needs to be checked more thoroughly
			Debug::Log("Error reading from stream");
			closed = true;
			return *this;
		}
		else if(readstep == 0)
		{
			Debug::Log("Stream closed prematurely");
			return *this;
		}

		str << buffer;
		
		//load object
		FileNode *node = new FileNode();
		fType.ReadFrom(str, node);
		obj.LoadFrom(node->GetChild(0));

		return *this;
	}
}