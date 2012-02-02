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

			virtual void WriteTo(std::ostream &os, FileNode *node) const;
			virtual void ReadFrom(std::istream &is, FileNode *node) const;
		protected:
			void WriteNode(TiXmlElement *, FileNode *) const;
			void ReadNode(TiXmlElement *, FileNode *) const;
		};
	}
}
