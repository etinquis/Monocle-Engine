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

			virtual void WriteToFile(const std::string &filename, FileNode *node);
			virtual void ReadFromFile(const std::string &filename, FileNode *node);
		private:
			void WriteNode(TiXmlElement *, FileNode *);
			void ReadNode(TiXmlElement *, FileNode *);
		};
	}
}
