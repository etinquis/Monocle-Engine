#pragma once

#ifdef MONOCLE_WINDOWS
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#ifdef MONOCLE_WINDOWS
#define SOCKETHANDLE SOCKET
#define SOCKETIPv4 AF_INET
#define SOCKETCLOSE closesocket
#else
#define SOCKETHANDLE int
#define SOCKETIPv4 PF_INET
#define SOCKETCLOSE close
#endif

#include "../File/FileNode.h"
#include "../File/Types/json.h"
#include <vector>
#include "../Debug.h"
#include <cstring>

namespace Monocle
{
	class Serializable;

	class SocketTypeBase
	{
	public:
		virtual SOCKETHANDLE CreateSocket() = 0;
	};

	class SocketStream
	{
	public:
		static const int BUFF_SIZE = 500;

		SocketStream(SOCKETHANDLE sHandle);

		bool isOpen();

		SocketStream &operator <<(const Serializable &obj);
		SocketStream &operator >>(Serializable &obj);

	protected:
		SOCKETHANDLE handle;
		FileType::json fType;
		char buffer[BUFF_SIZE];
		bool closed;
	};

	template<typename SocketType>
	class Socket
	{
	public:
		static const int BUFF_SIZE = 500;
		
		Socket()
		{
			if(!initialized)
			{
				Init();
			}
			
			SocketHandle = sType.CreateSocket();
			if(SocketHandle == -1)
			{
				Debug::Log("Could not create socket");
			}
		}
		~Socket()
		{
			Close();
		}

		static void Init()
		{
#ifdef MONOCLE_WINDOWS
			WSADATA data;
			WSAStartup(MAKEWORD(2,2), &data);
#endif
			initialized = true;
		}

		void Close()
		{
			for(std::vector<SocketStream*>::iterator it = streams.begin(); it != streams.end(); it++)
			{
				delete (*it);
			}
			SOCKETCLOSE(SocketHandle);
		}

	protected:
		SOCKETHANDLE SocketHandle;
		static bool initialized;

		std::vector<SocketStream *> streams;

		SocketType sType;
	};
	
	template<typename T>
	bool Socket<T>::initialized = false;
}
