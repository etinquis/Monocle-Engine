#pragma once

#include <string>
#include "FileNode.h"
#include <fstream>

namespace Monocle
{
	namespace FileType
	{
		class FileTypeBase
		{
		public:
			virtual void WriteTo(std::ostream &os, FileNode *node) const = 0;
			virtual void ReadFrom(std::istream &os, FileNode *node) const = 0;
		};
	}

	template <class T>
	class File
	{
	public:
		File(std::string filename)
			: filename(filename), ftype(T()), rootNode(FileNode())
		{
		}
		virtual ~File() {}

		FileNode *GetRootNode() { return &rootNode; }
		bool IsFlat() { return !rootNode.HasChildren(); }

		void Load()
		{
			if(Exists(filename))
			{
				std::ifstream f(filename.c_str());
				ftype.ReadFrom(f, &rootNode);
			}
		}

		static bool Exists(std::string filename)
		{
			std::ifstream fin(filename.c_str());
			
			if(!fin)
			{
				return false;
			}

			fin.close();
			return true;
		}

		void Save()
		{
			ftype.WriteToFile(filename, &rootNode);
		}

		bool operator==(const File& other)
		{
			return rootNode == other.rootNode;
		}

	protected:
		FileNode rootNode;
	private:
		File(const File &f) {}
		T ftype;
		std::string filename;
	};
}
