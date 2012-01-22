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

#include "../FileNode.h"

namespace Monocle
{
	class SocketStream
	{
		SocketStream(SOCKETHANDLE handle);

		template <typename T>
		SocketStream &operator <<(const T &obj)
		{
			return *this;
		}

		template <typename T>
		SocketStream &operator >>(T &obj)
		{
			return *this;
		}

	private:
		SOCKETHANDLE SocketHandle;
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