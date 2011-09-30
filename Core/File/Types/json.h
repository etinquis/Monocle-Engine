#pragma once

#include "../File.h"

namespace Json
{
	class Value;
}

namespace Monocle
{
	class Entity;
	class FileNode;

	namespace FileType
	{
		class json : public FileTypeBase
		{
		public:
			json();

			virtual void WriteToFile(const std::string &filename, FileNode *node);
			virtual void ReadFromFile(const std::string &filename, FileNode *node);
		private:
			void WriteNode(Json::Value *, FileNode *);
			void ReadNode(Json::Value *, FileNode *);
		};
	}
}
