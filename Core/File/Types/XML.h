#pragma once

#include "../File.h"

class TiXmlElement;

namespace Monocle
{
	class Entity;
	class FileNode;

	namespace FileType
	{
		class XML : public FileTypeBase
		{
		public:
			XML();

			virtual void WriteTo(std::ostream &os, FileNode *node);
			virtual void ReadFrom(std::istream &is, FileNode *node);
		private:
			void WriteNode(TiXmlElement *, FileNode *);
			void ReadNode(TiXmlElement *, FileNode *);
		};
	}
}
