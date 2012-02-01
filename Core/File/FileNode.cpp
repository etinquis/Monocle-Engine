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
		return AddChild(name);
	}

	FileNode *FileNode::GetChild(const std::string &name)
	{
		if(lookup.find(name) != lookup.end())
		{
			return lookup[name].front();
		}
		
		return NULL;
	}

	FileNode *FileNode::GetChild(const int idx)
	{
		ChildList::iterator it = children.begin();
		for(int i = 0; i < idx; i++, it++) { }
		return &(*it);
	}

	std::list<FileNode*> FileNode::GetChildren(const std::string &name)
	{
		return lookup[name];
	}

	const std::string FileNode::GetName() const
	{
		return name;
	}

	bool FileNode::HasChildren()
	{
		return children.size() > 0;
	}

	FileNode *FileNode::AddChild(const std::string &name)
	{
		children.push_back(FileNode(name));
		if(lookup.find(name) != lookup.end())
		{
			lookup.insert( std::pair<std::string, std::list<FileNode*> >(name, std::list<FileNode*>()) );
		}
		ChildList::pointer ptr = &children.back();
		lookup[name].push_back(ptr);

		return ptr;
	}

	void FileNode::RemoveChildren(const std::string &name)
	{
		for(ChildList::iterator it = children.begin(); it != children.end(); it++)
		{
			if(it->GetName() == name)
			{
				children.erase(it);
			}
		}

		lookup.erase(name);
	}

	bool FileNode::operator==(const FileNode &other) const
	{
		return children == other.children; //(attributes == other.attributes) && (children == other.children);
	}
}