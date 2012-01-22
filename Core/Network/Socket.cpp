#include "Socket.h"



namespace Monocle
{
	Socket::Socket()
	{
		socket(SOCKETIPv4, SOCK_STREAM, IPPROTO_TCP);
	}

	Socket::~Socket()
	{
		Close();
	}

	void Socket::Close()
	{
		closesocket(SocketHandle);
	}

	/*void Socket::Send(char *buf, int buflen)
	{
		send(SocketHandle, buf, buflen, NULL); 
	}*/
}