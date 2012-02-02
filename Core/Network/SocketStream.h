#pragma once

#include <ios>
#include "../Serializable.h"
#include "../File/Types/json.h"
#include "Network.h"

namespace Monocle
{
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

		std::streamsize sputn(const char *s, std::streamsize n);
		std::streamsize sgetn(char *s, std::streamsize n);

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
}