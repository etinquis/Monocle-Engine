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

		//Anything above is required (in order) for server compatibility.
		//Add new fields here sequentially

	};

	SocketStream::SocketStream(SOCKETHANDLE sHandle, bool client) : handle(sHandle), state(std::ios_base::goodbit), isClient(client)
	{

	}
	SocketStream::~SocketStream()
	{
		if(isClient) return;

		SOCKETCLOSE(handle);
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

		const char *buff = outstr.c_str();
		int buffLen = outstr.length();
		int totalSent = 0;
		int sent = 0;
		while(totalSent < buffLen)
		{
			sent = send(handle, buff + totalSent, buffLen - sent, NULL);
			if(sent < 0)
			{
				Debug::Log("Error attempting to send data.");
				setstate(std::ios_base::badbit);
				return *this;
			}
		}

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

		int headerReadResult = 0;
		//read header
		SocketMessageHeader header;
		memset(&header, 0, sizeof(header));
		headerReadResult = recv(handle, (char*)&header, sizeof(SocketMessageHeader),NULL);

		if(headerReadResult < 0)
		{
			//error in stream
			Debug::Log("Error reading header from stream");
			setstate(std::ios_base::badbit);
			return *this;
		}
		else if(headerReadResult == 0)
		{
			setstate(std::ios_base::eofbit);
			return *this;
		}

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
				setstate(std::ios_base::badbit);
				return *this;
			}
			else if(readstep == 0)
			{
				Debug::Log("Stream closed prematurely");
				setstate(std::ios_base::badbit | std::ios_base::eofbit);
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
			setstate(std::ios_base::badbit);
			return *this;
		}
		else if(readstep == 0)
		{
			Debug::Log("Stream closed prematurely");
			setstate(std::ios_base::eofbit | std::ios_base::failbit);
			return *this;
		}

		str << buffer;
		
		//load object
		FileNode *node = new FileNode();
		fType.ReadFrom(str, node);
		obj.LoadFrom(node->GetChild(0));

		return *this;
	}
	
	void SocketStream::clear(std::ios_base::iostate istate)
	{
		state = istate;
	}

	void SocketStream::setstate(std::ios_base::iostate istate)
	{
		clear( rdstate() | istate );
	}

	bool SocketStream::fail() const
	{
		return (state & std::ios_base::badbit) || (state & std::ios_base::failbit);
	}

	std::ios_base::iostate SocketStream::rdstate() const
	{
		return state;
	}

	bool SocketStream::operator !()
	{
		return this->fail();
	}

	bool SocketStream::good() const
	{
		return rdstate() == 0;
	}

	bool SocketStream::bad() const
	{
		return (state & std::ios_base::badbit);
	}

	bool SocketStream::eof() const
	{
		return (state & std::ios_base::eofbit);
	}

	SocketStream::operator void *()
	{
		if(fail())
		{
			return NULL;
		}
		return (void*)this;
	}

	SocketInit::SocketInit()
	{
	#ifdef MONOCLE_WINDOWS
		WSADATA data;

		int res;
		res = WSAStartup(MAKEWORD(2,2), &data);

		switch(res)
		{
		case 0:
			SocketsAvailable = true; break;
		default:
			SocketsAvailable = false; break;
		}
	#endif
	}

	SocketInit::~SocketInit()
	{
	#ifdef MONOCLE_WINDOWS
		if(SocketsAvailable)
		{
			WSACleanup();
		}
	#endif
	}
}