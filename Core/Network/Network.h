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