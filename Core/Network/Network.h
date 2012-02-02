#pragma once

#ifdef MONOCLE_WINDOWS

#ifdef __MINGW32__
#define WINVER 0x0501
#endif
#include <WinSock2.h>
#include <WS2tcpip.h>
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