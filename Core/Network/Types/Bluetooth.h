#pragma once

#include "../Socket.h"

namespace Monocle
{
	namespace NetworkType
	{
		class Bluetooth : public SocketTypeBase
		{
		public:
			virtual SOCKETHANDLE CreateSocket();
			virtual void Bind();
		};
	}
}