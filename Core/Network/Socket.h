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
#include <cstring>
#include <iostream>
#include "../Debug.h"

namespace Monocle
{
	class Serializable;

	//! Base class for socket protocols/types.  E.g TCP, UDP, Bluetooth, etc.
	class SocketTypeBase
	{
	public:
		virtual SOCKETHANDLE CreateSocket() = 0;
	};

	//! Stream class that facilitates communication between a pair of sockets.
	//! The << and >> operators are used to send and recieve Serializable objects
	//! over the stream.
	//! Supports a subset of std::ios functionality, mainly relating to error state.
	class SocketStream
	{
	public:
		static const int BUFF_SIZE = 500;

		SocketStream(SOCKETHANDLE sHandle, bool client = false);
		virtual ~SocketStream();

		//! Serializes the given obj and sends it across the socket.  Immediately returns
		//! if there are any error bits set.
		SocketStream &operator <<(const Serializable &obj);
		//! Attemps to read a serialized object from the socket.  This is a blocking call -
		//! if there isn't currently data waiting to be read, it will not return until there
		//! is.  Sets error bits in case of failure, in which case the passed obj should not
		//! be considered valid.
		SocketStream &operator >>(Serializable &obj);

		//! Returns a bool indicating whether the stream has no error flags set.
		virtual bool good() const;
		//! Returns a bool indicating whether either of the failbit or the badbit error
		//! flags are set.
		virtual bool fail() const;
		//! Returns a bool indicating whether the stream has been closed.
		virtual bool eof() const;
		//! Returns a bool indicating whether the badbit error flag has been set.
		virtual bool bad() const;
		//! Sets the stream state to the given state.  Overwrites current state.
		virtual void clear(std::ios_base::iostate state = std::ios_base::goodbit);
		//! Sets the stream state to the given state OR'd with the current state.
		virtual void setstate(std::ios_base::iostate state);
		//! Returns the current state of the stream.
		virtual std::ios_base::iostate rdstate() const;

		//! Equivalent to the fail() function.
		bool operator !();
		//! Similar to std::ios void* operator.  The returned pointer is not intended to
		//! be used as a reference, it simply indicates the state of the stream.  If null
		//! is returned, the stream has an error flag set.
		operator void*();
	protected:
		SOCKETHANDLE handle;
		FileType::minifiedjson fType;
		char buffer[BUFF_SIZE];
		std::ios_base::iostate state;
		bool isClient;
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
		operator void*()
		{
			if(closed)
			{
				return null;
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
