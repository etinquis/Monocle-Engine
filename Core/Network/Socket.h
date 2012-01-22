#pragma once;

#ifdef MONOCLE_WINDOWS
#include <WinSock.h>
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

namespace Monocle
{
	class SocketStream
	{
	public:
		const int BUFF_SIZE = 500;
		SocketStream(SOCKETHANDLE handle);

		template <typename T>
		SocketStream &operator <<(const T &obj)
		{
			str.clear();

			FileNode node;
			obj->SaveTo(node);
			
			fType.WriteTo(str, &node);

			std::string outstr = str.str();
			send(SocketHandle, outstr.c_str(), outstr.length(), NULL);

			return *this;
		}

		template <typename T>
		SocketStream &operator >>(T &obj)
		{
			str.clear();

			while(recv(SocketHandle, buffer, BUFF_SIZE, NULL) == BUFF_SIZE)
			{
				str << buffer;
			}

			FileNode node;
			fType.ReadFrom(str, &node);
			obj->LoadFrom(node);

			return *this;
		}

	private:
		SOCKETHANDLE SocketHandle;
		FileType::json fType;
		std::stringstream str;
		char buffer[BUFF_SIZE];
	};

	class Socket
	{
	public:
		Socket();
		~Socket();

		void Close();

	protected:
		SOCKETHANDLE SocketHandle;
	};
}