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
#else
#define SOCKETHANDLE int
#define SOCKETIPv4 PF_INET
#endif

#include "../File/FileNode.h"
#include "../File/Types/json.h"
#include <vector>
#include "../Debug.h"

namespace Monocle
{
	class SocketTypeBase
	{
	public:
		virtual SOCKETHANDLE CreateSocket() = 0;
	};

	class SocketStream
	{
	public:
		static const int BUFF_SIZE = 500;

		bool isOpen()
		{
			return !closed;
		}

		SocketStream(SOCKETHANDLE sHandle) : handle(sHandle), closed(false)
		{

		}

		template <typename T>
		SocketStream &operator <<(const T &obj)
		{
			std::ostringstream str;

			FileNode *node = new FileNode();
			obj.SaveTo(node);
			
			fType.WriteTo(str, node);

			std::string outstr = str.str();
			send(handle, outstr.c_str(), outstr.length(), NULL);
			
			return *this;
		}

		template <typename T>
		SocketStream &operator >>(T &obj)
		{
			std::stringstream str;

			memset(buffer, 0, BUFF_SIZE);

			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(handle, &fds);
			struct timeval tv;
			tv.tv_sec = 5;
			tv.tv_usec = 0;

			while(select(handle + 1, &fds, NULL, NULL, &tv) == 0) { }

			int readbytes = 0;
			do
			{
				buffer[0] = '\0';
				readbytes = recv(handle, buffer, BUFF_SIZE, NULL);
				str << buffer;
			}while(readbytes == BUFF_SIZE);

			if(readbytes == 0)
			{
				closed = true;
			}
			
			if(readbytes == -1)
			{
				closed = true;
				Debug::Log("Error recieving data");
			}
			else
			{
				FileNode *node = new FileNode();
				fType.ReadFrom(str, node);
				obj.LoadFrom(node->GetChild(T::NodeName));
			}
			return *this;
		}

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
			closesocket(SocketHandle);
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