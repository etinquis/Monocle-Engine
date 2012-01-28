#include "TCP.h"

namespace Monocle
{
	namespace NetworkType
	{
		SOCKETHANDLE TCP::CreateSocket()
		{
			return socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		}
	}
}