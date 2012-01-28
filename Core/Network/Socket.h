#pragma once;

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

		SocketStream(SOCKETHANDLE sHandle) : handle(sHandle)
		{

		}

		template <typename T>
		SocketStream &operator <<(const T &obj)
		{
			str.clear();

			FileNode *node = new FileNode();
			obj.SaveTo(node);
			
			fType.WriteTo(str, node);

			std::string outstr = str.str();
			send(handle, outstr.c_str(), outstr.length(), NULL);

			return *this;
		}

		template<>
		SocketStream &operator <<(const std::string &s)
		{
			str.clear();
			send(handle, s.c_str(), s.length(), NULL);

			return *this;
		}

		template <typename T>
		SocketStream &operator >>(T &obj)
		{
			str.clear();

			while(recv(handle, buffer, BUFF_SIZE, NULL) > 0)
			{
				str << buffer;
			}

			FileNode *node = new FileNode();
			fType.ReadFrom(str, node);
			obj.LoadFrom(node->GetChild(T::NodeName));

			return *this;
		}

		template <>
		SocketStream &operator >>(std::string &s)
		{
			str.clear();

			while(recv(handle, buffer, BUFF_SIZE, NULL) == BUFF_SIZE)
			{
				str << buffer;
			}

			s = str.str();
			return *this;
		}

	protected:
		SOCKETHANDLE handle;
		std::stringstream str;
		FileType::json fType;
		char buffer[BUFF_SIZE];
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