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

			virtual void WriteTo(std::ostream &os, FileNode *node);
			virtual void ReadFrom(std::istream &is, FileNode *node);
		private:
			void WriteNode(Json::Value *, FileNode *);
			void ReadNode(Json::Value *, FileNode *);
		};
	}
}
