#include "FileNode.h"
#include "Vector2.h"

namespace Monocle
{
	FileNode::FileNode()
		: name("root")
	{

	}

	FileNode::FileNode(const std::string &name)
		: name(name)
	{

	}

	FileNode *FileNode::InsertEndChildNode(const std::string &name)
	{
		children.insert(std::pair<std::string, FileNode>(name, FileNode(name)));
		return &children[name];
	}

	FileNode *FileNode::GetChild(const std::string &name)
	{
		if(children.find(name) != children.end())
		{
			return &children[name];
		}
		
		return NULL;
	}

	std::string FileNode::GetName()
	{
		return name;
	}

	bool FileNode::HasChildren()
	{
		return children.size() > 0;
	}
}