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

			virtual void WriteTo(std::ostream &os, FileNode *node) const;
			virtual void ReadFrom(std::istream &is, FileNode *node) const;
		protected:
			void WriteNode(Json::Value *, FileNode *) const;
			void ReadNode(Json::Value *, FileNode *) const;
		};

		class minifiedjson : public json
		{
		public:
			virtual void WriteTo(std::ostream &os, FileNode *node) const;
		};
	}
}
