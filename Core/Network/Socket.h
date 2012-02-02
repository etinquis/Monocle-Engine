#pragma once

#include "Network.h"
#include <vector>
#include "../Debug.h"

namespace Monocle
{
	class Serializable;
	class SocketStream;

	//! Base class for socket protocols/types.  E.g TCP, UDP, Bluetooth, etc.
	class SocketTypeBase
	{
	public:
		virtual SOCKETHANDLE CreateSocket() = 0;
		//! Attempts to find a connection in the linked list of the addr parameter
		//! that matches the protocol and type of this socket.  Returns the size of the
		//! result sockaddr and sets result if a valid connection is found, otherwise result
		//! is NULL and returns -1.
		virtual int FindAddr(const addrinfo *addr, sockaddr &result) = 0;
	};

	//! Internal class managing platform-specific initialization requirements for sockets.
	class SocketInit
	{
	public:
		bool SocketsAvailable;
		SocketInit();
		~SocketInit();
	};

	//! Base class for client and server sockets.  Implements
	//! shared functionality.  Shouldn't be used directly,
	//! instead use either ClientSocket or ServerSocket.
	template<typename SocketType>
	class Socket
	{
	public:
		typedef std::vector<SocketStream *> StreamList;
		static const int BUFF_SIZE = 500;
		
		//! Creates a new socket based on the type given in the template parameter.
		//! If the initialization or the socket creation fails, the socket will
		//! be closed and any subsequent attempts to cast the Socket to a void* will
		//! return NULL.  This allows the following construct to be used to initialize
		//! and check a socket:
		//! Socket mySocket;
		//! if(mySocket)
		//! {
		//!	  //socket is valid
		//! }
		//! else
		//! {
		//!   //socket initialization failed
		//! }
		Socket()
		{
			if(!init.SocketsAvailable)
			{
				Close();
				return;
			}

			SocketHandle = sType.CreateSocket();
			if(SocketHandle == -1)
			{
				Debug::Log("Could not create socket");
				Close();
			}
		}
		virtual ~Socket()
		{
			Close();
		}

		//! Deletes any existing SocketStreams associated with this socket and
		//! closes it.
		void Close()
		{
			while(this->streams.size() > 0)
			{
				delete (*this->streams.back());
				this->streams.pop_back();
			}
			SOCKETCLOSE(SocketHandle);
			closed = true;
		}
		//! If the socket is closed, returns NULL
		//! otherwise returns a non-null number.
		//! This should not be treated as an actual
		//! pointer to anything.
		operator void*()
		{
			if(closed)
			{
				return NULL;
			}
			return this;
		}

	protected:
		SOCKETHANDLE SocketHandle;
		bool closed;
		std::vector<SocketStream *> streams;
		SocketType sType;

	private:
		static SocketInit init;
	};

	template<typename T>
	SocketInit Socket<T>::init;
}
