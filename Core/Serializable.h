#pragma once

#include "File/FileNode.h"

namespace Monocle
{
	class Serializable
	{
	public:
		Serializable() {}
		Serializable(FileNode *myNode)
		{
			LoadFrom(myNode);
		}

		virtual void LoadFrom(FileNode *myNode) = 0;
		virtual void SaveTo(FileNode *parentNode) = 0;
	};
}